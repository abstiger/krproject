#include "krdb/krdb.h"
#include <string.h>

#define KR_MEMALIGN(size)   (((size) + 0xf) & ~(gsize) 0xf)

#define _kr_db_alloc()         g_slice_new (T_KRDB)
#define _kr_db_alloc0()        g_slice_new0 (T_KRDB)
#define _kr_db_free1(db)       g_slice_free (T_KRDB, db)

#define _kr_table_alloc()         g_slice_new (T_KRTable)
#define _kr_table_alloc0()        g_slice_new0 (T_KRTable)
#define _kr_table_free1(table)      g_slice_free (T_KRTable, table)

#define _kr_index_alloc()         g_slice_new (T_KRIndex)
#define _kr_index_alloc0()        g_slice_new0 (T_KRIndex)
#define _kr_index_free1(index)      g_slice_free (T_KRIndex, index)

#define _kr_record_alloc()         g_slice_new (T_KRRecord)
#define _kr_record_alloc0()        g_slice_new0 (T_KRRecord)
#define _kr_record_free1(record)      g_slice_free (T_KRRecord, record)

#define _kr_record_list_alloc()         g_slice_new (T_KRRecordList)
#define _kr_record_list_alloc0()        g_slice_new0 (T_KRRecordList)
#define _kr_record_list_free1(reclst)      g_slice_free (T_KRRecordList, reclst)

#define _kr_field_def_alloc(num)   g_slice_alloc (sizeof(T_KRFieldDef)*num)
#define _kr_field_def_alloc0(num)   g_slice_alloc0 (sizeof(T_KRFieldDef)*num)
#define _kr_field_def_free1(num, fld)  g_slice_free1 (sizeof(T_KRFieldDef)*num, fld)

#define _kr_field_val_alloc(num)   g_slice_alloc (sizeof(T_KRFieldVal)*num)
#define _kr_field_val_alloc0(num)   g_slice_alloc0 (sizeof(T_KRFieldVal)*num)
#define _kr_field_val_free1(num, fld)  g_slice_free1 (sizeof(T_KRFieldVal)*num, fld)

extern void *GetFldValAssignFunc(int iDataSrcId);

#if 0
static gint
kr_sort_with_field (gconstpointer p1, gconstpointer p2, gpointer pifldid)
{
    
    gint iFldId = *(int *)pifldid;
    T_KRRecord *ptRec1 = (T_KRRecord *)p1;
    T_KRRecord *ptRec2 = (T_KRRecord *)p2;
    
    if (iFldId == 0 || ABS(iFldId) >= ptRec1->iFldCnt)
    {
        return 0;
    }
    else if (iFldId > 0)
    {   
        iFldId = iFldId - 1;
        ptRec1 = (T_KRRecord *)p1;
        ptRec2 = (T_KRRecord *)p2;
    }
    else
    {
        iFldId = -iFldId - 1;     
        ptRec1 = (T_KRRecord *)p2;
        ptRec2 = (T_KRRecord *)p1;
    }    
    
    switch(ptRec1->ptFldDef[iFldId].type)
    {
        case KR_INTEGER:
            return (ptRec1->puFldVal[iFldId].i - \
                    ptRec2->puFldVal[iFldId].i);
            break;
        case KR_DOUBLE:
            return (ptRec1->puFldVal[iFldId].d - \
                    ptRec2->puFldVal[iFldId].d);
            break;
        case KR_CHAR:
            return strcmp(ptRec1->puFldVal[iFldId].s, \
                          ptRec2->puFldVal[iFldId].s);
            break;
        default:
            return 0;
            break;
    }
    return 0;
}
#endif

static inline gint
kr_indexid_cmpare (T_KRIndex *ptIndex, gint *id)
{
    return !(*((const gint*)&ptIndex->iIndexId) == *((const gint*) id));
}

T_KRIndex*
kr_get_table_index_by_id(T_KRTable *krtable, gint id)
{
    GList *ptList = g_list_find_custom(krtable->pIndexList, &id, \
                                   (GCompareFunc)kr_indexid_cmpare);
    return (T_KRIndex *)g_list_nth_data(ptList, 0);
}

T_KRIndex*
kr_get_db_index_by_id(T_KRDB *krdb, gint id)
{
    GList *ptList = g_list_find_custom(krdb->pIndexList, &id, \
                               (GCompareFunc)kr_indexid_cmpare);
    return (T_KRIndex *)g_list_nth_data(ptList, 0);
}

static inline gint
kr_tableid_cmpare (T_KRTable *ptTable, gint *id)
{
    return !(*((const gint*)&ptTable->iTableId) == *((const gint*) id));
}

T_KRTable*
kr_get_table_by_id(T_KRDB *krdb, gint id)
{
    GList *ptList = g_list_find_custom(krdb->pTableList, &id, \
                                (GCompareFunc)kr_tableid_cmpare);
    return (T_KRTable *)g_list_nth_data(ptList, 0);
}

static T_KRIndex*
kr_new_index(T_KRTable *krtable, gint id, gchar *name, KRIndexType type, 
             gint fieldid, GHashFunc hash_func, GEqualFunc key_equal_func)
{
    KRType     FieldType = krtable->ptFieldDef[fieldid].type;
    GHashFunc  hashFunc = hash_func ? \
              hash_func:(GHashFunc ) kr_get_hash_func(FieldType);
    GEqualFunc keyEqualFunc = key_equal_func ? \
              key_equal_func:(GEqualFunc ) kr_get_key_equal_func(FieldType);
    T_KRIndex *ptIndex = _kr_index_alloc0();
    ptIndex->iIndexId = id;
    g_strlcpy(ptIndex->caIndexName, name, sizeof(ptIndex->caIndexName));
    ptIndex->cIndexType = type;
    ptIndex->iIndexFieldId = fieldid;
    ptIndex->iSortFieldNo = -1;
    ptIndex->pHashIndex = g_hash_table_new(hashFunc, keyEqualFunc);
    return ptIndex;
}


T_KRIndex*
kr_create_index(T_KRDB *krdb, T_KRTable *krtable, gint id, gchar *name, 
                KRIndexType type,gint fieldid, 
                GHashFunc hash_func, GEqualFunc key_equal_func)
{
    KRType     FieldType = krtable->ptFieldDef[fieldid].type;
    GHashFunc  hashFunc = hash_func ? \
              hash_func:(GHashFunc ) kr_get_hash_func(FieldType);
    GEqualFunc keyEqualFunc = key_equal_func ? \
              key_equal_func:(GEqualFunc ) kr_get_key_equal_func(FieldType);
    T_KRIndex *ptIndex = _kr_index_alloc0();
    ptIndex->iIndexId = id;
    g_strlcpy(ptIndex->caIndexName, name, sizeof(ptIndex->caIndexName));
    ptIndex->cIndexType = type;
    ptIndex->iIndexFieldId = fieldid;
    ptIndex->iSortFieldNo = -1;

    if (type == KR_INDEX_TABLE)
    {
        ptIndex->pHashIndex = g_hash_table_new(hashFunc, keyEqualFunc);
        krtable->pIndexList = g_list_append(krtable->pIndexList, ptIndex);
        return ptIndex;
    }
    else if (type == KR_INDEX_DB)
    {
        T_KRIndex *ptDBIndex = kr_get_db_index_by_id(krdb, id);
        if (ptDBIndex != NULL) 
        {
            ptIndex->pHashIndex = ptDBIndex->pHashIndex;
            krtable->pIndexList = g_list_append(krtable->pIndexList, ptIndex);
            return ptIndex;
        }
        else
        {    
            ptIndex->pHashIndex = g_hash_table_new(hashFunc, keyEqualFunc);
            krtable->pIndexList = g_list_append(krtable->pIndexList, ptIndex);
            krdb->pIndexList = g_list_append(krdb->pIndexList, ptIndex);
            return ptIndex;
        }    
    }
    return NULL;
}

void
kr_drop_table_index(T_KRIndex *krindex, T_KRTable *krtable)
{
    if (krindex->cIndexType == KR_INDEX_TABLE)
        g_hash_table_destroy(krindex->pHashIndex);
    krtable->pIndexList = g_list_remove(krtable->pIndexList, krindex);
    _kr_index_free1 (krindex);
}


void
kr_drop_db_index(T_KRIndex *krindex, T_KRDB *krdb)
{
    g_hash_table_destroy(krindex->pHashIndex);
    krdb->pIndexList = g_list_remove(krdb->pIndexList, krindex);
}


static T_KRRecord* 
kr_create_record(T_KRTable *krtable, gpointer source)
{
    guint offset = 0;
    gchar *recaddr = NULL;
    recaddr = &krtable->pRecordBuff[krtable->uiRecordLoc * krtable->iRecordSize];
    
    T_KRRecord *ptRecord = (T_KRRecord *)recaddr;
    if (ptRecord->ptTable != NULL)
    {
        kr_delete_record(ptRecord->ptTable, ptRecord);
    }
    memset(recaddr, 0x00, krtable->iRecordSize);
    
    offset += sizeof(T_KRRecord);
    ptRecord->ptTable  = krtable;
    ptRecord->pRecBuf = &recaddr[offset];

    krtable->AssignFldValFunc(ptRecord, source);
    
    return ptRecord;
}



static void 
kr_destroy_record(T_KRRecord *krrecord)
{
    /*
    T_KRTable *ptTable = (T_KRTable *)krrecord->ptTable;
    g_slice_free1(ptTable->iRecordSize, krrecord->pRecMem);
    _kr_field_val_free1(ptTable->iFieldCnt, krrecord->puFldVal);
    _kr_record_free1 (krrecord);
    */
}


static void
kr_record_list_add(struct kr_list_head *krrecordlist, T_KRRecord *krrecord)
{
/*
    T_KRRecordList *reclst = _kr_record_list_alloc0();
    INIT_LIST_HEAD(&reclst->list);
    reclst->ptRecord = krrecord;

    kr_list_add_tail(&reclst->list, krrecordlist);
*/
}


static void
kr_record_list_del(struct kr_list_head *krrecordlist, T_KRRecord *krrecord)
{
/*
    T_KRRecordList *delrec, *p;
    
    kr_list_for_each_entry_safe(delrec, p, krrecordlist, list)//must be safe
    {
        if (delrec->ptRecord == krrecord)
        {
            kr_list_del(&delrec->list);
            _kr_record_list_free1 (delrec);
            break;
        }
    }
*/
}


static void 
kr_rebuild_index_ins(T_KRIndex *krindex, T_KRRecord *krrecord)
{
    gpointer key = kr_get_field_value(krrecord, krindex->iIndexFieldId);
    
    GList *ptNodeList = g_hash_table_lookup(krindex->pHashIndex, key);
    ptNodeList = g_list_prepend(ptNodeList, krrecord);
        
    g_hash_table_replace(krindex->pHashIndex, key, ptNodeList);
}


static void 
kr_rebuild_index_del(T_KRIndex *krindex, T_KRRecord *krrecord)
{
    gpointer key = kr_get_field_value(krrecord, krindex->iIndexFieldId);

    GList *ptNodeList = g_hash_table_lookup(krindex->pHashIndex, key);
    ptNodeList = g_list_remove(ptNodeList, krrecord);
            
    T_KRRecord *ptRecord = g_list_nth_data(ptNodeList, 0);
    if (ptRecord != NULL )
    {   
        T_KRIndex *newIndex = krindex;
        if (krrecord->ptTable != ptRecord->ptTable) /* different tables */
        {    
            newIndex = kr_get_table_index_by_id(ptRecord->ptTable, \
                                                krindex->iIndexId);
        }
        key = kr_get_field_value(ptRecord, newIndex->iIndexFieldId);
        g_hash_table_replace(newIndex->pHashIndex, key, ptNodeList);
    }
    else
    {
        g_hash_table_remove(krindex->pHashIndex, key);
    }
}



T_KRRecord * 
kr_insert_record(T_KRTable *krtable, gpointer source)
{
    /*first:create this record*/
    T_KRRecord *krrecord = kr_create_record(krtable, source);

    /*second:insert real data into the list*/
    //kr_record_list_add(&krtable->tRecordList, krrecord);
    
    /*third:rebuild all hash-indexes insert*/
    g_list_foreach(krtable->pIndexList, (GFunc)kr_rebuild_index_ins, krrecord);
    
    /*fourth:increase table records number*/
    krtable->uiRecordNum++;
    krtable->uiRecordLoc = ++krtable->uiRecordLoc % (guint)krtable->dSizeKeepValue;
    return krrecord;
}


void 
kr_delete_record(T_KRTable *krtable, T_KRRecord *krrecord)
{
    /*first:rebuild all hash-indexes delete*/
    g_list_foreach(krtable->pIndexList, (GFunc)kr_rebuild_index_del, krrecord);
    
    /*second:delete real data from the list*/
    //kr_record_list_del(&krtable->tRecordList, krrecord);
    
    /*third:destroy this record*/
    kr_destroy_record(krrecord);
    
    /*fourth:decrease table records number*/
    krtable->uiRecordNum--;
}


static void
kr_keep_record_count(T_KRTable *krtable)
{
/*
    T_KRRecordList *ptRecordList;
    while (krtable->uiRecordNum > (gint )krtable->dSizeKeepValue)
    {
        ptRecordList = kr_list_first_entry(&krtable->tRecordList, \
                                           T_KRRecordList, list);
        kr_delete_record(krtable, ptRecordList->ptRecord);
    }
*/
}


static void
kr_keep_trans_interval(T_KRTable *krtable)
{
    
}


void
kr_keep_table_size(T_KRTable *krtable)
{
    switch(krtable->cSizeKeepMode)
    {
        case KR_KEEP_RECORD_COUNT:
            kr_keep_record_count(krtable);
            break;
        case KR_KEEP_TRANS_INTERVAL:
            kr_keep_trans_interval(krtable);
            break;    
        default:
            break;    
    }
}


T_KRTable*
kr_create_table(T_KRDB *krdb, gint id, gchar *name, 
                KRSizeKeepMode keepmode, gdouble keepvalue, gint fieldcnt, 
                GFunc load_field_def_func)
{
    gint i = 0;
    T_KRTable *ptTable = _kr_table_alloc0();
    ptTable->iTableId = id;
    g_strlcpy(ptTable->caTableName, name, sizeof(ptTable->caTableName));
    ptTable->cSizeKeepMode = keepmode;
    ptTable->dSizeKeepValue = keepvalue;
    ptTable->iSortFieldNo = -1;
    ptTable->iFieldCnt = fieldcnt;
    ptTable->ptFieldDef = _kr_field_def_alloc0(ptTable->iFieldCnt);
    load_field_def_func(ptTable->ptFieldDef, &ptTable->iTableId);
    ptTable->iRecordSize = sizeof(T_KRRecord);
    for (i = 0; i<ptTable->iFieldCnt; i++)
    {
        ptTable->iRecordSize += ptTable->ptFieldDef[i].length;
    }
    ptTable->iRecordSize = KR_MEMALIGN(ptTable->iRecordSize);

    ptTable->pRecordBuff = (gchar *)g_slice_alloc0(ptTable->iRecordSize * \
                                         (guint )ptTable->dSizeKeepValue);
    ptTable->AssignFldValFunc = (GFunc)GetFldValAssignFunc(id);
    ptTable->uiRecordNum = 0;
    ptTable->uiRecordLoc = 0;
    ptTable->pIndexList = NULL;
    
    krdb->pTableList = g_list_append(krdb->pTableList, ptTable);
    return ptTable;
}

void
kr_drop_table(T_KRTable *krtable)
{
    g_slice_free1(krtable->iRecordSize * (guint )krtable->dSizeKeepValue, \
                  krtable->pRecordBuff);

    g_list_foreach (krtable->pIndexList, (GFunc)kr_drop_table_index, krtable);
    g_list_free(krtable->pIndexList);

    _kr_field_def_free1(krtable->iFieldCnt, krtable->ptFieldDef);
    _kr_table_free1 (krtable);
}

T_KRDB*
kr_create_db(gchar *name)
{
    T_KRDB *ptDB = _kr_db_alloc0();
    
    g_strlcpy(ptDB->caDBName, name, sizeof(ptDB->caDBName));
    ptDB->ptCurrTable = NULL;
    ptDB->ptCurrRecord = NULL;
    ptDB->pTableList = NULL;
    
    return ptDB;
}

void
kr_drop_db(T_KRDB *krdb)
{
    g_list_foreach (krdb->pTableList, (GFunc)kr_drop_table, NULL);
    g_list_free(krdb->pTableList);

    g_list_foreach (krdb->pIndexList, (GFunc)kr_drop_db_index, krdb);
    g_list_free(krdb->pIndexList);

    _kr_db_free1 (krdb);
}

GList* 
kr_get_record_list(T_KRIndex *krindex, gpointer key)
{
    GList *pRecList = g_hash_table_lookup(krindex->pHashIndex, key);
    return pRecList;
}
