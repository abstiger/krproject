#include "kr_db.h"

T_KRDB* kr_db_new(char *psDBName, T_DbsEnv *ptDbsEnv, T_KRModule *ptModule)
{
    T_KRDB *ptDB = kr_db_create(psDBName, ptDbsEnv, ptModule);
    if (ptDB == NULL) {
        KR_LOG(KR_LOGERROR, "kr_db_create Error!");
        return NULL;
    }

    /*then create tables*/
    if (_kr_db_build_table(ptDB) != 0) {
        KR_LOG(KR_LOGERROR, "_kr_db_build_table Error!");
        return NULL;
    }

    /*then create indexes*/
    if (_kr_db_build_index(ptDB) != 0) {
        KR_LOG(KR_LOGERROR, "_kr_db_build_index Error!");
        return NULL;
    }
    
    /*finally create index table relations */
    if (_kr_db_build_index_table(ptDB) != 0) {
        KR_LOG(KR_LOGERROR, "_kr_db_build_index_table Error!");
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



T_KRList* kr_db_select(T_KRDB *ptDB, int iIndexId, void *key, time_t tBeginTime, time_t tEndTime, int iSortFieldId)
{
    /*alloc return record list*/
    T_KRList *pRecList=kr_list_new();
    kr_list_set_compare(pRecList, (KRCompareDataFunc)kr_record_compare);
    
    /*get index first*/
    T_KRIndex *ptIndex = kr_index_get(ptDB, iIndexId);
    if (ptIndex == NULL) {
        KR_LOG(KR_LOGERROR, "kr_index_get[%d] Error!", iIndexId);
        return NULL;
    }
    
    /*get records from internal*/
    T_KRIndexSolt *ptIndexSlot = kr_hashtable_lookup(ptIndex->pHashTable, key);

    /*get records from external*/
    
    return pRecList;
}




