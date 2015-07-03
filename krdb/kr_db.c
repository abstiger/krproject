#include "kr_db.h"
#include "kr_db_kernal.h"
#include "kr_db_persist.h"

#include "krparam/kr_param_class_input.h"
#include "krparam/kr_param_class_index.h"

static int _kr_db_build_table(char *psParamClassName, char *psParamObjectKey, char *psParamObjectString, void *ptParamObject, void *data)
{
    T_KRParamInput *ptParamInput = (T_KRParamInput *)ptParamObject;
    T_KRDB *ptDB = (T_KRDB *)data;

    T_KRTable *ptTable = kr_table_create(ptDB,
                ptParamInput->lInputId, 
                ptParamInput->caInputName, 
                ptParamInput->caKeepMode[0], 
                ptParamInput->lKeepValue,
                ptParamInput->lFieldCnt,
                ptParamInput->ptFieldDef);
    if (ptTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_table_create [%ld] failed!", \
                ptParamInput->lInputId);
        return -1; 
    }
    
    for (int i=0; i<ptParamInput->lFieldCnt; ++i) {
        T_KRParamInputField *ptParamInputField = &ptParamInput->ptFieldDef[i];
        /*TODO:load field define
        ptFieldDef->offset = ulFieldOffset;
        ulFieldOffset += ptFieldDef->length;
        // String terminated with '\0'
        if (ptFieldDef[iCnt].type == KR_TYPE_STRING) {
            ulFieldOffset += 1;
        }
        */
    }
    return 0;
}

static int _kr_db_build_index(char *psParamClassName, char *psParamObjectKey, char *psParamObjectString, void *ptParamObject, void *data)
{
    T_KRParamIndex *ptParamIndex = (T_KRParamIndex *)ptParamObject;
    T_KRDB *ptDB = (T_KRDB *)data;
    
    T_KRIndex *ptIndex = kr_index_create(ptDB,
                ptParamIndex->lIndexId, \
                ptParamIndex->caIndexName, \
                ptParamIndex->caIndexFieldType[0]);
    if (ptIndex == NULL) {
        KR_LOG(KR_LOGERROR, "kr_index_create [%ld] failed!", \
                ptParamIndex->lIndexId);
        return -1; 
    }
    
    for (int i=0; i<ptParamIndex->lTableCnt; ++i) {
        T_KRParamIndexTable *ptParamIndexTable = &ptParamIndex->ptTableDef[i];
        T_KRIndexTable *ptIndexTable = kr_index_table_create(ptDB,
                ptParamIndex->lIndexId, \
                ptParamIndexTable->lTableId, \
                ptParamIndexTable->lIndexFieldId, \
                ptParamIndexTable->lSortFieldId);
        if (ptIndexTable == NULL) {
            KR_LOG(KR_LOGERROR, "kr_index_table_create [%ld][%ld] failed!", \
                    ptParamIndex->lIndexId, ptParamIndexTable->lTableId);
            return -1; 
        }
    }
    
    return 0;
}

T_KRDB* kr_db_new(char *name, T_KRParam *ptParam)
{
    T_KRDB *ptDB = kr_db_create(name, ptParam);
    if (ptDB == NULL) {
        KR_LOG(KR_LOGERROR, "kr_db_create %s error!", name);
        return NULL;
    }

    /*fist create tables*/
    if (kr_param_object_foreach(ptParam, KR_PARAM_INPUT, 
            _kr_db_build_table, ptDB) != 0) {
        KR_LOG(KR_LOGERROR, "_kr_db_build_table Error!");
        return NULL;
    }

    /*then create indexes*/
    if (kr_param_object_foreach(ptParam, KR_PARAM_INDEX, 
            _kr_db_build_index, ptDB) != 0) {
        KR_LOG(KR_LOGERROR, "_kr_db_build_index Error!");
        return NULL;
    }
    
    return ptDB;
}


void kr_db_free(T_KRDB *ptDB)
{
    kr_db_drop(ptDB);
}


int kr_db_insert(T_KRDB *ptDB, T_KRRecord *ptRecord)
{
    /*insert into memory, internal*/
    kr_record_insert(ptRecord);
    
    /*TODO:persist to database, external*/
    
    return 0;
}



T_KRList* kr_db_select(T_KRDB *ptDB, int iIndexId, void *key, 
    time_t tBeginTime, time_t tEndTime, int iSortFieldId)
{
    /*get index first*/
    T_KRIndex *ptIndex = kr_index_get(ptDB, iIndexId);
    if (ptIndex == NULL) {
        KR_LOG(KR_LOGERROR, "kr_index_get [%d] error!", iIndexId);
        return NULL;
    }
    
    /*get records from internal*/
    T_KRIndexSolt *ptIndexSlot = kr_hashtable_lookup(ptIndex->pHashTable, key);

    /*get records from external*/

    /*alloc return record list*/
    T_KRList *pRecList=kr_list_new();
    kr_list_set_compare(pRecList, (KRCompareDataFunc)kr_record_compare);
        
    return pRecList;
}

T_KRList* kr_db_select_by_record(T_KRRecord *ptRecord, int iIndexId,
    time_t tBeginTime, time_t tEndTime, int iSortFieldId)
{
    T_KRTable *ptTable = ptRecord->ptTable;
    T_KRDB *ptDB = ptTable->ptDB;
    
    T_KRIndexTable *ptIndexTable = \
        kr_index_table_get(ptDB, iIndexId, ptTable->iTableId);
    if (ptIndexTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_index_table_get [%d][%d] error!", \
               iIndexId, ptTable->iTableId);
        return NULL;
    }
    //E_KRType eKeyType = kr_field_get_type(ptRecord, ptIndexTable->iIndexFieldId);
    void *key = kr_field_get_value(ptRecord, ptIndexTable->iIndexFieldId);
    
    /*get records from internal*/
    T_KRIndexSolt *ptIndexSlot = \
        kr_hashtable_lookup(ptIndexTable->ptIndex->pHashTable, key);
    //ptIndexSlot->pRecList

    /*get records from external*/

    /*alloc return record list*/
    T_KRList *pRecList=kr_list_new();
    kr_list_set_compare(pRecList, (KRCompareDataFunc)kr_record_compare);
    
    return pRecList;
}


int kr_record_get_table_id(T_KRRecord *ptRecord)
{
    return ptRecord->ptTable->iTableId;
}


