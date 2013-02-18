#include "kr_sdi.h"

/*static dataitem*/
T_KRSDI *kr_sdi_construct(T_KRShmSDIDef *sdi_def, T_KRModule *datamodule)
{
    T_KRSDI *krsdi = kr_calloc(sizeof(T_KRSDI));
    if (krsdi == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krsdi failed!");
        return NULL;
    }
    krsdi->ptShmSDIDef = sdi_def;
    krsdi->lSDIId = sdi_def->lSdiId;
    krsdi->ptSDICalc = kr_calc_construct(KR_CALCBEHOOF_DATA, \
            sdi_def->caSdiFilterString, kr_rule_get_type, kr_rule_get_value);
    krsdi->eValueType = sdi_def->caSdiValueType[0];
    krsdi->SDIAggrFunc = (KRSDIAggrFunc )kr_sdi_aggr_func;
    if (sdi_def->caSdiAggrFunc[0] != '\0') {
        krsdi->SDIAggrFunc = (KRSDIAggrFunc )kr_module_symbol(datamodule, 
                sdi_def->caSdiAggrFunc);
        if (krsdi->SDIAggrFunc == NULL) {
            KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", \
                    sdi_def->caSdiAggrFunc);
            return NULL;
        }
    }
    krsdi->eValueInd = KR_VALUE_UNSET;
    
    return krsdi;
}

int kr_sdi_compute(T_KRSDI *krsdi, void *param)
{
    krsdi->eValueInd == KR_VALUE_UNSET;
    
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

    if (krsdi->SDIAggrFunc(krsdi, ptContext) != 0) {
        KR_LOG(KR_LOGERROR, "Run SDI[%ld] AggrFunc failed!", krsdi->lSDIId);
        return -1;
    }
    
    return 0;
}

void kr_sdi_destruct(T_KRSDI *krsdi)
{
    kr_calc_destruct(krsdi->ptSDICalc);
    kr_free(krsdi);
}

T_KRSDITable *kr_sdi_table_construct(T_KRShmSDI *shm_sdi, T_KRModule *datamodule)
{
    T_KRSDITable *krsditable = kr_calloc(sizeof(T_KRSDITable));
    if (krsditable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krsditable failed!");
        return NULL;
    }
    krsditable->ptShmSDIs = shm_sdi;
    krsditable->lSDICnt = shm_sdi->lSDIDefCnt;
    krsditable->ptSDITable = \
      kr_hashtable_new_full(kr_long_hash, kr_long_equal, \
                            NULL, (KRDestroyNotify )kr_sdi_destruct);
    
    int i = 0;
    T_KRSDI *krsdi = NULL;
    for (i=0; i<shm_sdi->lSDIDefCnt; ++i) {
        krsdi = kr_sdi_construct(&shm_sdi->stShmSDIDef[i], datamodule);
        if (krsdi == NULL) {
            KR_LOG(KR_LOGERROR, "kr_sdi_construct [%d] failed!", i);
            kr_hashtable_destroy(krsditable->ptSDITable);
            kr_free(krsditable);
            return NULL;
        }
        kr_hashtable_insert(krsditable->ptSDITable, &krsdi->lSDIId, krsdi);
    }
    krsditable->tConstructTime = shm_sdi->tLastLoadTime;
    
    return krsditable;    
}


void kr_sdi_table_destruct(T_KRSDITable *krsditable)
{
    kr_hashtable_destroy(krsditable->ptSDITable);
    kr_free(krsditable);
}


E_KRFieldType kr_sdi_get_type(int sid, T_KRContext *krcontext)
{
    if (krcontext == NULL) return KR_FIELDTYPE_UNKNOWN;
        
    long lSID = (long )sid;
    T_KRSDITable *krsditable = krcontext->ptDym->sditable;
    T_KRSDI *krsdi = kr_hashtable_lookup(krsditable->ptSDITable, &lSID);
    if (krsdi == NULL) return KR_FIELDTYPE_UNKNOWN;
    
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
            KR_LOG(KR_LOGERROR, "kr_sdi_compute [%d] failed!", sid);
            return NULL;
        }
        krsdi->ptCurrRec = krcontext->ptCurrRec;
    } else {
        KR_LOG(KR_LOGDEBUG, "Get SDI [%d] value record cached!", sid);
    }
    
    if (krsdi->eValueInd == KR_VALUE_SETED) {
        switch(krsdi->eValueType)
        {
            case KR_FIELDTYPE_INT:
                return &krsdi->eValue.i;
            case KR_FIELDTYPE_LONG:
                return &krsdi->eValue.l;
            case KR_FIELDTYPE_DOUBLE:
                return &krsdi->eValue.d;
            case KR_FIELDTYPE_STRING:
                return krsdi->eValue.s;
        }
    }
    return NULL;
}
