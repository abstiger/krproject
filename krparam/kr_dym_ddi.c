#include "kr_dym_ddi.h"

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
    krddi->ptDDICalc = kr_calc_construct(ddi_def->caDdiFilterFormat[0], \
            ddi_def->caDdiFilterString);
    krddi->eValueType = ddi_def->caDdiValueType[0];
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
    krddi->ptRelated = kr_hashtable_new(kr_pointer_hash, kr_pointer_equal);
    
    return krddi;
}


void kr_ddi_destruct(T_KRDDI *krddi)
{
    kr_hashtable_destroy(krddi->ptRelated);
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

