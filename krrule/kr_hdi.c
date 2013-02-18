#include "kr_hdi.h"
#include "kr_hdi_cache.h"

/*history dataitem*/
T_KRHDI *kr_hdi_construct(T_KRShmHDIDef *hdi_def, T_KRModule *datamodule)
{
    T_KRHDI *krhdi = kr_calloc(sizeof(T_KRHDI));
    if (krhdi == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krhdi failed!");
        return NULL;
    }
    krhdi->ptShmHDIDef = hdi_def;
    krhdi->lHDIId = hdi_def->lHdiId;
    krhdi->eValueType = hdi_def->caHdiValueType[0];
    krhdi->HDIAggrFunc = (KRHDIAggrFunc )kr_hdi_aggr_func;
    /*get the retrieve data function from module*/
    if (hdi_def->caHdiAggrFunc[0] != '\0') {
        krhdi->HDIAggrFunc = (KRHDIAggrFunc )kr_module_symbol(datamodule,
                hdi_def->caHdiAggrFunc);
        if (krhdi->HDIAggrFunc == NULL) {
            KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", \
                    hdi_def->caHdiAggrFunc);
            return NULL;
        }
    }
    krhdi->ptCurrRec = NULL;
    
    return krhdi;
}

int kr_hdi_compute(T_KRHDI *krhdi, void *param)
{
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
    
    if (krhdi->HDIAggrFunc(krhdi, ptContext) != 0) {
        KR_LOG(KR_LOGERROR, "Run HDI[%ld] AggrFunc failed!", krhdi->lHDIId);
        return -1;
    }
    
    return 0;
}


void kr_hdi_destruct(T_KRHDI *krhdi)
{
    kr_free(krhdi);
}


T_KRHDITable *kr_hdi_table_construct(T_KRShmHDI *shm_hdi, T_KRModule *datamodule)
{
    T_KRHDITable *krhditable = kr_calloc(sizeof(T_KRHDITable));
    if (krhditable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krhditable failed!");
        return NULL;
    }
    krhditable->ptShmHDIs = shm_hdi;
    krhditable->lHDICnt = shm_hdi->lHDIDefCnt;
    krhditable->ptHDITable = \
      kr_hashtable_new_full(kr_long_hash, kr_long_equal, \
                            NULL, (KRDestroyNotify )kr_hdi_destruct);
    
    int i = 0;
    T_KRHDI *krhdi = NULL;
    for (i=0; i<shm_hdi->lHDIDefCnt; ++i) {
        krhdi = kr_hdi_construct(&shm_hdi->stShmHDIDef[i], datamodule);
        if (krhdi == NULL) {
            KR_LOG(KR_LOGERROR, "kr_hdi_construct [%d] failed!", i);
            kr_hashtable_destroy(krhditable->ptHDITable);
            kr_free(krhditable);
            return NULL;
        }
        kr_hashtable_insert(krhditable->ptHDITable, &krhdi->lHDIId, krhdi);
    }
    krhditable->tConstructTime = shm_hdi->tLastLoadTime;
    
    return krhditable;    
}


void kr_hdi_table_destruct(T_KRHDITable *krhditable)
{
    kr_hashtable_destroy(krhditable->ptHDITable);
    kr_free(krhditable);
}


E_KRFieldType kr_hdi_get_type(int hid, T_KRContext *krcontext)
{
    if (krcontext == NULL) return KR_FIELDTYPE_UNKNOWN;
        
    long lHID = (long )hid;
    T_KRHDITable *krhditable = krcontext->ptDym->hditable;
    T_KRHDI *krhdi = kr_hashtable_lookup(krhditable->ptHDITable, &lHID);
    if (krhdi == NULL) return KR_FIELDTYPE_UNKNOWN;
    
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
            case KR_FIELDTYPE_INT:
                krhdi->eValue.i = cache_value->uValue.i; break;
            case KR_FIELDTYPE_LONG:
                krhdi->eValue.l = cache_value->uValue.l; break;
            case KR_FIELDTYPE_DOUBLE:
                krhdi->eValue.d = cache_value->uValue.d; break;
            case KR_FIELDTYPE_STRING:
                strcpy(krhdi->eValue.s, cache_value->uValue.s);
                break;
        }
        krhdi->eValueInd = KR_VALUE_SETED;
    } else {
        /*if not, compute it and set the cache*/
        kr_hdi_compute(krhdi, krcontext);
        if (krhdi->eValueInd != KR_VALUE_SETED) {
            KR_LOG(KR_LOGERROR, "kr_hdi_compute failed!");
            return;
        }
        
        cache_value->eValueType = krhdi->eValueType;
        time_t tTransTime = kr_get_transtime(krcontext->ptCurrRec);
        kr_ttime_to_date(tTransTime, cache_value->caDate);
        switch(krhdi->eValueType)
        {
            case KR_FIELDTYPE_INT:
                cache_value->uValue.i = krhdi->eValue.i; break;
            case KR_FIELDTYPE_LONG:
                cache_value->uValue.l = krhdi->eValue.l; break;
            case KR_FIELDTYPE_DOUBLE:
                cache_value->uValue.d = krhdi->eValue.d; break;
            case KR_FIELDTYPE_STRING:
                cache_value->uValue.s = kr_strdup(krhdi->eValue.s);
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
            KR_LOG(KR_LOGERROR, "kr_hdi_compute [%d] failed!", hid);
            return NULL;
        }
        krhdi->ptCurrRec = krcontext->ptCurrRec;
    } else {
        KR_LOG(KR_LOGDEBUG, "Get HDI [%d] value record cached!", hid);
    }
    
    if (krhdi->eValueInd == KR_VALUE_SETED) {
        switch(krhdi->eValueType)
        {
            case KR_FIELDTYPE_INT:
                return &krhdi->eValue.i;
            case KR_FIELDTYPE_LONG:
                return &krhdi->eValue.l;
            case KR_FIELDTYPE_DOUBLE:
                return &krhdi->eValue.d;
            case KR_FIELDTYPE_STRING:
                return krhdi->eValue.s;
        }
    }
    return NULL;
}
