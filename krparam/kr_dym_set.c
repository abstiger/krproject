#include "kr_dym_set.h"
#include "dbs/dbs/set_cfg_cur.h"


static int kr_set_load(T_KRHashSet *krhashset, int set_id, T_DbsEnv *dbsenv)
{
    int iCnt = 0;    
    int iResult = 0;
    int nRet = 0;
    T_SetCfgCur stSetCfgCur = {0};
    U_KRValue uEleVal ={0};
    
    stSetCfgCur.lInSetId = set_id;
    iResult = dbsSetCfgCur(dbsenv, KR_DBCUROPEN, &stSetCfgCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSetCfgCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsSetCfgCur(dbsenv, KR_DBCURFETCH, &stSetCfgCur);
        if (iResult != KR_DBNOTFOUND && 
                iResult != KR_DBOK && iResult != KR_DBOKWITHINFO) {
            KR_LOG(KR_LOGERROR, "dbsSetCfgCur Fetch Error[%d]![%s]:[%s]",
                    iResult, dbsenv->sqlstate, dbsenv->sqlerrmsg);
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

    iResult = dbsSetCfgCur(dbsenv, KR_DBCURCLOSE, &stSetCfgCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSetCfgCur Close Error!");
        return -1;
    }
    
    return nRet;
}


T_KRSet *kr_set_construct(T_KRShmSetDef *krshmsetdef, T_DbsEnv *dbsenv)
{
    T_KRSet *krset = kr_calloc(sizeof(T_KRSet));
    if (krset == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krset failed!");
        return NULL;
    }
    krset->ptShmSetDef = krshmsetdef;
    krset->lSetId = krshmsetdef->lSetId;
    krset->eValueType = (E_KRType )krshmsetdef->caElementType[0];
    
    krset->ptHashSet = kr_hashset_create(krshmsetdef->caSetName, \
            (E_KRType )krshmsetdef->caElementType[0]);
    if (krset == NULL) {
        KR_LOG(KR_LOGERROR, "kr_hashset_create [%ld] Failed!", \
                krshmsetdef->lSetId);
        return NULL;
    }
    
    if (kr_set_load(krset->ptHashSet, krshmsetdef->lSetId, dbsenv) != 0) {
        KR_LOG(KR_LOGERROR, "kr_set_load[%ld] Failed!", krshmsetdef->lSetId);
        kr_hashset_destroy(krset->ptHashSet);
        return NULL;
    }
    return krset;
}

void kr_set_init(T_KRSet *krset)
{
    //dummy function
}

void kr_set_destruct(T_KRSet *krset)
{
    kr_hashset_destroy(krset->ptHashSet);
    kr_free(krset);
}

cJSON *kr_set_info(T_KRSet *krset) 
{ 
    cJSON *set = cJSON_CreateObject(); 
    cJSON *def = kr_shm_set_info(krset->ptShmSetDef); 
    cJSON_AddItemToObject(set, "def", def); 
    return set; 
}

T_KRSetTable *kr_set_table_construct(T_KRShmSet *krshmset, T_DbsEnv *dbsenv)
{
    T_KRSetTable *krsettable = kr_calloc(sizeof(T_KRSetTable));
    if (krsettable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krsettable failed!");
        return NULL;
    }
    krsettable->ptShmSet = krshmset;
    krsettable->lSetCnt = krshmset->lSetDefCnt;
    krsettable->ptSetTable = \
      kr_hashtable_new_full(kr_long_hash, kr_long_equal, \
                            NULL, (KRDestroyNotify )kr_set_destruct);
    
    int i = 0;
    T_KRSet *krset = NULL;
    for (i=0; i<krshmset->lSetDefCnt; ++i) {
        krset = kr_set_construct(&krshmset->stShmSetDef[i], dbsenv);
        if (krset == NULL) {
            KR_LOG(KR_LOGERROR, "kr_set_construct [%d] failed!", i);
            kr_hashtable_destroy(krsettable->ptSetTable);
            kr_free(krsettable);
            return NULL;
        }
        kr_hashtable_insert(krsettable->ptSetTable, &krset->lSetId, krset);
    }
    krsettable->tConstructTime = krshmset->tLastLoadTime;
    
    return krsettable;    
}

void kr_set_table_init(T_KRSetTable *krsettable)
{
    //dummy function
}

void kr_set_table_destruct(T_KRSetTable *krsettable)
{
    kr_hashtable_destroy(krsettable->ptSetTable);
    kr_free(krsettable);
}

T_KRSet *kr_set_lookup(T_KRSetTable *krsettable, int id)
{
    long lSID = (long )id;
    return kr_hashtable_lookup(krsettable->ptSetTable, &lSID);
}
