#include "krutils/kr_utils.h"
#include "kr_db_kernel.h"
#include "kr_db_mmap.h"

#define KR_MEMALIGN(size)   (((size) + 0xf) & ~(size_t) 0xf)

static inline int kr_db_field_compare(void *p1, void *p2, void *sort_field)
{
    int iFldId = *(int *)sort_field;
    T_KRRecord *ptRec1 = (T_KRRecord *)p1;
    T_KRRecord *ptRec2 = (T_KRRecord *)p2;
    
    E_KRType eType1 = kr_get_field_type(ptRec1, iFldId);
    E_KRType eType2 = kr_get_field_type(ptRec1, iFldId);
    if (eType1 != eType2) return 0;
        
    void *pVal1 = kr_get_field_value(ptRec1, iFldId);
    void *pVal2 = kr_get_field_value(ptRec2, iFldId);
    
    switch(eType1)
    {
        case KR_TYPE_INT:
            return (int )(*(int *)pVal1 - *(int *)pVal2);
        case KR_TYPE_LONG:
            return (int )(*(long *)pVal1 - *(long *)pVal2);
        case KR_TYPE_DOUBLE:
            return (int )(*(double *)pVal1 - *(double *)pVal2);
        case KR_TYPE_STRING:
            return strcmp((char *)pVal1, (char *)pVal2);
        case KR_TYPE_POINTER:
            return (int )(*(int *)pVal1 - *(int *)pVal2);
    }
    return 0;
}

static inline int kr_indexid_match(void *ptr, void *key)
{
    T_KRIndex *ptIndex = (T_KRIndex *)ptr; 
    return (*((const int*)&ptIndex->iIndexId) == *((const int*) key));
}

static inline int kr_tableid_match(void *ptr, void *key)
{
    T_KRTable *ptTable = (T_KRTable *)ptr; 
    return (*((const int*)&ptTable->iTableId) == *((const int*) key));
}

T_KRTable* kr_get_table(T_KRDB *krdb, int id)
{
    T_KRListNode *ptListNode = kr_list_search(krdb->pTableList, &id);
    if (ptListNode != NULL) {
        return (T_KRTable *)kr_list_value(ptListNode);
    }
    return NULL;
}

T_KRIndex* kr_get_table_index(T_KRTable *krtable, int id)
{
    T_KRListNode *ptListNode = kr_list_search(krtable->pIndexList, &id);
    if (ptListNode != NULL) {
        return (T_KRIndex *)kr_list_value(ptListNode);
    }
    return NULL;
}

T_KRIndex* kr_get_db_index(T_KRDB *krdb, int id)
{
    T_KRListNode *ptListNode = kr_list_search(krdb->pIndexList, &id);
    if (ptListNode != NULL) {
        return (T_KRIndex *)kr_list_value(ptListNode);
    }
    return NULL;
}


T_KRIndex* kr_create_index(T_KRDB *krdb, T_KRTable *krtable, int index_id, 
                           char *index_name, E_KRIndexType index_type, 
                           int index_field_id, int sort_field_id)
{
    T_KRIndex *ptIndex = (T_KRIndex *)kr_calloc(sizeof(T_KRIndex));
    if (ptIndex == NULL) {
        fprintf(stderr, "kr_calloc ptIndex failed!\n");
        return NULL;
    }
    ptIndex->iIndexId = index_id;
    strncpy(ptIndex->caIndexName, index_name, sizeof(ptIndex->caIndexName));
    ptIndex->eIndexType = index_type;
    ptIndex->iIndexFieldId = index_field_id;
    ptIndex->iSortFieldId = sort_field_id;
    
    E_KRType index_field_type = krtable->ptFieldDef[index_field_id].type;
    KRHashFunc hash_func = (KRHashFunc )kr_get_hash_func(index_field_type);
    KREqualFunc equal_func = (KREqualFunc )kr_get_equal_func(index_field_type);
    
    switch(ptIndex->eIndexType) 
    {
        /* table-index build with two steps:
         * 1.create new hashtable;
         * 2.add index to the table-index list
         */
        case KR_INDEXTYPE_TABLE:
        {
            ptIndex->pHashTable = kr_hashtable_new(hash_func, equal_func); 
            kr_list_add_tail(krtable->pIndexList, ptIndex);
            break;
        }
        
        /* db-index build will be a little bit complicated...
         * Check whether db-index with same index_id exists?
         * exists: link hashtable to the exist one, add to table-index list;
         * not: create an new hashtable, add to table-index and db-index list;
         */
        case KR_INDEXTYPE_DB:
        {
            T_KRIndex *ptDBIndex = kr_get_db_index(krdb, index_id);
            if (ptDBIndex != NULL) {
                ptIndex->pHashTable = ptDBIndex->pHashTable;
                kr_list_add_tail(krtable->pIndexList, ptIndex);
            } else {
                ptIndex->pHashTable = kr_hashtable_new(hash_func, equal_func); 
                kr_list_add_tail(krtable->pIndexList, ptIndex);
                kr_list_add_tail(krdb->pIndexList, ptIndex);
            }
            break;
        }
        default:
        {  
            fprintf(stderr, "Unknown index type[%c]\n", ptIndex->eIndexType);
            return NULL;
        }
    }
    
    return ptIndex;
}


void kr_drop_table_index(T_KRIndex *krindex, T_KRTable *krtable)
{
    /* here we just destroy the hashtable of table-index, 
     * since the hashtable of db-index cross tables, 
     * we will destroy that in function kr_drop_db_index 
     */
    kr_list_remove(krtable->pIndexList, krindex); 
    if (krindex->eIndexType == KR_INDEXTYPE_TABLE) {
        kr_hashtable_destroy(krindex->pHashTable);
        kr_free(krindex);
    }
}


void kr_drop_db_index(T_KRIndex *krindex, T_KRDB *krdb)
{
    kr_list_remove(krdb->pIndexList, krindex);
    kr_hashtable_destroy(krindex->pHashTable);
    kr_free(krindex);
}


static void kr_rebuild_index_ins(T_KRIndex *krindex, T_KRRecord *krrecord)
{
    void *key = kr_get_field_value(krrecord, krindex->iIndexFieldId);
    
    T_KRList *pRecList = kr_hashtable_lookup(krindex->pHashTable, key);
    if (pRecList == NULL) {
        pRecList = kr_list_new();
        kr_list_set_compare(pRecList, (KRCompareDataFunc)kr_db_field_compare);
    }

    if (krindex->iSortFieldId >= 0) {
        kr_list_add_sorted(pRecList, krrecord, &krindex->iSortFieldId);
    } else {
        kr_list_add_tail(pRecList, krrecord);
    }
        
    kr_hashtable_replace(krindex->pHashTable, key, pRecList);
}


static void kr_rebuild_index_del(T_KRIndex *krindex, T_KRRecord *krrecord)
{
    void *key = kr_get_field_value(krrecord, krindex->iIndexFieldId);

    T_KRList *pRecList = kr_hashtable_lookup(krindex->pHashTable, key);
    if (pRecList != NULL) kr_list_remove(pRecList, krrecord);
            
    if (kr_list_length(pRecList) != 0 ) {
        T_KRRecord *ptRecord = kr_list_value(kr_list_last(pRecList));
        T_KRIndex *newIndex = krindex;
        /* the dropped record and the last record of current list 
         * belongs to different tables 
         */
        if (krrecord->ptTable != ptRecord->ptTable) {
            newIndex = kr_get_table_index(ptRecord->ptTable, krindex->iIndexId);
        }
        key = kr_get_field_value(ptRecord, newIndex->iIndexFieldId);
        kr_hashtable_replace(newIndex->pHashTable, key, pRecList);
    } else {
        kr_list_destroy(pRecList);
        kr_hashtable_remove(krindex->pHashTable, key);
    }
}


int kr_insert_record(T_KRRecord *krrecord, T_KRTable *krtable)
{    
    /*first:rebuild all hash-indexes insert*/
    kr_list_foreach(krtable->pIndexList, \
                    (KRForEachFunc )kr_rebuild_index_ins, krrecord);
    
    /*secord:increase table records number*/
    if (++krtable->uiRecordNum > krtable->lSizeKeepValue) {
        krtable->uiRecordNum = krtable->lSizeKeepValue;
    }
    krtable->uiRecordLoc = (++krtable->uiRecordLoc) % krtable->lSizeKeepValue;
    
    /*third:mmap file need sync*/
    if (krtable->caMMapFile[0] != '\0')  kr_db_mmap_sync(krtable);
    
    return 0;
}


void kr_delete_record(T_KRRecord *krrecord, T_KRTable *krtable)
{
    /*first:rebuild all hash-indexes delete*/
    kr_list_foreach(krtable->pIndexList, \
                    (KRForEachFunc )kr_rebuild_index_del, krrecord);
        
    /*secord:decrease table records number*/
    if (--krtable->uiRecordNum < 0) {
        krtable->uiRecordNum = 0;
    }
    
    /*third:mmap file need sync*/
    if (krtable->caMMapFile[0] != '\0') kr_db_mmap_sync(krtable);
}


static void 
kr_assign_field_value(T_KRTable *krtable, T_KRRecord *krrecord, void *msg)
{
    int fldno = 0, fldlen = 0;
    void *data = msg, *fldval = NULL;

    if (krtable->pMapFuncPre) data=krtable->pMapFuncPre(msg);
    for (fldno=0; fldno<krtable->iFieldCnt; fldno++)
    {
        fldlen = krtable->ptFieldDef[fldno].length;
        fldval = kr_get_field_value(krrecord, fldno);
        krtable->pMapFunc(fldval, fldno, fldlen, data);
    }
    if (krtable->pMapFuncPost) krtable->pMapFuncPost(data);
}


T_KRRecord* kr_create_record_from_data(T_KRTable *krtable, void *data)
{
    unsigned int offset = 0;
    char *recaddr = NULL;
    recaddr = &krtable->pRecordBuff[krtable->uiRecordLoc*krtable->iRecordSize];
    
    T_KRRecord *ptRecord = (T_KRRecord *)recaddr;
    if (ptRecord->ptTable != NULL) {
        kr_delete_record(ptRecord, ptRecord->ptTable);
        kr_destroy_record(ptRecord);
    }
    memset(recaddr, 0x00, krtable->iRecordSize);
    
    offset += sizeof(T_KRRecord);
    ptRecord->ptTable  = krtable;
    ptRecord->pRecBuf = &recaddr[offset];

    kr_assign_field_value(krtable, ptRecord, data);
    
    return ptRecord;
}

T_KRRecord* kr_create_record_from_mmap(T_KRTable *krtable, void *mmaprec)
{
    unsigned int offset = 0;
    char *recaddr = NULL;
    recaddr = &krtable->pRecordBuff[krtable->uiRecordLoc*krtable->iRecordSize];
    
    T_KRRecord *ptRecord = (T_KRRecord *)recaddr;
    if (ptRecord->ptTable != NULL) {
        kr_delete_record(ptRecord, ptRecord->ptTable);
        kr_destroy_record(ptRecord);
    }
    memcpy(ptRecord, mmaprec, krtable->iRecordSize);
    
    offset += sizeof(T_KRRecord);
    ptRecord->ptTable  = krtable;
    ptRecord->pRecBuf = &recaddr[offset];

    return ptRecord;
}

void kr_destroy_record(T_KRRecord *krrecord)
{
    /*we do nothing with this function currently*/
}


T_KRTable* kr_create_table(T_KRDB *krdb, int table_id, 
                char *table_name, char *mmap_file, 
                E_KRSizeKeepMode keep_mode, long keep_value, int field_cnt, 
                KRLoadDefFunc load_field_def_func, 
                KRMapFuncPre map_func_pre,
                KRMapFunc map_func, 
                KRMapFuncPost map_func_post,
                void *param)
{
    T_KRTable *ptTable = kr_calloc(sizeof(T_KRTable));
    if (ptTable == NULL) {
        fprintf(stderr, "kr_calloc ptTable failed!\n");
        return NULL;
    }
    ptTable->iTableId = table_id;
    strncpy(ptTable->caTableName, table_name, sizeof(ptTable->caTableName));
    strncpy(ptTable->caMMapFile, mmap_file, sizeof(ptTable->caMMapFile));
    ptTable->eSizeKeepMode = keep_mode;
    ptTable->lSizeKeepValue = keep_value;
    ptTable->iFieldCnt = field_cnt;
    ptTable->ptFieldDef = kr_calloc(ptTable->iFieldCnt * sizeof(T_KRFieldDef));
    if (ptTable == NULL) {
        fprintf(stderr, "kr_calloc ptTable->ptFieldDef failed!\n");
        return NULL;
    }
    load_field_def_func(param, ptTable->ptFieldDef, &ptTable->iTableId);
    
    ptTable->iRecordSize = sizeof(T_KRRecord);
    int i = 0;
    for (i = 0; i<ptTable->iFieldCnt; i++) {
        ptTable->iRecordSize += ptTable->ptFieldDef[i].length;
    }
    ptTable->iRecordSize = KR_MEMALIGN(ptTable->iRecordSize);

    ptTable->pMapFuncPre = map_func_pre;
    ptTable->pMapFunc = map_func;
    ptTable->pMapFuncPost = map_func_post;
    ptTable->uiRecordNum = 0;
    ptTable->uiRecordLoc = 0;

    if (ptTable->caMMapFile[0] == '\0') {
        ptTable->pRecordBuff = \
            (char *)kr_calloc(ptTable->iRecordSize * ptTable->lSizeKeepValue);
        if (ptTable->pRecordBuff == NULL) {
            fprintf(stderr, "kr_calloc ptTable->pRecordBuff failed!\n");
            return NULL;
        }
    } else {
        if (kr_db_mmap_table(ptTable) != 0) {
            fprintf(stderr, "kr_create_mmap_file failed!\n");
            return NULL;
        }
    }
    ptTable->pIndexList = kr_list_new();
    kr_list_set_match(ptTable->pIndexList, (KRCompareFunc )kr_indexid_match);
    
    kr_list_add_tail(krdb->pTableList, ptTable);

    return ptTable;
}

void kr_drop_table(T_KRTable *krtable, T_KRDB *krdb)
{
    if (krtable->caMMapFile[0] == '\0') {
        kr_free(krtable->pRecordBuff);
    } else {
        kr_db_unmmap_table(krtable);
    }

    kr_list_remove(krdb->pTableList, krtable);
    kr_list_foreach(krtable->pIndexList, \
                    (KRForEachFunc )kr_drop_table_index, krtable);
    kr_list_destroy(krtable->pIndexList);

    kr_free(krtable->ptFieldDef);
    kr_free(krtable);
}


T_KRDB* kr_create_db(char *db_name, T_KRModule *krdbmodule)
{
    T_KRDB *ptDB = (T_KRDB *)kr_calloc(sizeof(T_KRDB));
    if (ptDB == NULL) {
        fprintf(stderr, "kr_calloc ptDB failed!\n");
        return NULL;
    }
    strncpy(ptDB->caDBName, db_name, sizeof(ptDB->caDBName));
    ptDB->ptModule = krdbmodule;
    
    ptDB->pTableList = kr_list_new();
    kr_list_set_match(ptDB->pTableList, (KRCompareFunc )kr_tableid_match);
    
    ptDB->pIndexList = kr_list_new();
    kr_list_set_match(ptDB->pIndexList, (KRCompareFunc )kr_indexid_match);
    
    return ptDB;
}

void kr_drop_db(T_KRDB *krdb)
{
    kr_list_foreach(krdb->pTableList, (KRForEachFunc)kr_drop_table, krdb);
    kr_list_destroy(krdb->pTableList);

    kr_list_foreach(krdb->pIndexList, (KRForEachFunc)kr_drop_db_index, krdb);
    kr_list_destroy(krdb->pIndexList);

    kr_free(krdb);
}

T_KRList* kr_get_record_list(T_KRIndex *krindex, void *key)
{
    T_KRList *pRecList = kr_hashtable_lookup(krindex->pHashTable, key);
    return pRecList;
}
