#include "kr_dym_sdi.h"

/*static dataitem*/
T_KRSDI *kr_sdi_construct(T_KRShmSDIDef *sdi_def, T_KRModule *datamodule,
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func)
{
    T_KRSDI *krsdi = kr_calloc(sizeof(T_KRSDI));
    if (krsdi == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krsdi failed!");
        return NULL;
    }
    krsdi->ptShmSDIDef = sdi_def;
    krsdi->lSDIId = sdi_def->lSdiId;
    krsdi->ptSDICalc = kr_calc_construct(sdi_def->caSdiFilterFormat[0], \
            sdi_def->caSdiFilterString, get_type_func, get_value_func);
    krsdi->eValueType = sdi_def->caSdiValueType[0];
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
    krsdi->ptRelated = kr_hashtable_new(kr_pointer_hash, kr_pointer_equal);
    
    return krsdi;
}

void kr_sdi_init(T_KRSDI *krsdi)
{
    /*initialize first*/
    krsdi->eValueInd = KR_VALUE_UNSET;
    kr_hashtable_remove_all(krsdi->ptRelated);

    /*string comes from kr_strdup, need kr_free*/
    if (krsdi->eValueType == KR_TYPE_STRING)
        kr_free(krsdi->uValue.s);
    memset(&krsdi->uValue, 0x00, sizeof(krsdi->uValue));
}

void kr_sdi_destruct(T_KRSDI *krsdi)
{
    kr_hashtable_destroy(krsdi->ptRelated);
    kr_calc_destruct(krsdi->ptSDICalc);
    kr_free(krsdi);
}

cJSON *kr_sdi_info(T_KRSDI *krsdi) 
{ 
    cJSON *sdi = cJSON_CreateObject(); 
    cJSON *def = kr_shm_sdi_info(krsdi->ptShmSDIDef); 
    cJSON_AddItemToObject(sdi, "def", def); 
    return sdi; 
}

T_KRSDITable *kr_sdi_table_construct(T_KRShmSDI *shm_sdi, T_KRModule *datamodule, KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func)
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
        krsdi = kr_sdi_construct(&shm_sdi->stShmSDIDef[i], datamodule, \
                get_type_func, get_value_func);
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

static void _kr_sdi_init_hfunc(void *key, void *value, void *data)
{
    kr_sdi_init((T_KRSDI *)value);
}

void kr_sdi_table_init(T_KRSDITable *krsditable)
{
    kr_hashtable_foreach(krsditable->ptSDITable, _kr_sdi_init_hfunc, NULL);
}

void kr_sdi_table_destruct(T_KRSDITable *krsditable)
{
    kr_hashtable_destroy(krsditable->ptSDITable);
    kr_free(krsditable);
}

T_KRSDI *kr_sdi_lookup(T_KRSDITable *krsditable, int id)
{
    long lSID = (long )id;
    return kr_hashtable_lookup(krsditable->ptSDITable, &lSID);
}
