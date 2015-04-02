#include "kr_data.h"

/*static dataitem*/
T_KRSDI *kr_sdi_construct(T_KRParamSDIDef *ptParamSDIDef, T_KRModule *ptModule,
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue)
{
    T_KRSDI *ptSDI = kr_calloc(sizeof(T_KRSDI));
    if (ptSDI == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptSDI failed!");
        return NULL;
    }
    ptSDI->ptParamSDIDef = ptParamSDIDef;
    ptSDI->lSDIId = ptParamSDIDef->lSdiId;
    ptSDI->ptSDICalc = kr_calc_construct(ptParamSDIDef->caSdiFilterFormat[0], \
            ptParamSDIDef->caSdiFilterString, pfGetType, pfGetValue);
    ptSDI->eValueType = ptParamSDIDef->caSdiValueType[0];
    if (ptParamSDIDef->caSdiAggrFunc[0] != '\0') {
        ptSDI->pfSDIAggr = (KRSDIAggrFunc )kr_module_symbol(ptModule, 
                ptParamSDIDef->caSdiAggrFunc);
        if (ptSDI->pfSDIAggr == NULL) {
            KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", \
                    ptParamSDIDef->caSdiAggrFunc);
            return NULL;
        }
    }
    ptSDI->eValueInd = KR_VALUE_UNSET;
    ptSDI->ptRelated = kr_hashtable_new(kr_pointer_hash, kr_pointer_equal);
    
    return ptSDI;
}

void kr_sdi_init(T_KRSDI *ptSDI)
{
    /*initialize first*/
    ptSDI->eValueInd = KR_VALUE_UNSET;
    kr_hashtable_remove_all(ptSDI->ptRelated);

    /*string comes from kr_strdup, need kr_free*/
    if (ptSDI->eValueType == KR_TYPE_STRING)
        kr_free(ptSDI->uValue.s);
    memset(&ptSDI->uValue, 0x00, sizeof(ptSDI->uValue));
}

void kr_sdi_destruct(T_KRSDI *ptSDI)
{
    kr_hashtable_destroy(ptSDI->ptRelated);
    kr_calc_destruct(ptSDI->ptSDICalc);
    kr_free(ptSDI);
}


T_KRSDITable *kr_sdi_table_construct(T_KRParamSDI *ptParamSDI, T_KRModule *ptModule, KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue)
{
    T_KRSDITable *ptSdiTable = kr_calloc(sizeof(T_KRSDITable));
    if (ptSdiTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptSdiTable failed!");
        return NULL;
    }
    ptSdiTable->ptParamSDI = ptParamSDI;
    ptSdiTable->lSDICnt = ptParamSDI->lSDIDefCnt;
    ptSdiTable->ptSDITable = \
      kr_hashtable_new_full(kr_long_hash, kr_long_equal, \
                            NULL, (KRDestroyNotify )kr_sdi_destruct);
    
    int i = 0;
    T_KRSDI *ptSDI = NULL;
    for (i=0; i<ptParamSDI->lSDIDefCnt; ++i) {
        ptSDI = kr_sdi_construct(&ptParamSDI->stParamSDIDef[i], ptModule, \
                pfGetType, pfGetValue);
        if (ptSDI == NULL) {
            KR_LOG(KR_LOGERROR, "kr_sdi_construct [%d] failed!", i);
            kr_hashtable_destroy(ptSdiTable->ptSDITable);
            kr_free(ptSdiTable);
            return NULL;
        }
        kr_hashtable_insert(ptSdiTable->ptSDITable, &ptSDI->lSDIId, ptSDI);
    }
    ptSdiTable->tConstructTime = ptParamSDI->tLastLoadTime;
    
    return ptSdiTable;    
}

static void _kr_sdi_init_hfunc(void *key, void *value, void *data)
{
    kr_sdi_init((T_KRSDI *)value);
}

void kr_sdi_table_init(T_KRSDITable *ptSdiTable)
{
    kr_hashtable_foreach(ptSdiTable->ptSDITable, _kr_sdi_init_hfunc, NULL);
}

void kr_sdi_table_destruct(T_KRSDITable *ptSdiTable)
{
    kr_hashtable_destroy(ptSdiTable->ptSDITable);
    kr_free(ptSdiTable);
}

T_KRSDI *kr_sdi_lookup(T_KRSDITable *ptSdiTable, int id)
{
    long lSID = (long )id;
    return kr_hashtable_lookup(ptSdiTable->ptSDITable, &lSID);
}

