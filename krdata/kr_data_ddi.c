#include "kr_data.h"

/*static dataitem*/
T_KRDDI *kr_ddi_construct(T_KRParamDDIDef *ptParamDDIDef, T_KRModule *ptModule,
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue)
{
    T_KRDDI *ptDDI = kr_calloc(sizeof(T_KRDDI));
    if (ptDDI == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptDDI failed!");
        return NULL;
    }
    ptDDI->ptParamDDIDef = ptParamDDIDef;
    ptDDI->lDDIId = ptParamDDIDef->lDdiId;
    ptDDI->ptDDICalc = kr_calc_construct(ptParamDDIDef->caDdiFilterFormat[0], \
            ptParamDDIDef->caDdiFilterString, pfGetType, pfGetValue);
    ptDDI->eValueType = ptParamDDIDef->caDdiValueType[0];
    /*get the retrieve data function from module*/
    if (ptParamDDIDef->caDdiAggrFunc[0] != '\0') {
        ptDDI->pfDDIAggr = (KRDDIAggrFunc )kr_module_symbol(ptModule,
                ptParamDDIDef->caDdiAggrFunc);
        if (ptDDI->pfDDIAggr == NULL) {
            KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", \
                    ptParamDDIDef->caDdiAggrFunc);
            return NULL;
        }
    }
    ptDDI->eValueInd = KR_VALUE_UNSET;
    ptDDI->ptRelated = kr_hashtable_new(kr_pointer_hash, kr_pointer_equal);
    
    return ptDDI;
}

void kr_ddi_init(T_KRDDI *ptDDI)
{
    /*initialize first*/
    ptDDI->eValueInd = KR_VALUE_UNSET;
    kr_hashtable_remove_all(ptDDI->ptRelated);

    /*string comes from kr_strdup, need kr_free*/
    if (ptDDI->eValueType == KR_TYPE_STRING)
        kr_free(ptDDI->uValue.s);
    memset(&ptDDI->uValue, 0x00, sizeof(ptDDI->uValue));
}

void kr_ddi_destruct(T_KRDDI *ptDDI)
{
    kr_hashtable_destroy(ptDDI->ptRelated);
    kr_calc_destruct(ptDDI->ptDDICalc);
    kr_free(ptDDI);
}


T_KRDDITable *kr_ddi_table_construct(T_KRParamDDI *ptParamDDI, T_KRModule *ptModule, KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue)
{
    T_KRDDITable *ptDdiTable = kr_calloc(sizeof(T_KRDDITable));
    if (ptDdiTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptDdiTable failed!");
        return NULL;
    }
    ptDdiTable->ptParamDDI = ptParamDDI;
    ptDdiTable->lDDICnt = ptParamDDI->lDDIDefCnt;
    ptDdiTable->ptDDITable = \
      kr_hashtable_new_full(kr_long_hash, kr_long_equal, \
                            NULL, (KRDestroyNotify )kr_ddi_destruct);
    
    int i = 0;
    T_KRDDI *ptDDI = NULL;
    for (i=0; i<ptParamDDI->lDDIDefCnt; ++i) {
        ptDDI = kr_ddi_construct(&ptParamDDI->stParamDDIDef[i], ptModule, \
                pfGetType, pfGetValue);
        if (ptDDI == NULL) {
            KR_LOG(KR_LOGERROR, "kr_ddi_construct [%d] failed!", i);
            kr_hashtable_destroy(ptDdiTable->ptDDITable);
            kr_free(ptDdiTable);
            return NULL;
        }
        kr_hashtable_insert(ptDdiTable->ptDDITable, &ptDDI->lDDIId, ptDDI);
    }
    ptDdiTable->tConstructTime = ptParamDDI->tLastLoadTime;
    
    return ptDdiTable;    
}

static void _kr_ddi_init_hfunc(void *key, void *value, void *data)
{
    kr_ddi_init((T_KRDDI *)value);
}

void kr_ddi_table_init(T_KRDDITable *ptDdiTable)
{
    kr_hashtable_foreach(ptDdiTable->ptDDITable, _kr_ddi_init_hfunc, NULL);
}

void kr_ddi_table_destruct(T_KRDDITable *ptDdiTable)
{
    kr_hashtable_destroy(ptDdiTable->ptDDITable);
    kr_free(ptDdiTable);
}

T_KRDDI *kr_ddi_lookup(T_KRDDITable *ptDdiTable, int id)
{
    long lDID = (long )id;
    return kr_hashtable_lookup(ptDdiTable->ptDDITable, &lDID);
}


