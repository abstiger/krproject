#include "kr_db_internal.h"



int kr_record_compare(T_KRRecord *ptRec1, T_KRRecord *ptRec2, int iFieldId)
{
    /*compare field type*/
    E_KRType eType1 = kr_field_get_type(ptRec1, iFieldId);
    E_KRType eType2 = kr_field_get_type(ptRec1, iFieldId);
    if (eType1 != eType2) return 0;
    
    /*get value compare function*/
    KRCompareFunc pfCompare = kr_get_compare_func(eType1);
    if (pfCompare == NULL) return 0;

    /*compare field value*/
    void *pVal1 = kr_field_get_value(ptRec1, iFieldId);
    void *pVal2 = kr_field_get_value(ptRec2, iFieldId);

    return pfCompare(pVal1, pVal2);
}


T_KRRecord* kr_record_new(T_KRTable *ptTable)
{
    /*get current record address*/
    size_t ulCurrRecOffset = ptTable->uiRecordLoc*ptTable->iRecordSize;
    char *psCurrRecAddr = &ptTable->pRecordBuff[ulCurrRecOffset];
    
    /*delete and free old record*/
    T_KRRecord *ptRecord = (T_KRRecord *)psCurrRecAddr;
    if (ptRecord->ptTable != NULL) {
        kr_record_delete(ptRecord);
        kr_record_free(ptRecord);
    }
    
    /*create and set new record*/
    memset(psCurrRecAddr, 0x00, ptTable->iRecordSize);
    ptRecord->pfFree = NULL;
    ptRecord->ptTable = ptTable;
    ptRecord->pRecBuf = psCurrRecAddr+sizeof(T_KRRecord);

    /*move record location to the next*/
    ptTable->uiRecordLoc = (++ptTable->uiRecordLoc)%ptTable->lKeepValue;

    return ptRecord;
}


void kr_record_free(T_KRRecord *ptRecord)
{
    if (ptRecord->pfFree) {
        ptRecord->pfFree(ptRecord);
    }
}


static void kr_rebuild_index_ins(T_KRIndexTable *ptIndextable, T_KRRecord *ptRecord)
{
    void *key = kr_field_get_value(ptRecord, ptIndextable->iIndexFieldId);
    T_KRHashTable *pHashTable = ptIndextable->ptIndex->pHashTable;

    T_KRIndexSolt *ptIndexSlot = kr_hashtable_lookup(pHashTable, key);
    if (ptIndexSlot == NULL) {
        /*create slot if not found*/
        ptIndexSlot = kr_calloc(sizeof(*ptIndexSlot));
        ptIndexSlot->eKeyType = 
            kr_field_get_type(ptRecord, ptIndextable->iIndexFieldId);
        KRDupFunc pfKeyDup = kr_get_dup_func(ptIndexSlot->eKeyType);
        ptIndexSlot->pKeyValue = pfKeyDup(key);
        ptIndexSlot->tLocMinProcTime = kr_get_proctime(ptRecord);
        ptIndexSlot->tLocMinTransTime = kr_get_transtime(ptRecord);
        ptIndexSlot->tExtMaxProcTime = kr_get_proctime(ptRecord);
        ptIndexSlot->tExtMaxTransTime = kr_get_transtime(ptRecord);
        ptIndexSlot->pRecList = kr_list_new();
        kr_hashtable_insert(pHashTable, ptIndexSlot->pKeyValue, ptIndexSlot);
    }

    /*modify statistical fields of local*/
    if (kr_get_proctime(ptRecord) < ptIndexSlot->tLocMinProcTime) {
        ptIndexSlot->tLocMinProcTime = kr_get_proctime(ptRecord);
    }
    if (kr_get_transtime(ptRecord) < ptIndexSlot->tLocMinTransTime) {
        ptIndexSlot->tLocMinTransTime = kr_get_transtime(ptRecord);
    }

    /*add record to list*/
    kr_list_add_tail(ptIndexSlot->pRecList, ptRecord);
}


static void kr_rebuild_index_del(T_KRIndexTable *ptIndextable, T_KRRecord *ptRecord)
{
    void *key = kr_field_get_value(ptRecord, ptIndextable->iIndexFieldId);
    T_KRHashTable *pHashTable = ptIndextable->ptIndex->pHashTable;

    T_KRIndexSolt *ptIndexSlot = kr_hashtable_lookup(pHashTable, key);
    if (ptIndexSlot != NULL) {
        /*modify statistical fields of external*/
        if (kr_get_proctime(ptRecord) > ptIndexSlot->tExtMaxProcTime) {
            ptIndexSlot->tExtMaxProcTime = kr_get_proctime(ptRecord);
        }
        if (kr_get_transtime(ptRecord) > ptIndexSlot->tExtMaxTransTime) {
            ptIndexSlot->tExtMaxTransTime = kr_get_transtime(ptRecord);
        }
        /*remove record from list*/
        kr_list_remove(ptIndexSlot->pRecList, ptRecord);

        /*free this slot if there is no records*/
        if (kr_list_length(ptIndexSlot->pRecList) == 0 ) {
            kr_hashtable_remove(pHashTable, ptIndexSlot->pKeyValue);
            kr_list_destroy(ptIndexSlot->pRecList);
            kr_free(ptIndexSlot->pKeyValue);
            kr_free(ptIndexSlot);
        }
    }
}


void kr_record_insert(T_KRRecord *ptRecord)
{    
    T_KRTable *ptTable = ptRecord->ptTable;
    
    /*first:rebuild all hash-indexes of this table with insert*/
    kr_list_foreach(ptTable->pIndexTableList, \
            (KRForEachFunc )kr_rebuild_index_ins, ptRecord);

    /*secord:increase table records number*/
    if (++ptTable->uiRecordNum > ptTable->lKeepValue) {
        ptTable->uiRecordNum = ptTable->lKeepValue;
    }
}


void kr_record_delete(T_KRRecord *ptRecord)
{
    T_KRTable *ptTable = ptRecord->ptTable;
    
    /*first:rebuild all hash-indexes of this table with delete*/
    kr_list_foreach(ptTable->pIndexTableList, \
            (KRForEachFunc )kr_rebuild_index_del, ptRecord);

    /*secord:decrease table records number*/
    if (--ptTable->uiRecordNum < 0) {
        ptTable->uiRecordNum = 0;
    }
}


static inline int kr_table_indexid_match(void *ptr, void *key)
{
    T_KRIndexTable *ptIndexTable = (T_KRIndexTable *)ptr; 
    return (*((const int*)&ptIndexTable->ptIndex->iIndexId) == *((const int*) key));
}

T_KRIndex* kr_index_create(T_KRDB *ptDB,
        int iIndexId, char *psIndexName, 
        E_KRType eIndexFieldType)
{
    T_KRIndex *ptIndex = (T_KRIndex *)kr_calloc(sizeof(T_KRIndex));
    if (ptIndex == NULL) {
        fprintf(stderr, "kr_calloc ptIndex failed!\n");
        return NULL;
    }
    ptIndex->ptDB = ptDB;
    ptIndex->iIndexId = iIndexId;
    strncpy(ptIndex->caIndexName, psIndexName, sizeof(ptIndex->caIndexName));
    ptIndex->eIndexFieldType = eIndexFieldType;
    KRHashFunc hash_func = (KRHashFunc )kr_get_hash_func(eIndexFieldType);
    KREqualFunc equal_func = (KREqualFunc )kr_get_equal_func(eIndexFieldType);
    ptIndex->pHashTable = kr_hashtable_new(hash_func, equal_func);

    ptIndex->pIndexTableList = kr_list_new();
    kr_list_set_match(ptIndex->pIndexTableList, 
            (KRCompareFunc )kr_table_indexid_match);

    kr_list_add_tail(ptDB->pIndexList, ptIndex);
    
    return ptIndex;
}


void kr_index_drop(T_KRIndex *ptIndex)
{
    kr_list_remove(ptIndex->ptDB->pIndexList, ptIndex);
    kr_hashtable_destroy(ptIndex->pHashTable);
    kr_list_destroy(ptIndex->pIndexTableList);
    kr_free(ptIndex);
}


T_KRIndex* kr_index_get(T_KRDB *ptDB, int iIndexId)
{
    T_KRListNode *ptListNode = kr_list_search(ptDB->pIndexList, &iIndexId);
    if (ptListNode != NULL) {
        return (T_KRIndex *)kr_list_value(ptListNode);
    }
    return NULL;
}


void kr_table_lock(T_KRTable *ptTable)
{
    pthread_mutex_lock(&ptTable->tLock);
}

void kr_table_unlock(T_KRTable *ptTable)
{
    pthread_mutex_unlock(&ptTable->tLock);
}


static inline int kr_index_tableid_match(void *ptr, void *key)
{
    T_KRIndexTable *ptIndexTable = (T_KRIndexTable *)ptr; 
    return (*((const int*)&ptIndexTable->ptTable->iTableId) == *((const int*) key));
}

T_KRTable* kr_table_create(T_KRDB *ptDB,
        int iTableId, char *psTableName, 
        E_KRSizeKeepMode eKeepMode, long lKeepValue)
{
    T_KRTable *ptTable = kr_calloc(sizeof(T_KRTable));
    if (ptTable == NULL) {
        fprintf(stderr, "kr_calloc ptTable failed!\n");
        return NULL;
    }
    pthread_mutex_init(&ptTable->tLock, NULL);
    ptTable->ptDB = ptDB;
    ptTable->iTableId = iTableId;
    strncpy(ptTable->caTableName, psTableName, sizeof(ptTable->caTableName));
    ptTable->eKeepMode = eKeepMode;
    ptTable->lKeepValue = lKeepValue;
    ptTable->uiRecordNum = 0;
    ptTable->uiRecordLoc = 0;

    ptTable->pIndexTableList = kr_list_new();
    kr_list_set_match(ptTable->pIndexTableList, 
            (KRCompareFunc )kr_index_tableid_match);

    kr_list_add_tail(ptDB->pTableList, ptTable);

    return ptTable;
}


void kr_table_drop(T_KRTable *ptTable)
{
    kr_list_remove(ptTable->ptDB->pTableList, ptTable);
    pthread_mutex_destroy(&ptTable->tLock);
    kr_list_destroy(ptTable->pIndexTableList);
    kr_free(ptTable->pRecordBuff);
    kr_free(ptTable->ptFieldDef);
    kr_free(ptTable);
}



T_KRTable* kr_table_get(T_KRDB *ptDB, int iTableId)
{
    T_KRListNode *ptListNode = kr_list_search(ptDB->pTableList, &iTableId);
    if (ptListNode != NULL) {
        return (T_KRTable *)kr_list_value(ptListNode);
    }
    return NULL;
}


T_KRIndexTable* kr_index_table_create(T_KRDB *ptDB,
        int iIndexId, int iTableId,
        int iIndexFieldId, int iSortFieldId)
{
    T_KRIndex *ptIndex = kr_index_get(ptDB, iIndexId);
    if (ptIndex == NULL) {
        fprintf(stderr, "kr_index_get [%d] failed!\n", iIndexId);
        return NULL;
    }

    T_KRTable *ptTable = kr_table_get(ptDB, iTableId);
    if (ptTable == NULL) {
        fprintf(stderr, "kr_table_get [%d] failed!\n", iTableId);
        return NULL;
    }

    T_KRIndexTable *ptIndexTable = \
        (T_KRIndexTable *)kr_calloc(sizeof(T_KRIndexTable));
    if (ptIndexTable == NULL) {
        fprintf(stderr, "kr_calloc ptIndexTable failed!\n");
        return NULL;
    }
    ptIndexTable->ptIndex = ptIndex;
    ptIndexTable->ptTable = ptTable;
    ptIndexTable->iIndexFieldId = iIndexFieldId;
    ptIndexTable->iSortFieldId = iSortFieldId;
    
    kr_list_add_tail(ptIndex->pIndexTableList, ptIndexTable);
    kr_list_add_tail(ptTable->pIndexTableList, ptIndexTable);
    kr_list_add_tail(ptDB->pIndexTableList, ptIndexTable);

    return ptIndexTable;
}


void kr_index_table_drop(T_KRIndexTable *ptIndexTable)
{
    T_KRTable *ptTable = ptIndexTable->ptTable;
    T_KRIndex *ptIndex = ptIndexTable->ptIndex;

    kr_list_remove(ptIndex->pIndexTableList, ptIndexTable);
    kr_list_remove(ptTable->pIndexTableList, ptIndexTable);

    kr_free(ptIndexTable);
}


static inline int kr_tableid_match(void *ptr, void *key)
{
    T_KRTable *ptTable = (T_KRTable *)ptr; 
    return (*((const int*)&ptTable->iTableId) == *((const int*) key));
}

static inline int kr_indexid_match(void *ptr, void *key)
{
    T_KRIndex *ptIndex = (T_KRIndex *)ptr; 
    return (*((const int*)&ptIndex->iIndexId) == *((const int*) key));
}

static inline int kr_index_table_match(void *ptr, void *key)
{
    T_KRIndexTable *ptIndexTable1 = (T_KRIndexTable *)ptr; 
    T_KRIndexTable *ptIndexTable2 = (T_KRIndexTable *)ptr;
    T_KRIndex *ptIndex1 = ptIndexTable1->ptIndex;
    T_KRIndex *ptIndex2 = ptIndexTable2->ptIndex;
    T_KRTable *ptTable1 = ptIndexTable1->ptTable;
    T_KRTable *ptTable2 = ptIndexTable2->ptTable;
    
    if (ptIndex1->iIndexId != ptIndex2->iIndexId) {
        return (ptIndex1->iIndexId == ptIndex2->iIndexId);
    } else {
        return (ptTable1->iTableId == ptTable2->iTableId);
    }
}

T_KRDB* kr_db_create(char *psDBName, T_DbsEnv *ptDbsEnv, T_KRModule *ptModule)
{
    T_KRDB *ptDB = (T_KRDB *)kr_calloc(sizeof(T_KRDB));
    if (ptDB == NULL) {
        fprintf(stderr, "kr_calloc ptDB failed!\n");
        return NULL;
    }
    strncpy(ptDB->caDBName, psDBName, sizeof(ptDB->caDBName));
    ptDB->dbsenv = ptDbsEnv;

    ptDB->pTableList = kr_list_new();
    kr_list_set_match(ptDB->pTableList, (KRCompareFunc )kr_tableid_match);
    kr_list_set_free(ptDB->pTableList, (KRFreeFunc )kr_table_drop);

    ptDB->pIndexList = kr_list_new();
    kr_list_set_match(ptDB->pIndexList, (KRCompareFunc )kr_indexid_match);
    kr_list_set_free(ptDB->pIndexList, (KRFreeFunc )kr_index_drop);

    ptDB->pIndexTableList = kr_list_new();
    kr_list_set_match(ptDB->pIndexTableList, (KRCompareFunc )kr_index_table_match);
    kr_list_set_free(ptDB->pIndexTableList, (KRFreeFunc )kr_index_table_drop);


    return ptDB;
}


void kr_db_drop(T_KRDB *ptDB)
{
    kr_list_destroy(ptDB->pTableList);
    kr_list_destroy(ptDB->pIndexList);
    kr_list_destroy(ptDB->pIndexTableList);
    kr_free(ptDB);
}


T_KRIndexTable* kr_index_table_get(T_KRDB *ptDB, int iIndexId, int iTableId)
{
    T_KRIndex *ptIndex = kr_index_get(ptDB, iIndexId);
    if (ptIndex == NULL) {
        fprintf(stderr, "kr_index_get [%d] failed!\n", iIndexId);
        return NULL;
    }

    T_KRTable *ptTable = kr_table_get(ptDB, iTableId);
    if (ptTable == NULL) {
        fprintf(stderr, "kr_table_get [%d] failed!\n", iTableId);
        return NULL;
    }
    
    T_KRIndexTable stIndexTable = {0};
    stIndexTable.ptIndex = ptIndex;
    stIndexTable.ptTable = ptTable;
    T_KRListNode *ptListNode = kr_list_search(ptDB->pIndexTableList, &stIndexTable);
    if (ptListNode != NULL) {
        return (T_KRIndexTable *)kr_list_value(ptListNode);
    }
    return NULL;
}
