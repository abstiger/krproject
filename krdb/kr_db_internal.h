#ifndef __KR_DB_INTERNAL_H__
#define __KR_DB_INTERNAL_H__

#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"

typedef struct _kr_db_t T_KRDB;
typedef struct _kr_table_t T_KRTable;
typedef struct _kr_index_t T_KRIndex;
typedef struct _kr_index_table_t T_KRIndexTable;
typedef struct _kr_index_slot_t T_KRIndexSolt;

typedef struct _kr_field_def_t T_KRFieldDef;
typedef struct _kr_record_t T_KRRecord;

/*the two public fieldno of ptDB'field definition*/
typedef enum {
    KR_FIELDID_PROCTIME    = 0,   /*timestamp of processing*/
    KR_FIELDID_TRANSTIME   = 1    /*timestamp of transaction*/
}E_KRPublicFieldId;

/*ptDB table size keep mode*/
typedef enum {
    KR_SIZEKEEPMODE_RECORD  = '0', /*keep record counts*/ 
    KR_SIZEKEEPMODE_TIME    = '1'  /*keep transtime interval*/
}E_KRSizeKeepMode;

/*field description*/
struct _kr_field_def_t
{
    int           id;
    char          name[30+1];
    E_KRType      type;
    int           length;
    size_t        offset;
};

/*record stored in ptDB*/
struct _kr_record_t
{
    KRFreeFunc       pfFree;
    T_KRTable        *ptTable;
    char             *pRecBuf;
};

/*index's hashtable slot define*/
struct _kr_index_slot_t
{
    E_KRType        eKeyType;
    void            *pKeyValue;
    time_t          tLocMinProcTime;    /*set while add */
    time_t          tLocMinTransTime;   /*set while add */
    time_t          tExtMaxProcTime;    /*set while remove */
    time_t          tExtMaxTransTime;   /*set while remove */
    T_KRList        *pRecList;
};

/*hash table index definition*/
struct _kr_index_t
{
    T_KRDB           *ptDB;
    int              iIndexId;
    char             caIndexName[30+1];
    char             caIndexDesc[100+1];
    E_KRType         eIndexFieldType;
    T_KRHashTable    *pHashTable;
    T_KRList         *pIndexTableList;    /* tables in this index */
};

struct _kr_table_t
{
    pthread_mutex_t  tLock;
    T_KRDB           *ptDB;
    int              iTableId;
    char             caTableName[30+1];
    char             caTableDesc[100+1];
    E_KRSizeKeepMode eKeepMode;
    long             lKeepValue;
    int              iFieldCnt;         /* field count of this table */
    T_KRFieldDef     *ptFieldDef;       /* field define of this table */
    int              iRecordSize;       /* record size needed to allocated */
    char             *pRecordBuff;      /* pointer to this table's buffer */
    unsigned int     uiRecordLoc;       /* current record location*/
    unsigned int     uiRecordNum;       /* total records number*/
    T_KRList         *pIndexTableList;  /* indexes of this table */
};

struct _kr_index_table_t
{
    T_KRIndex        *ptIndex;
    T_KRTable        *ptTable;
    int              iIndexFieldId;
    int              iSortFieldId;
};

struct _kr_db_t
{
    char             caDBName[30+1];       /* name of this db */
    T_DbsEnv         *dbsenv;
    T_KRList         *pTableList;          /* tables in this db */
    T_KRList         *pIndexList;          /* indexes of this db */
    T_KRList         *pIndexTableList;     /* indexes of this db */
};


static inline char* kr_field_get_name(T_KRRecord *ptRecord, int ifldid)
{ 
    return ((T_KRTable *) ptRecord->ptTable)->ptFieldDef[ifldid].name;
}

static inline int kr_field_get_length(T_KRRecord *ptRecord, int ifldid)
{ 
    return ((T_KRTable *) ptRecord->ptTable)->ptFieldDef[ifldid].length;
}

static inline E_KRType kr_field_get_type(T_KRRecord *ptRecord, int ifldid)
{ 
    return ((T_KRTable *) ptRecord->ptTable)->ptFieldDef[ifldid].type;
}

static inline void* kr_field_get_value(T_KRRecord *ptRecord, int ifldid)
{
    size_t field_offset = \
         ((T_KRTable *) ptRecord->ptTable)->ptFieldDef[ifldid].offset;
    return &ptRecord->pRecBuf[field_offset];
}

static inline time_t kr_get_proctime(T_KRRecord *ptRecord)
{
    time_t tProcTime;
    tProcTime = (time_t )*(long *)kr_field_get_value(\
            ptRecord, KR_FIELDID_PROCTIME);
    return tProcTime;
}

static inline time_t kr_get_transtime(T_KRRecord *ptRecord)
{
    time_t tTransTime;
    tTransTime = (time_t )*(long *)kr_field_get_value(\
            ptRecord, KR_FIELDID_TRANSTIME);
    return tTransTime;
}

extern T_KRRecord* kr_record_new(T_KRTable *ptTable);
extern void kr_record_free(T_KRRecord *ptRecord);
extern int kr_record_compare(T_KRRecord *ptRec1, T_KRRecord *ptRec2, int iFieldId);
extern void kr_record_insert(T_KRRecord *ptRecord);
extern void kr_record_delete(T_KRRecord *ptRecord);

extern T_KRIndex* kr_index_create(T_KRDB *ptDB,
        int iIndexId, char *psIndexName, 
        E_KRType eIndexFieldType);
extern void kr_index_drop(T_KRIndex *ptIndex);
extern T_KRIndex* kr_index_get(T_KRDB *ptDB, int iIndexId);

extern T_KRTable* kr_table_create(T_KRDB *ptDB,
        int iTableId, char *psTableName, 
        E_KRSizeKeepMode eKeepMode, long lKeepValue);
extern void kr_table_drop(T_KRTable *ptTable);
extern T_KRTable* kr_table_get(T_KRDB *ptDB, int iTableId);

extern T_KRIndexTable* kr_index_table_create(T_KRDB *ptDB,
        int iIndexId, int iTableId,
        int iIndexFieldId, int iSortFieldId);
extern void kr_index_table_drop(T_KRIndexTable *ptIndexTable);
extern T_KRIndexTable* kr_index_table_get(T_KRDB *ptDB, int iIndexId, int iTableId);

extern T_KRDB* kr_db_create(char *psDBName, T_DbsEnv *ptDbsEnv, T_KRModule *ptModule);
extern void kr_db_drop(T_KRDB *ptDB);

#endif /* __KR_DB_INTERNAL_H__ */
