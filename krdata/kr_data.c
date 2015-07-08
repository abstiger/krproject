#include "kr_data.h"
#include "kr_data_item.h"
#include "kr_data_temp.h"


T_KRData* kr_data_construct(T_KRParam *ptParam, T_KRModule *ptModule)
{
    T_KRData *ptData = kr_malloc(sizeof(T_KRData));
    if (ptData == NULL) {
        KR_LOG(KR_LOGERROR, "kr_malloc ptData Failed!");
        return NULL;
    }
    ptData->ptParam = ptParam;
    ptData->ptModule = ptModule;

    ptData->ptCurrRec = NULL;
    ptData->ptTravRec = NULL;
    ptData->ptItemTable = kr_hashtable_new_full(
                              (KRHashFunc )kr_string_hash, \
                              (KREqualFunc )kr_string_equal,\
                              NULL, 
                             (KRDestroyNotify )kr_data_item_free);
    if (ptData->ptItemTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_hashtable_new_full item table Failed!");
        return NULL;
    }

    //load data item according parameter
    if (kr_data_item_load(ptData->ptItemTable, ptParam) != 0) {
        KR_LOG(KR_LOGERROR, "load item table error!");
        return NULL;
    }
    
    ptData->ptTempTable = kr_hashtable_new_full(
                              (KRHashFunc )kr_string_hash, \
                              (KREqualFunc )kr_string_equal,\
                              NULL, 
                             (KRDestroyNotify )kr_data_temp_free);
    if (ptData->ptTempTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_hashtable_new_full temp table Failed!");
        return NULL;
    }
    
    ptData->tConstructTime = kr_param_load_time(ptParam);
    
    return ptData;
}


void kr_data_init(T_KRData *ptData)
{
    if (ptData) {
        ptData->ptCurrRec = NULL;
        ptData->ptTravRec = NULL;
        //kr_hashtable_remove_all(ptData->ptItemTable);
        kr_hashtable_remove_all(ptData->ptTempTable);
    }
}


void kr_data_destruct(T_KRData *ptData)
{
    if (ptData) {
        kr_hashtable_destroy(ptData->ptItemTable);
        kr_hashtable_destroy(ptData->ptTempTable);
        kr_free(ptData);
    }
}


int kr_data_check(T_KRData *ptData, T_KRParam *ptParam)
{  
    /*check item table*/
    if (ptData->tConstructTime != kr_param_load_time(ptParam)) {
        KR_LOG(KR_LOGDEBUG, "reload ...[%ld][%ld]", 
                ptData->tConstructTime, kr_param_load_time(ptParam));

        kr_hashtable_remove_all(ptData->ptItemTable);
        if (kr_data_item_load(ptData->ptItemTable, ptParam) != 0) {
            KR_LOG(KR_LOGERROR, "reload item table error!");
            return -1;
        }
    }

    return 0;
}

E_KRType kr_data_get_type(char kind, int id, void *data)
{
    T_KRData *ptData = (T_KRData *)data;
    switch(kind) 
    {
        case KR_CALCKIND_CID:
            return kr_record_get_field_type(ptData->ptCurrRec, id);
            break;
        case KR_CALCKIND_FID:
            return kr_record_get_field_type(ptData->ptTravRec, id);
            break;
        case KR_CALCKIND_SET:
        case KR_CALCKIND_SID:
        case KR_CALCKIND_DID:
        case KR_CALCKIND_HID:
            return kr_data_item_type(kind, id, ptData);
            break;
        default:
            return KR_TYPE_UNKNOWN;
    }
}


void *kr_data_get_value(char kind, int id, void *data)
{
    T_KRData *ptData = (T_KRData *)data;
    switch(kind) 
    {
        case KR_CALCKIND_CID:
            return kr_record_get_field_value(ptData->ptCurrRec, id);
            break;
        case KR_CALCKIND_FID:
            return kr_record_get_field_value(ptData->ptTravRec, id);
            break;
        case KR_CALCKIND_SET:
        case KR_CALCKIND_SID:
        case KR_CALCKIND_DID:
        case KR_CALCKIND_HID:
            return kr_data_item_value(kind, id, ptData);
            break;
        default:
            return NULL;
    }
}
