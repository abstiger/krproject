#include "kr_context.h"

extern int kr_sdi_aggr_func(T_KRSDI *krsdi, T_KRContext *krcontext);


int kr_sdi_compute(T_KRSDI *krsdi, void *param)
{
    /*initialize first*/
    kr_sdi_init(krsdi);
    
    T_KRShmSDIDef *ptSDIDef = krsdi->ptShmSDIDef;
    T_KRContext *ptContext = (T_KRContext *)param;
    T_KRTable *ptTable = ptContext->ptCurrRec->ptTable;
    T_KRIndex *ptIndex = \
      kr_get_table_index(ptTable, ptSDIDef->lStatisticsIndex);
    if (ptIndex == NULL) {
        KR_LOG(KR_LOGERROR, "Current table[%d] doesn't have index[%ld]!", \
               ptTable->iTableId, ptSDIDef->lStatisticsIndex);
        return 0;
    }
    ptContext->eKeyType  = \
        kr_get_field_type(ptContext->ptCurrRec, ptIndex->iIndexFieldId);
    ptContext->pKeyValue = \
        kr_get_field_value(ptContext->ptCurrRec, ptIndex->iIndexFieldId);
    ptContext->ptRecList = \
        kr_get_record_list(ptIndex, ptContext->pKeyValue);

    if (krsdi->SDIAggrFunc == NULL) 
        krsdi->SDIAggrFunc = (KRSDIAggrFunc )kr_sdi_aggr_func;
    if (krsdi->SDIAggrFunc(krsdi, ptContext) != 0) {
        KR_LOG(KR_LOGERROR, "Run SDI[%ld] AggrFunc failed!", krsdi->lSDIId);
        return -1;
    }
    
    return 0;
}


E_KRType kr_sdi_get_type(int sid, T_KRContext *krcontext)
{
    if (krcontext == NULL) return KR_TYPE_UNKNOWN;
        
    long lSID = (long )sid;
    T_KRSDITable *krsditable = krcontext->ptDym->sditable;
    T_KRSDI *krsdi = kr_hashtable_lookup(krsditable->ptSDITable, &lSID);
    if (krsdi == NULL) return KR_TYPE_UNKNOWN;
    
    return krsdi->eValueType;
}

void *kr_sdi_get_value(int sid, T_KRContext *krcontext)
{
    if (krcontext == NULL) return NULL;
        
    long lSID = (long )sid;
    T_KRSDITable *krsditable = krcontext->ptDym->sditable;
    T_KRSDI *krsdi = kr_hashtable_lookup(krsditable->ptSDITable, &lSID);
    if (krsdi == NULL) return NULL;
    
    if (krsdi->ptCurrRec != krcontext->ptCurrRec) {
        kr_sdi_compute(krsdi, krcontext);
        if (krsdi->eValueInd != KR_VALUE_SETED) {
            KR_LOG(KR_LOGDEBUG, "kr_sdi_compute [%d] unset!", sid);
            return NULL;
        }
        krsdi->ptCurrRec = krcontext->ptCurrRec;
    } else {
        KR_LOG(KR_LOGDEBUG, "Get SDI [%d] value record cached!", sid);
    }
    
    if (krsdi->eValueInd == KR_VALUE_SETED) {
        return kr_get_value(&krsdi->uValue, krsdi->eValueType);
    }
    return NULL;
}
