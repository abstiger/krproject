#include "kr_data.h"
#include "dbs/dbs/set_cfg_cur.h"


static int kr_set_load(T_KRHashSet *krhashset, int set_id, T_DbsEnv *ptDbsEnv)
{
    int iCnt = 0;    
    int iResult = 0;
    int nRet = 0;
    T_SetCfgCur stSetCfgCur = {0};
    U_KRValue uEleVal ={0};
    
    stSetCfgCur.lInSetId = set_id;
    iResult = dbsSetCfgCur(ptDbsEnv, KR_DBCUROPEN, &stSetCfgCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSetCfgCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsSetCfgCur(ptDbsEnv, KR_DBCURFETCH, &stSetCfgCur);
        if (iResult != KR_DBNOTFOUND && 
                iResult != KR_DBOK && iResult != KR_DBOKWITHINFO) {
            KR_LOG(KR_LOGERROR, "dbsSetCfgCur Fetch Error[%d]![%s]:[%s]",
                    iResult, ptDbsEnv->sqlstate, ptDbsEnv->sqlerrmsg);
            nRet = -1;
            break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Load DataSet[%d],Elements[%d] Totally!", \
                    stSetCfgCur.lInSetId, iCnt);
            break;
        }
        
        kr_string_rtrim(stSetCfgCur.caOutElementValue);
        memset(&uEleVal, 0x00, sizeof(uEleVal));
        switch(krhashset->type)
        {
            case KR_TYPE_INT: 
                uEleVal.i = atoi(stSetCfgCur.caOutElementValue);
                nRet = kr_hashset_add(krhashset, &uEleVal.i);
                break;
            case KR_TYPE_LONG: 
                uEleVal.l = atol(stSetCfgCur.caOutElementValue);
                nRet = kr_hashset_add(krhashset, &uEleVal.l);
                break;
            case KR_TYPE_DOUBLE: 
                uEleVal.d = atof(stSetCfgCur.caOutElementValue);
                nRet = kr_hashset_add(krhashset, &uEleVal.d);
                break;
            case KR_TYPE_STRING: 
                uEleVal.s = stSetCfgCur.caOutElementValue;
                nRet = kr_hashset_add(krhashset, uEleVal.s);
                break;
            default:
                KR_LOG(KR_LOGERROR, "Unknown set value type[%c]!", \
                        krhashset->type);
                nRet = -1;
                break;
        }
        
        if (nRet != 0) {
            KR_LOG(KR_LOGERROR, "kr_set_add [%s] Error!", \
                   stSetCfgCur.caOutElementValue);
            break;
        }

        iCnt++;
    }

    iResult = dbsSetCfgCur(ptDbsEnv, KR_DBCURCLOSE, &stSetCfgCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSetCfgCur Close Error!");
        return -1;
    }
    
    return nRet;
}


T_KRSet *kr_set_construct(T_KRParamSetDef *ptParamSetDef, T_DbsEnv *ptDbsEnv)
{
    T_KRSet *ptSet = kr_calloc(sizeof(T_KRSet));
    if (ptSet == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptSet failed!");
        return NULL;
    }
    ptSet->ptParamSetDef = ptParamSetDef;
    ptSet->lSetId = ptParamSetDef->lSetId;
    ptSet->eValueType = (E_KRType )ptParamSetDef->caElementType[0];
    
    ptSet->ptHashSet = kr_hashset_create(ptParamSetDef->caSetName, \
            (E_KRType )ptParamSetDef->caElementType[0]);
    if (ptSet == NULL) {
        KR_LOG(KR_LOGERROR, "kr_hashset_create [%ld] Failed!", \
                ptParamSetDef->lSetId);
        return NULL;
    }
    
    if (kr_set_load(ptSet->ptHashSet, ptParamSetDef->lSetId, ptDbsEnv) != 0) {
        KR_LOG(KR_LOGERROR, "kr_set_load[%ld] Failed!", ptParamSetDef->lSetId);
        kr_hashset_destroy(ptSet->ptHashSet);
        return NULL;
    }
    return ptSet;
}

void kr_set_init(T_KRSet *ptSet)
{
    //dummy function
}

void kr_set_destruct(T_KRSet *ptSet)
{
    kr_hashset_destroy(ptSet->ptHashSet);
    kr_free(ptSet);
}


T_KRSetTable *kr_set_table_construct(T_KRParamSet *ptParamSet, T_DbsEnv *ptDbsEnv)
{
    T_KRSetTable *ptSetTable = kr_calloc(sizeof(T_KRSetTable));
    if (ptSetTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptSetTable failed!");
        return NULL;
    }
    ptSetTable->ptParamSet = ptParamSet;
    ptSetTable->lSetCnt = ptParamSet->lSetDefCnt;
    ptSetTable->ptSetTable = \
      kr_hashtable_new_full(kr_long_hash, kr_long_equal, \
                            NULL, (KRDestroyNotify )kr_set_destruct);
    
    int i = 0;
    T_KRSet *ptSet = NULL;
    for (i=0; i<ptParamSet->lSetDefCnt; ++i) {
        ptSet = kr_set_construct(&ptParamSet->stParamSetDef[i], ptDbsEnv);
        if (ptSet == NULL) {
            KR_LOG(KR_LOGERROR, "kr_set_construct [%d] failed!", i);
            kr_hashtable_destroy(ptSetTable->ptSetTable);
            kr_free(ptSetTable);
            return NULL;
        }
        kr_hashtable_insert(ptSetTable->ptSetTable, &ptSet->lSetId, ptSet);
    }
    ptSetTable->tConstructTime = ptParamSet->tLastLoadTime;
    
    return ptSetTable;    
}

void kr_set_table_init(T_KRSetTable *ptSetTable)
{
    //dummy function
}

void kr_set_table_destruct(T_KRSetTable *ptSetTable)
{
    kr_hashtable_destroy(ptSetTable->ptSetTable);
    kr_free(ptSetTable);
}

T_KRSet *kr_set_lookup(T_KRSetTable *ptSetTable, int id)
{
    long lSID = (long )id;
    return kr_hashtable_lookup(ptSetTable->ptSetTable, &lSID);
}

