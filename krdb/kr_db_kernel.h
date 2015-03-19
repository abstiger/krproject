#ifndef __KR_DB_KERNEL_H__
#define __KR_DB_KERNEL_H__

#include "krutils/kr_utils.h"

typedef void (*KRLoadDefFunc)(void *dbsenv, void *def, void *tableid);
typedef void *(*KRMapFuncPre)(void *msg);
typedef void (*KRMapFunc)(void *fldval, int fldno, int fldlen, void *data);
typedef void (*KRMapFuncPost)(void *data);

/*the two public fieldno of krdb'field definition*/
typedef enum {
    KR_FIELDNO_KRDBTIME    = 0,   /*timestamp of krdb insertion*/
    KR_FIELDNO_TRANSTIME   = 1    /*timestamp of transaction*/
}E_KRFieldNo;


/*krdb's index type*/
typedef enum {
    KR_INDEXTYPE_TABLE     = 'S', /*only for this table*/ 
    KR_INDEXTYPE_DB        = 'M'  /*a multiple table index*/
}E_KRIndexType;

/*krdb table size keep mode*/
typedef enum {
    KR_SIZEKEEPMODE_RECORD  = '0', /*keep record counts*/ 
    KR_SIZEKEEPMODE_TIME    = '1'  /*keep transtime interval*/
}E_KRSizeKeepMode;

/*field description*/
typedef struct _kr_field_def_t
{
    int           id;
    char          name[30+1];
    E_KRType      type;
    int           length;
    size_t        offset;
}T_KRFieldDef;

/*record stored in krdb*/
typedef struct _kr_record_t
{
    void          *ptTable;
    char          *pRecBuf;
}T_KRRecord;

/*hash table index definition*/
typedef struct _kr_index_t
{
    int            iIndexId;
    char           caIndexName[30+1];
    E_KRIndexType  eIndexType;
    int            iIndexFieldId;
    int            iSortFieldId;
    T_KRHashTable  *pHashTable;
}T_KRIndex;

typedef struct _kr_table_t
{
    pthread_mutex_t  tLock;
    int              iTableId;
    char             caTableName[30+1];
    KRMapFuncPre     pMapFuncPre;       /* pre handle message */
    KRMapFunc        pMapFunc;          /* assign field value */
    KRMapFuncPost    pMapFuncPost;      /* post handle message */
    E_KRSizeKeepMode eSizeKeepMode;
    long             lSizeKeepValue;
    int              iFieldCnt;         /* field count of this table */
    int              iRecordSize;       /* record size needed to allocated */
    char             *pRecordBuff;      /* pointer to this table's buffer */
    T_KRFieldDef     *ptFieldDef;       /* field define of this table */
    unsigned int     uiRecordLoc;       /* current record location*/
    unsigned int     uiRecordNum;       /* total records number*/
    T_KRList         *pIndexList;       /* indexes of this table */
}T_KRTable;

typedef struct _kr_db_t
{
    char            caDBName[30+1];       /* name of this db */
    T_KRModule      *ptModule;            /* module for this db */
    T_KRList        *pTableList;          /* tables in this db */
    T_KRList        *pIndexList;          /* indexes of this db */
}T_KRDB;


static inline char* kr_get_field_name(T_KRRecord *krrecord, int ifldid)
{ 
    return ((T_KRTable *) krrecord->ptTable)->ptFieldDef[ifldid].name;
}

static inline int kr_get_field_length(T_KRRecord *krrecord, int ifldid)
{ 
    return ((T_KRTable *) krrecord->ptTable)->ptFieldDef[ifldid].length;
}

static inline E_KRType kr_get_field_type(T_KRRecord *krrecord, int ifldid)
{ 
    return ((T_KRTable *) krrecord->ptTable)->ptFieldDef[ifldid].type;
}

static inline void* kr_get_field_value(T_KRRecord *krrecord, int ifldid)
{
    size_t field_offset = \
         ((T_KRTable *) krrecord->ptTable)->ptFieldDef[ifldid].offset;
    return &krrecord->pRecBuf[field_offset];
}

static inline time_t kr_get_transtime(T_KRRecord *ptRecord)
{
    time_t tTransTime;
    tTransTime = (time_t )*(long *)kr_get_field_value(\
            ptRecord, KR_FIELDNO_TRANSTIME);
    return tTransTime;
}


T_KRRecord*   kr_create_record_from_data(T_KRTable *krtable, void *source);
void          kr_destroy_record(T_KRRecord *krrecord);

int           kr_insert_record(T_KRRecord *krrecord, T_KRTable *krtable);
void          kr_delete_record(T_KRRecord *krrecord, T_KRTable *krtable);

T_KRIndex*    kr_create_index(T_KRDB *krdb, T_KRTable *krtable, int index_id, 
                char *index_name, E_KRIndexType index_type, 
                int index_field_id, int sort_field_id);
void          kr_drop_table_index(T_KRIndex *krindex, T_KRTable *krtable);

T_KRTable*    kr_create_table(T_KRDB *krdb, int table_id, char *table_name, 
                E_KRSizeKeepMode keep_mode, long keep_value, int field_cnt, 
                KRLoadDefFunc load_field_def_func, 
                KRMapFuncPre map_func_pre,
                KRMapFunc map_func,
                KRMapFuncPost map_func_post,
                void *param);
void          kr_drop_table(T_KRTable *krtable, T_KRDB *krdb);
void          kr_lock_table(T_KRTable *krtable);
void          kr_unlock_table(T_KRTable *krtable);

T_KRTable*    kr_get_table(T_KRDB *krdb, int id);
T_KRIndex*    kr_get_table_index(T_KRTable *krtable, int id);
T_KRIndex*    kr_get_db_index(T_KRDB *krdb, int id);
T_KRList*     kr_get_record_list(T_KRIndex *krindex, void *key);

T_KRDB*       kr_create_db(char *db_name, T_KRModule *krdbmodule);
void          kr_drop_db_index(T_KRIndex *krindex, T_KRDB *krdb);
void          kr_drop_db(T_KRDB *krdb);

#endif /* __KR_DB_KERNEL_H__ */
