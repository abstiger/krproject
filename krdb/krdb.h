#ifndef __KR_DB_H__
#define __KR_DB_H__

/*
 *  KRDB is written by Tiger
 *
 *  Special thanks to Jennifer who named this.
 *
 *  KRRules is coming soon~
 */


#include "com/krutils.h"
#include "com/krlist.h"

typedef struct _kr_index_t
{
    gint         iIndexId;
    gchar        caIndexName[30+1];
    KRIndexType  cIndexType;   /* '0'-table index,'1'-db index*/
    gint         iIndexFieldId;
    gint         iSortFieldNo; /* sorted ascending with this fieldid, negative then decending*/
    GHashTable   *pHashIndex;  /* hash-table-indexes  */
}T_KRIndex;

typedef struct _kr_table_t
{
    gint              iTableId;
    gchar             caTableName[30+1];
    KRSizeKeepMode    cSizeKeepMode;     /* '0'--keep record counts; '1'--keep transtime interval*/
    gdouble           dSizeKeepValue;    /* records number or timeinterval referred to cSizeKeepMode */
    gint              iSortFieldNo;      /* sorted ascending with this fieldid, negative then decending*/
    gint              iFieldCnt;         /* field count of this table */
    gint              iRecordSize;       /* record size needed to allocated  */
    gchar             *pRecordBuff;      /* pointer to this table's all record buffer */
    T_KRFieldDef      *ptFieldDef;       /* field define of this table */
    GFunc             AssignFldValFunc;  /* function for assigning field value */
    guint             uiRecordLoc;       /* current record location*/
    guint             uiRecordNum;       /* total records number*/
    GList             *pIndexList;       /* indexes of this table */
}T_KRTable;

typedef struct _kr_db_t
{
    gchar           caDBName[30+1];
    T_KRTable       *ptCurrTable;
    T_KRRecord      *ptCurrRecord;
    GList           *pTableList;          /* tables in this db */
    GList           *pIndexList;          /* indexes of this db */
}T_KRDB;


static inline KRType
kr_get_field_type (T_KRRecord *krrecord, gint ifldid)
{ 
    return ((T_KRTable *) krrecord->ptTable)->ptFieldDef[ifldid].type;
}

static inline void* 
kr_get_field_value (T_KRRecord *krrecord, gint ifldid)
{
    gint field_offset = \
         ((T_KRTable *) krrecord->ptTable)->ptFieldDef[ifldid].offset;
    return &krrecord->pRecBuf[field_offset];
}


T_KRRecord*   kr_insert_record(T_KRTable *krtable, gpointer source);
void          kr_delete_record(T_KRTable *krtable, T_KRRecord *krrecord);

void          kr_keep_table_size(T_KRTable *krtable);

T_KRIndex*    kr_create_index(T_KRDB *krdb, T_KRTable *krtable, 
                 gint id, gchar *name, KRIndexType type, gint fieldid, 
                 GHashFunc hash_func, GEqualFunc key_equal_func);
void          kr_drop_table_index(T_KRIndex *krindex, T_KRTable *krtable);

T_KRTable*    kr_create_table(T_KRDB *krdb, gint id, gchar *name, 
                 KRSizeKeepMode keepmode, gdouble keepvalue, gint fieldcnt, 
                 GFunc load_field_def_func);
T_KRTable*    kr_get_table_by_id(T_KRDB *krdb, gint id);
T_KRIndex*    kr_get_table_index_by_id(T_KRTable *krtable, gint id);
T_KRIndex*    kr_get_db_index_by_id(T_KRDB *krdb, gint id);
GList*        kr_get_record_list(T_KRIndex *krindex, gpointer key);
void          kr_drop_table(T_KRTable *krtable);

T_KRDB*       kr_create_db(gchar *name);
void          kr_drop_db_index(T_KRIndex *krindex, T_KRDB *krdb);
void          kr_drop_db(T_KRDB *krdb);


#endif /* __KR_DB_H__ */
