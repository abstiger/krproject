#ifndef __KR_DB_KERNEL_H__
#define __KR_DB_KERNEL_H__

#include "krutils/kr_utils.h"

/*the two public fieldno of krdb'field definition*/
typedef enum {
    KR_FIELDNO_KRDBTIME  = 0,    /*timestamp of krdb insertion*/
    KR_FIELDNO_TRANSTIME = 1     /*timestamp of transaction*/
}E_KRFieldNo;


/*krdb's index type*/
typedef enum {
    KR_INDEXTYPE_TABLE     = 'S', /*only for this table*/ 
    KR_INDEXTYPE_DB   = 'M'  /*a multiple table index*/
}E_KRIndexType;

/*krdb table size keep mode*/
typedef enum {
    KR_SIZEKEEPMODE_RECORD  = '0', /*keep record counts*/ 
    KR_SIZEKEEPMODE_TIME    = '1'  /*keep transtime interval*/
}E_KRSizeKeepMode;


/*record stored in krdb*/
typedef struct _kr_record_t
{
    void        *ptTable;
    char        *pRecBuf;
}T_KRRecord;

/*hash table index definition*/
typedef struct _kr_index_t
{
    int          iIndexId;
    char         caIndexName[30+1];
    E_KRIndexType  eIndexType;
    int          iIndexFieldId;
    int          iSortFieldId;
    T_KRHashTable  *pHashTable;
}T_KRIndex;

typedef struct _kr_table_t
{
    int              iTableId;
    char             caTableName[30+1];
    char             caMMapFile[100+1];
    unsigned int     uiMMapSize;
    void             *pMMapAddr;
    E_KRSizeKeepMode   eSizeKeepMode;
    long             lSizeKeepValue;
    int              iFieldCnt;         /* field count of this table */
    int              iRecordSize;       /* record size needed to allocated */
    char             *pRecordBuff;      /* pointer to this table's buffer */
    T_KRFieldDef     *ptFieldDef;       /* field define of this table */
    KRFunc           AssignFldValFunc;  /* function to assign field value */
    unsigned int     uiRecordLoc;       /* current record location*/
    unsigned int     uiRecordNum;       /* total records number*/
    T_KRList           *pIndexList;       /* indexes of this table */
}T_KRTable;

typedef struct _kr_db_t
{
    char            caDBName[30+1];
    T_KRTable       *ptCurrTable;
    T_KRRecord      *ptCurrRecord;
    T_KRList          *pTableList;          /* tables in this db */
    T_KRList          *pIndexList;          /* indexes of this db */
}T_KRDB;


static inline E_KRFieldType kr_get_field_type(T_KRRecord *krrecord, int ifldid)
{ 
    return ((T_KRTable *) krrecord->ptTable)->ptFieldDef[ifldid].type;
}

static inline void* kr_get_field_value(T_KRRecord *krrecord, int ifldid)
{
    int field_offset = \
         ((T_KRTable *) krrecord->ptTable)->ptFieldDef[ifldid].offset;
    return &krrecord->pRecBuf[field_offset];
}

T_KRRecord* kr_create_record_from_data(T_KRTable *krtable, void *source);
T_KRRecord* kr_create_record_from_mmap(T_KRTable *krtable, void *mmaprec);
void kr_destroy_record(T_KRRecord *krrecord);

int           kr_insert_record(T_KRRecord *krrecord, T_KRTable *krtable);
void          kr_delete_record(T_KRRecord *krrecord, T_KRTable *krtable);

T_KRIndex* kr_create_index(T_KRDB *krdb, T_KRTable *krtable, int index_id, 
                           char *index_name, E_KRIndexType index_type, 
                           int index_field_id, int sort_field_id);
void          kr_drop_table_index(T_KRIndex *krindex, T_KRTable *krtable);

T_KRTable* kr_create_table(T_KRDB *krdb, int table_id, 
                char *table_name, char *mmap_file, 
                E_KRSizeKeepMode keep_mode, long keep_value, int field_cnt, 
                KRFunc load_field_def_func, KRFunc assign_field_val_func);
T_KRTable*    kr_get_table(T_KRDB *krdb, int id);
T_KRIndex*    kr_get_table_index(T_KRTable *krtable, int id);
T_KRIndex*    kr_get_db_index(T_KRDB *krdb, int id);
T_KRList*       kr_get_record_list(T_KRIndex *krindex, void *key);
void          kr_drop_table(T_KRTable *krtable);

T_KRDB*       kr_create_db(char *name);
void          kr_drop_db_index(T_KRIndex *krindex, T_KRDB *krdb);
void          kr_drop_db(T_KRDB *krdb);


#endif /* __KR_DB_KERNEL_H__ */
