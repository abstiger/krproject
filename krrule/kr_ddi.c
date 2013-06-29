#include "kr_ddi.h"

/*static dataitem*/
T_KRDDI *kr_ddi_construct(T_KRShmDDIDef *ddi_def, T_KRModule *datamodule)
{
    T_KRDDI *krddi = kr_calloc(sizeof(T_KRDDI));
    if (krddi == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krddi failed!");
        return NULL;
    }
    krddi->ptShmDDIDef = ddi_def;
    krddi->lDDIId = ddi_def->lDdiId;
    //FIXME:
    krddi->ptDDICalc = kr_calc_construct('J', \
        ddi_def->caDdiFilterString, kr_rule_get_type, kr_rule_get_value);
    krddi->eValueType = ddi_def->caDdiValueType[0];
    krddi->DDIAggrFunc = (KRDDIAggrFunc )kr_ddi_aggr_func;
    /*get the retrieve data function from module*/
    if (ddi_def->caDdiAggrFunc[0] != '\0') {
        krddi->DDIAggrFunc = (KRDDIAggrFunc )kr_module_symbol(datamodule,
                ddi_def->caDdiAggrFunc);
        if (krddi->DDIAggrFunc == NULL) {
            KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", \
                    ddi_def->caDdiAggrFunc);
            return NULL;
        }
    }
    krddi->eValueInd = KR_VALUE_UNSET;
    
    return krddi;
}


int kr_ddi_compute(T_KRDDI *krddi, void *param)
{
    /*initialize first*/
    krddi->eValueInd == KR_VALUE_UNSET;
    /*string comes from kr_strdup, need kr_free*/
    if (krddi->eValueType == KR_FIELDTYPE_STRING)
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

    if (krddi->DDIAggrFunc(krddi, ptContext) != 0) {
        KR_LOG(KR_LOGERROR, "Run DDI[%ld] AggrFunc failed!", krddi->lDDIId);
        return -1;
    }
    
    return 0;
}

void kr_ddi_destruct(T_KRDDI *krddi)
{
    kr_calc_destruct(krddi->ptDDICalc);
    kr_free(krddi);
}

T_KRDDITable *kr_ddi_table_construct(T_KRShmDDI *shm_ddi, T_KRModule *datamodule)
{
    T_KRDDITable *krdditable = kr_calloc(sizeof(T_KRDDITable));
    if (krdditable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krdditable failed!");
        return NULL;
    }
    krdditable->ptShmDDIs = shm_ddi;
    krdditable->lDDICnt = shm_ddi->lDDIDefCnt;
    krdditable->ptDDITable = \
      kr_hashtable_new_full(kr_long_hash, kr_long_equal, \
                            NULL, (KRDestroyNotify )kr_ddi_destruct);
    
    int i = 0;
    T_KRDDI *krddi = NULL;
    for (i=0; i<shm_ddi->lDDIDefCnt; ++i) {
        krddi = kr_ddi_construct(&shm_ddi->stShmDDIDef[i], datamodule);
        if (krddi == NULL) {
            KR_LOG(KR_LOGERROR, "kr_ddi_construct [%d] failed!", i);
            kr_hashtable_destroy(krdditable->ptDDITable);
            kr_free(krdditable);
            return NULL;
        }
        kr_hashtable_insert(krdditable->ptDDITable, &krddi->lDDIId, krddi);
    }
    krdditable->tConstructTime = shm_ddi->tLastLoadTime;
    
    return krdditable;    
}


void kr_ddi_table_destruct(T_KRDDITable *krdditable)
{
    kr_hashtable_destroy(krdditable->ptDDITable);
    kr_free(krdditable);
}

E_KRFieldType kr_ddi_get_type(int did, T_KRContext *krcontext)
{
    if (krcontext == NULL) return KR_FIELDTYPE_UNKNOWN;
        
    long lDID = (long )did;
    T_KRDDITable *krdditable = krcontext->ptDym->dditable;
    T_KRDDI *krddi = kr_hashtable_lookup(krdditable->ptDDITable, &lDID);
    if (krddi == NULL) return KR_FIELDTYPE_UNKNOWN;
    
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
