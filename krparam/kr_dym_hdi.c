#include "kr_dym_hdi.h"

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

