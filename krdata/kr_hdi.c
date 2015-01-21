#include "kr_context.h"
#include "kr_hdi_cache.h"

extern int kr_hdi_aggr_func(T_KRHDI *krhdi, T_KRContext *krcontext);


int kr_hdi_compute(T_KRHDI *krhdi, void *param)
{
    /*initialize first*/
    kr_hdi_init(krhdi);

    T_KRShmHDIDef *ptHDIDef = krhdi->ptShmHDIDef;
    T_KRContext *ptContext = (T_KRContext *)param;
    T_KRTable *ptTable = ptContext->ptCurrRec->ptTable;
    T_KRIndex *ptIndex = \
      kr_get_table_index(ptTable, ptHDIDef->lStatisticsIndex);
    if (ptIndex == NULL) {
        KR_LOG(KR_LOGDEBUG, "table[%d] doesn't have index[%ld]!", \
               ptTable->iTableId, ptHDIDef->lStatisticsIndex);
        return 0;
    }
    ptContext->eKeyType  = \
        kr_get_field_type(ptContext->ptCurrRec, ptIndex->iIndexFieldId);
    ptContext->pKeyValue = \
        kr_get_field_value(ptContext->ptCurrRec, ptIndex->iIndexFieldId);
    ptContext->ptRecList = \
        kr_get_record_list(ptIndex, ptContext->pKeyValue);
    
    if (krhdi->HDIAggrFunc == NULL) 
        krhdi->HDIAggrFunc = (KRHDIAggrFunc )kr_hdi_aggr_func;
    if (krhdi->HDIAggrFunc(krhdi, ptContext) != 0) {
        KR_LOG(KR_LOGERROR, "Run HDI[%ld] AggrFunc failed!", krhdi->lHDIId);
        return -1;
    }
    
    return 0;
}


E_KRType kr_hdi_get_type(int hid, T_KRContext *krcontext)
{
    if (krcontext == NULL) return KR_TYPE_UNKNOWN;
        
    long lHID = (long )hid;
    T_KRHDITable *krhditable = krcontext->ptDym->hditable;
    T_KRHDI *krhdi = kr_hashtable_lookup(krhditable->ptHDITable, &lHID);
    if (krhdi == NULL) return KR_TYPE_UNKNOWN;
    
    return krhdi->eValueType;
}


static void kr_hdi_get_value_from_cache(T_KRHDI *krhdi, T_KRContext *krcontext)
{
    T_KRHDICacheValue *cache_value = \
        kr_hdi_cache_get(krcontext->ptEnv->ptHDICache, krcontext->pKeyValue, krhdi->lHDIId);
    //TODO:replace "00000000" with sharememory's batchdate
    if (cache_value->eValueInd == KR_VALUE_SETED &&
        strcmp(cache_value->caDate, "00000000") >= 0) {
        /*if cached and date valid, get it directly*/
        KR_LOG(KR_LOGDEBUG, "Get HDI [%ld] value date cached!", krhdi->lHDIId);
        switch(krhdi->eValueType)
        {
            case KR_TYPE_INT:
                krhdi->uValue.i = cache_value->uValue.i; break;
            case KR_TYPE_LONG:
                krhdi->uValue.l = cache_value->uValue.l; break;
            case KR_TYPE_DOUBLE:
                krhdi->uValue.d = cache_value->uValue.d; break;
            case KR_TYPE_STRING:
                krhdi->uValue.s = kr_strdup(cache_value->uValue.s);
                break;
        }
        krhdi->eValueInd = KR_VALUE_SETED;
    } else {
        /*if not, compute it and set the cache*/
        kr_hdi_compute(krhdi, krcontext);
        if (krhdi->eValueInd != KR_VALUE_SETED) {
            KR_LOG(KR_LOGDEBUG, "kr_hdi_compute [%ld] unset!", krhdi->lHDIId);
            return;
        }
        
        cache_value->eValueType = krhdi->eValueType;
        time_t tTransTime = kr_get_transtime(krcontext->ptCurrRec);
        kr_ttime_to_date(tTransTime, cache_value->caDate);
        switch(krhdi->eValueType)
        {
            case KR_TYPE_INT:
                cache_value->uValue.i = krhdi->uValue.i; break;
            case KR_TYPE_LONG:
                cache_value->uValue.l = krhdi->uValue.l; break;
            case KR_TYPE_DOUBLE:
                cache_value->uValue.d = krhdi->uValue.d; break;
            case KR_TYPE_STRING:
                cache_value->uValue.s = kr_strdup(krhdi->uValue.s);
                break;
        }
        cache_value->eValueInd = KR_VALUE_SETED;
    }
}


void *kr_hdi_get_value(int hid, T_KRContext *krcontext)
{
    if (krcontext == NULL) return NULL;
    
    long lHID = (long )hid;
    T_KRHDITable *krhditable = krcontext->ptDym->hditable;
    T_KRHDI *krhdi = kr_hashtable_lookup(krhditable->ptHDITable, &lHID);
    if (krhdi == NULL) return NULL;
    
    if (krhdi->ptCurrRec != krcontext->ptCurrRec) {
        /*get the value from cache*/
        if (krcontext->ptEnv->ptHDICache != NULL) {
            kr_hdi_get_value_from_cache(krhdi, krcontext);
        } else if (kr_hdi_compute(krhdi, krcontext) != 0) {
            KR_LOG(KR_LOGDEBUG, "kr_hdi_compute [%d] unset!", hid);
            return NULL;
        }
        krhdi->ptCurrRec = krcontext->ptCurrRec;
    } else {
        KR_LOG(KR_LOGDEBUG, "Get HDI [%d] value record cached!", hid);
    }
    
    if (krhdi->eValueInd == KR_VALUE_SETED) {
        return kr_get_value(&krhdi->uValue, krhdi->eValueType);
    }
    return NULL;
}
