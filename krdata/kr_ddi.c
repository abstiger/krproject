#include "kr_context.h"

extern int kr_ddi_aggr_func(T_KRDDI *krddi, T_KRContext *krcontext);

int kr_ddi_compute(T_KRDDI *krddi, void *param)
{
    /*initialize first*/
    krddi->eValueInd == KR_VALUE_UNSET;
    kr_hashtable_remove_all(krddi->ptRelated);

    /*string comes from kr_strdup, need kr_free*/
    if (krddi->eValueType == KR_TYPE_STRING)
        kr_free(krddi->uValue.s);
    memset(&krddi->uValue, 0x00, sizeof(krddi->uValue));
    
    T_KRShmDDIDef *ptDDIDef = krddi->ptShmDDIDef;
    T_KRContext *ptContext = (T_KRContext *)param;
    T_KRTable *ptTable = ptContext->ptCurrRec->ptTable;
    T_KRIndex *ptIndex = \
      kr_get_table_index(ptTable, ptDDIDef->lStatisticsIndex);
    if (ptIndex == NULL) {
        KR_LOG(KR_LOGERROR, "Current table[%d] doesn't have index[%ld]!", \
               ptTable->iTableId, ptDDIDef->lStatisticsIndex);
        return 0;
    }
    ptContext->eKeyType  = \
        kr_get_field_type(ptContext->ptCurrRec, ptIndex->iIndexFieldId);
    ptContext->pKeyValue = \
        kr_get_field_value(ptContext->ptCurrRec, ptIndex->iIndexFieldId);
    ptContext->ptRecList = \
        kr_get_record_list(ptIndex, ptContext->pKeyValue);

    krddi->DDIAggrFunc=krddi->DDIAggrFunc?krddi->DDIAggrFunc:kr_ddi_aggr_func;
    if (krddi->DDIAggrFunc(krddi, ptContext) != 0) {
        KR_LOG(KR_LOGERROR, "Run DDI[%ld] AggrFunc failed!", krddi->lDDIId);
        return -1;
    }
    
    return 0;
}


E_KRType kr_ddi_get_type(int did, T_KRContext *krcontext)
{
    if (krcontext == NULL) return KR_TYPE_UNKNOWN;
        
    long lDID = (long )did;
    T_KRDDITable *krdditable = krcontext->ptDym->dditable;
    T_KRDDI *krddi = kr_hashtable_lookup(krdditable->ptDDITable, &lDID);
    if (krddi == NULL) return KR_TYPE_UNKNOWN;
    
    return krddi->eValueType;
}

void *kr_ddi_get_value(int did, T_KRContext *krcontext)
{
    if (krcontext == NULL) return NULL;
        
    long lDID = (long )did;
    T_KRDDITable *krdditable = krcontext->ptDym->dditable;
    T_KRDDI *krddi = kr_hashtable_lookup(krdditable->ptDDITable, &lDID);
    if (krddi == NULL) return NULL;
    
    if (krddi->ptCurrRec != krcontext->ptCurrRec) {
        kr_ddi_compute(krddi, krcontext);
        if (krddi->eValueInd != KR_VALUE_SETED) {
            KR_LOG(KR_LOGERROR, "kr_ddi_compute [%d] failed!", did);
            return NULL;
        }
        krddi->ptCurrRec = krcontext->ptCurrRec;
    } else {
        KR_LOG(KR_LOGDEBUG, "Get DDI [%d] value record cached!", did);
    }
    
    if (krddi->eValueInd == KR_VALUE_SETED) {
        return kr_get_value(&krddi->uValue, krddi->eValueType);
    }
    return NULL;
}
