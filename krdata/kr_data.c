#include "kr_data.h"
#include "kr_data_item.h"
#include "kr_data_item_set.h"
#include "kr_data_item_sdi.h"
#include "kr_data_item_ddi.h"
#include "kr_data_item_hdi.h"


static int _kr_data_item_set_load(char *psParamClassName, 
        char *psParamObjectKey, char *psParamObjectString, 
        void *ptParamObject, void *data)
{
    T_KRParamSet *ptParamSet = (T_KRParamSet *)ptParamObject;
    T_KRData *ptData = (T_KRData *)data;

    T_KRDataItem *ptDataItem = kr_data_item_new(
            ptParamSet,
            ptParamSet->caSetName[0], //FIXME
            ptParamSet->lSetId, 
            ptData->pfGetType,
            ptData->pfGetValue,
            kr_data_item_set_new,
            kr_data_item_set_aggr,
            kr_data_item_set_free);
    if (ptDataItem == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_item_create [%ld] failed!", \
                ptParamSet->lSetId);
        return -1; 
    }

    //insert into item table
    kr_hashtable_replace(ptData->ptItemTable, ptDataItem->caDataItemId, ptDataItem);

    return 0;
}


static int _kr_data_item_sdi_load(char *psParamClassName, 
        char *psParamObjectKey, char *psParamObjectString, 
        void *ptParamObject, void *data)
{
    T_KRParamSdi *ptParamSdi = (T_KRParamSdi *)ptParamObject;
    T_KRData *ptData = (T_KRData *)data;

    T_KRDataItem *ptDataItem = kr_data_item_new(
            ptParamSdi,
            ptParamSdi->caSdiName[0], //FIXME
            ptParamSdi->lSdiId, 
            ptData->pfGetType,
            ptData->pfGetValue,
            kr_data_item_sdi_new,
            kr_data_item_sdi_aggr,
            kr_data_item_sdi_free);
    if (ptDataItem == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_item_create [%ld] failed!", \
                ptParamSdi->lSdiId);
        return -1; 
    }

    //insert into item table
    kr_hashtable_replace(ptData->ptItemTable, ptDataItem->caDataItemId, ptDataItem);

    return 0;
}


static int _kr_data_item_ddi_load(char *psParamClassName, 
        char *psParamObjectKey, char *psParamObjectString, 
        void *ptParamObject, void *data)
{
    T_KRParamDdi *ptParamDdi = (T_KRParamDdi *)ptParamObject;
    T_KRData *ptData = (T_KRData *)data;

    T_KRDataItem *ptDataItem = kr_data_item_new(
            ptParamDdi,
            ptParamDdi->caDdiName[0], //FIXME
            ptParamDdi->lDdiId, 
            ptData->pfGetType,
            ptData->pfGetValue,
            kr_data_item_ddi_new,
            kr_data_item_ddi_aggr,
            kr_data_item_ddi_free);
    if (ptDataItem == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_item_create [%ld] failed!", \
                ptParamDdi->lDdiId);
        return -1; 
    }

    //insert into item table
    kr_hashtable_replace(ptData->ptItemTable, ptDataItem->caDataItemId, ptDataItem);

    return 0;
}


static int _kr_data_item_hdi_load(char *psParamClassName, 
        char *psParamObjectKey, char *psParamObjectString, 
        void *ptParamObject, void *data)
{
    T_KRParamHdi *ptParamHdi = (T_KRParamHdi *)ptParamObject;
    T_KRData *ptData = (T_KRData *)data;

    T_KRDataItem *ptDataItem = kr_data_item_new(
            ptParamHdi,
            ptParamHdi->caHdiName[0], //FIXME: 
            ptParamHdi->lHdiId, 
            ptData->pfGetType,
            ptData->pfGetValue,
            kr_data_item_hdi_new,
            kr_data_item_hdi_aggr,
            kr_data_item_hdi_free);
    if (ptDataItem == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_item_create [%ld] failed!", \
                ptParamHdi->lHdiId);
        return -1; 
    }

    //insert into item table
    kr_hashtable_replace(ptData->ptItemTable, ptDataItem->caDataItemId, ptDataItem);

    return 0;
}


static int kr_data_item_load(T_KRData *ptData, T_KRParam *ptParam)
{
    //load set
    if (kr_param_object_foreach(ptParam, KR_PARAM_SET, 
                _kr_data_item_set_load, ptData) != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_item_set_load Error!");
        return -1;
    }

    //load sdi
    if (kr_param_object_foreach(ptParam, KR_PARAM_SDI, 
                _kr_data_item_sdi_load, ptData) != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_item_sdi_load Error!");
        return -1;
    }

    //load ddi
    if (kr_param_object_foreach(ptParam, KR_PARAM_DDI, 
                _kr_data_item_ddi_load, ptData) != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_item_ddi_load Error!");
        return -1;
    }

    //load hdi
    if (kr_param_object_foreach(ptParam, KR_PARAM_HDI, 
                _kr_data_item_hdi_load, ptData) != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_item_hdi_load Error!");
        return -1;
    }

    return 0;
}


    
T_KRData* kr_data_construct(T_KRParam *ptParam, T_KRModule *ptModule,
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue)
{
    T_KRData *ptData = kr_malloc(sizeof(T_KRData));
    if (ptData == NULL) {
        KR_LOG(KR_LOGERROR, "kr_malloc ptData Failed!");
        return NULL;
    }
    ptData->ptModule = ptModule;
    ptData->pfGetType = pfGetType;
    ptData->pfGetValue = pfGetValue;

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
    if (kr_data_item_load(ptData, ptParam) != 0) {
        KR_LOG(KR_LOGERROR, "load item table error!");
        return NULL;
    }
    
    ptData->tConstructTime = kr_param_load_time(ptParam);
    
    return ptData;
}


void kr_data_init(T_KRData *ptData)
{
    if (ptData) {
        //kr_hashtable_remove_all(ptData->ptItemTable);
    }
}


void kr_data_destruct(T_KRData *ptData)
{
    if (ptData) {
        kr_hashtable_destroy(ptData->ptItemTable);
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
        if (kr_data_item_load(ptData, ptParam) != 0) {
            KR_LOG(KR_LOGERROR, "reload item table error!");
            return -1;
        }
    }

    return 0;
}


static T_KRDataItem *kr_data_item_get(T_KRData *ptData, char kind, int id, T_KRContext *ptContext)
{
    char caDataItemId[20+1] = {0};
    snprintf(caDataItemId, sizeof(caDataItemId), "%c_%d", kind, id);

    T_KRDataItem *ptDataItem = kr_hashtable_lookup(ptData->ptItemTable, caDataItemId);
    if (ptDataItem == NULL) {
        KR_LOG(KR_LOGERROR, "kr_hashtable_lookup [%c]:[%d] failed!", kind, id);
        return NULL;
    }

    //check whether computed yet, that is record level cache
    T_KRRecord *ptCurrRec = kr_context_get_data(ptContext, "curr_rec");
    if (ptDataItem->ptCurrRec != ptCurrRec) {
        ptDataItem->ptCurrRec = ptCurrRec;
        if (ptDataItem->pfDataItemAggr != NULL) {
            if (ptDataItem->pfDataItemAggr(ptDataItem, ptContext) != 0) {
                KR_LOG(KR_LOGERROR, "pfDataItemAggr [%c]:[%d] failed!", 
                        kind, id);
                return NULL;
            }
        }
    } else {
        KR_LOG(KR_LOGDEBUG, "Get [%c]:[%d] record level cached!", kind, id);
    }
    
    return ptDataItem;
}


E_KRType kr_data_get_type(T_KRData *ptData, char kind, int id, T_KRContext *ptContext)
{
    T_KRDataItem *ptDataItem = kr_data_item_get(ptData, kind, id, ptContext);
    if (ptDataItem == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_item_get [%c]:[%d] failed!", kind, id);
        return KR_TYPE_UNKNOWN;
    }
    
    if (ptDataItem->eValueInd != KR_VALUE_SETED) {
        KR_LOG(KR_LOGERROR, "[%c]:[%d] value unset!", kind, id);
        return KR_TYPE_UNKNOWN;
    }

    return ptDataItem->eValueType;
}


void *kr_data_get_value(T_KRData *ptData, char kind, int id, T_KRContext *ptContext)
{
    T_KRDataItem *ptDataItem = kr_data_item_get(ptData, kind, id, ptContext);
    if (ptDataItem == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_item_get [%c]:[%d] failed!", kind, id);
        return NULL;
    }
    
    if (ptDataItem->eValueInd != KR_VALUE_SETED) {
        KR_LOG(KR_LOGERROR, "[%c]:[%d] value unset!", kind, id);
        return NULL;
    }

    return kr_get_value(&ptDataItem->uValue, ptDataItem->eValueType);
}
