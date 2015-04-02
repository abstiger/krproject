#include "kr_data.h"

/*history dataitem*/
T_KRHDI *kr_hdi_construct(T_KRParamHDIDef *ptParamHDIDef, T_KRModule *ptModule)
{
    T_KRHDI *ptHDI = kr_calloc(sizeof(T_KRHDI));
    if (ptHDI == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptHDI failed!");
        return NULL;
    }
    ptHDI->ptParamHDIDef = ptParamHDIDef;
    ptHDI->lHDIId = ptParamHDIDef->lHdiId;
    ptHDI->eValueType = ptParamHDIDef->caHdiValueType[0];
    /*get the retrieve data function from module*/
    if (ptParamHDIDef->caHdiAggrFunc[0] != '\0') {
        ptHDI->pfHDIAggr = (KRHDIAggrFunc )kr_module_symbol(ptModule,
                ptParamHDIDef->caHdiAggrFunc);
        if (ptHDI->pfHDIAggr == NULL) {
            KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", \
                    ptParamHDIDef->caHdiAggrFunc);
            return NULL;
        }
    }
    ptHDI->ptCurrRec = NULL;
    
    return ptHDI;
}

void kr_hdi_init(T_KRHDI *ptHDI)
{
    /*initialize first*/
    ptHDI->eValueInd = KR_VALUE_UNSET;

    /*string comes from kr_strdup, need kr_free*/
    if (ptHDI->eValueType == KR_TYPE_STRING)
        kr_free(ptHDI->uValue.s);
    memset(&ptHDI->uValue, 0x00, sizeof(ptHDI->uValue));
}

void kr_hdi_destruct(T_KRHDI *ptHDI)
{
    kr_free(ptHDI);
}


T_KRHDITable *kr_hdi_table_construct(T_KRParamHDI *ptParamHDI, T_KRModule *ptModule)
{
    T_KRHDITable *ptHdiTable = kr_calloc(sizeof(T_KRHDITable));
    if (ptHdiTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptHdiTable failed!");
        return NULL;
    }
    ptHdiTable->ptParamHDI = ptParamHDI;
    ptHdiTable->lHDICnt = ptParamHDI->lHDIDefCnt;
    ptHdiTable->ptHDITable = \
      kr_hashtable_new_full(kr_long_hash, kr_long_equal, \
                            NULL, (KRDestroyNotify )kr_hdi_destruct);
    
    int i = 0;
    T_KRHDI *ptHDI = NULL;
    for (i=0; i<ptParamHDI->lHDIDefCnt; ++i) {
        ptHDI = kr_hdi_construct(&ptParamHDI->stParamHDIDef[i], ptModule);
        if (ptHDI == NULL) {
            KR_LOG(KR_LOGERROR, "kr_hdi_construct [%d] failed!", i);
            kr_hashtable_destroy(ptHdiTable->ptHDITable);
            kr_free(ptHdiTable);
            return NULL;
        }
        kr_hashtable_insert(ptHdiTable->ptHDITable, &ptHDI->lHDIId, ptHDI);
    }
    ptHdiTable->tConstructTime = ptParamHDI->tLastLoadTime;
    
    return ptHdiTable;    
}

static void _kr_hdi_init_hfunc(void *key, void *value, void *data)
{
    kr_hdi_init((T_KRHDI *)value);
}

void kr_hdi_table_init(T_KRHDITable *ptHdiTable)
{
    kr_hashtable_foreach(ptHdiTable->ptHDITable, _kr_hdi_init_hfunc, NULL);
}

void kr_hdi_table_destruct(T_KRHDITable *ptHdiTable)
{
    kr_hashtable_destroy(ptHdiTable->ptHDITable);
    kr_free(ptHdiTable);
}

T_KRHDI *kr_hdi_lookup(T_KRHDITable *ptHdiTable, int id)
{
    long lHID = (long )id;
    return kr_hashtable_lookup(ptHdiTable->ptHDITable, &lHID);
}

