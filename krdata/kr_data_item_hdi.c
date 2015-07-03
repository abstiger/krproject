#include "kr_data_item_hdi.h"
#include "kr_data_item_hdi_cache.h"
#include "krparam/kr_param_class_hdi.h"


extern int kr_hdi_aggr_day(T_KRHdi *ptHdi, T_KRData *ptData, char *object);
extern int kr_hdi_aggr_mon(T_KRHdi *ptHdi, T_KRData *ptData, char *object);
extern int kr_hdi_aggr_flag(T_KRHdi *ptHdi, T_KRData *ptData, char *object);

void *kr_data_item_hdi_new(T_KRDataItem *ptDataItem)
{
    T_KRParamHdi *ptParamHdi = (T_KRParamHdi *)ptDataItem->ptDataItemDef;

    T_KRHdi *ptHdi = kr_calloc(sizeof(*ptHdi));
    if (ptHdi == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptHdi failed!");
        return NULL;
    }

    return ptHdi;
}


void kr_data_item_hdi_free(void *priv) 
{
    T_KRHdi *ptHdi = (T_KRHdi *)priv;
    if (ptHdi != NULL) {
        kr_free(ptHdi);
    }
}


static int kr_data_item_hdi_compute(T_KRDataItem *ptDataItem, T_KRData *ptData)
{
    T_KRParamHdi *ptParamHdi = (T_KRParamHdi *)ptDataItem->ptDataItemDef;
    T_KRHdi *ptHdi = (T_KRHdi *)ptDataItem->ptPrivate;

    int nRet = 0;
    char caDataObject[KR_HDI_OBJECT_LEN+1];

    switch(ptHdi->eKeyType)
    {
        case KR_TYPE_INT:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%d", *(int *)ptHdi->pKeyValue);
            break;         
        case KR_TYPE_LONG:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%ld", *(long *)ptHdi->pKeyValue);
            break; 
        case KR_TYPE_DOUBLE:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%lf", *(double *)ptHdi->pKeyValue);
            break; 
        case KR_TYPE_STRING:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%s", (char *)ptHdi->pKeyValue);
            break; 
        default:
            KR_LOG(KR_LOGERROR, "Unsupported object type[%c]!", \
                   ptHdi->eKeyType);
            return -1;
    }

    switch(ptParamHdi->caStatisticsType[0])
    {
        case KR_HDI_STATISTICS_DAY:
            nRet = kr_hdi_aggr_day(ptHdi, ptData, caDataObject);
            if (nRet != 0) {
                KR_LOG(KR_LOGERROR, "kr_hdi_aggr_day Error!");
            }
            break;
        case KR_HDI_STATISTICS_MONTH:
            nRet = kr_hdi_aggr_mon(ptHdi, ptData, caDataObject);
            if (nRet != 0) {
                KR_LOG(KR_LOGERROR, "kr_hdi_aggr_mon Error!");
            }
            break;
        case KR_HDI_STATISTICS_FLAG:
            nRet = kr_hdi_aggr_flag(ptHdi, ptData, caDataObject);
            if (nRet != 0) {
                KR_LOG(KR_LOGERROR, "kr_hdi_aggr_flag Error!");
            }
            break;
        default:
            KR_LOG(KR_LOGERROR, "Unsupported statistics type[%s]!", \
                   ptParamHdi->caStatisticsType);
            return -1;        
    }
    
    ptDataItem->eValueInd = KR_VALUE_SETED;
    
    return 0;
}


int kr_data_item_hdi_aggr(T_KRDataItem *ptDataItem, T_KRData *ptData)
{
    T_KRParamHdi *ptParamHdi = (T_KRParamHdi *)ptDataItem->ptDataItemDef;
    T_KRHdi *ptHdi = (T_KRHdi *)ptDataItem->ptPrivate;

    //FIXME:create hdi cache
    gptHdiCache = kr_hdi_cache_create(1000);
    if (gptHdiCache == NULL) {
        KR_LOG(KR_LOGERROR, "kr_hdi_cache_create [%d] failed!");
        return -1;
    }

    //get it from lru cache
    T_KRHdiCacheValue *ptHdiCacheValue = \
        kr_hdi_cache_get(gptHdiCache, ptHdi->pKeyValue, ptDataItem->lDataItemId);
    //TODO:replace "00000000" with sharememory's batchdate
    if (ptHdiCacheValue->eValueInd == KR_VALUE_SETED &&
        strcmp(ptHdiCacheValue->caDate, "00000000") >= 0) {
        /*if cached and date valid, get it directly*/
        KR_LOG(KR_LOGDEBUG, "Get Hdi [%ld] value date cached!", ptDataItem->lDataItemId);
        switch(ptDataItem->eValueType)
        {
            case KR_TYPE_INT:
                ptDataItem->uValue.i = ptHdiCacheValue->uValue.i; break;
            case KR_TYPE_LONG:
                ptDataItem->uValue.l = ptHdiCacheValue->uValue.l; break;
            case KR_TYPE_DOUBLE:
                ptDataItem->uValue.d = ptHdiCacheValue->uValue.d; break;
            case KR_TYPE_STRING:
                ptDataItem->uValue.s = kr_strdup(ptHdiCacheValue->uValue.s);
                break;
        }
    } else {
        /*if not, compute it and set the cache*/
        if (kr_data_item_hdi_compute(ptDataItem, ptData) != 0) {
            KR_LOG(KR_LOGDEBUG, "kr_data_item_hdi_compute [%ld] failed!", ptDataItem->lDataItemId);
            return -1;
        }
        
        ptHdiCacheValue->eValueType = ptDataItem->eValueType;
        time_t tTransTime = kr_get_transtime(ptData->ptCurrRec);
        kr_ttime_to_date(tTransTime, ptHdiCacheValue->caDate);
        switch(ptDataItem->eValueType)
        {
            case KR_TYPE_INT:
                ptHdiCacheValue->uValue.i = ptDataItem->uValue.i; break;
            case KR_TYPE_LONG:
                ptHdiCacheValue->uValue.l = ptDataItem->uValue.l; break;
            case KR_TYPE_DOUBLE:
                ptHdiCacheValue->uValue.d = ptDataItem->uValue.d; break;
            case KR_TYPE_STRING:
                ptHdiCacheValue->uValue.s = kr_strdup(ptDataItem->uValue.s);
                break;
        }
        ptHdiCacheValue->eValueInd = KR_VALUE_SETED;
    }
    return 0;
}


static int _kr_data_item_hdi_load(char *psParamClassName, char *psParamObjectKey, 
        char *psParamObjectString, void *ptParamObject, void *data)
{
    T_KRParamHdi *ptParamHdi = (T_KRParamHdi *)ptParamObject;
    T_KRHashTable *ptItemTable = (T_KRHashTable *)data;

    T_KRDataItem *ptDataItem = kr_data_item_new(
            ptParamHdi,
            ptParamHdi->caHdiName[0], //FIXME: 
            ptParamHdi->lHdiId, 
            kr_data_item_hdi_new,
            kr_data_item_hdi_aggr,
            kr_data_item_hdi_free);
    if (ptDataItem == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_item_create [%ld] failed!", \
                ptParamHdi->lHdiId);
        return -1; 
    }

    //insert into item table
    kr_hashtable_replace(ptItemTable, ptDataItem->caDataItemId, ptDataItem);

    return 0;
}


int kr_data_item_hdi_load(T_KRHashTable *ptItemTable, T_KRParam *ptParam)
{
    //load hdi
    if (kr_param_object_foreach(ptParam, KR_PARAM_HDI, 
                _kr_data_item_hdi_load, ptItemTable) != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_item_hdi_load Error!");
        return -1;
    }
    
    return 0;   
}
