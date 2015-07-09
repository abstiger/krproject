#include "kr_param_persist.h"
#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"
#include "dbs/dbs/param_cur.h"
#include "dbs/dbs/param_object_ins.h"

void *kr_param_persist_odbc_load_pre(T_KRParamPersistConfig *ptParamPersistConfig)
{
    char *psDBName = ptParamPersistConfig->value.odbc.name;
    char *psDBUser = ptParamPersistConfig->value.odbc.user;
    char *psDBPass = ptParamPersistConfig->value.odbc.pass;
    T_DbsEnv *ptDbsEnv = dbsConnect(psDBName, psDBUser, psDBPass);
    if (ptDbsEnv == NULL) {
        KR_LOG(KR_LOGERROR, "dbsConnect [%s] [%s] [%s] failed!",
                    psDBName, psDBUser, psDBPass);
        return NULL;
    }
    return ptDbsEnv;
}


void kr_param_persist_odbc_load_post(void *ptResource, T_KRParamPersistConfig *ptParamPersistConfig)
{
    T_DbsEnv *ptDbsEnv = (T_DbsEnv *)ptResource;
    if (ptDbsEnv->sqlcode != KR_DBOK) {
        dbsRollback(ptDbsEnv);
    } else {
        dbsCommit(ptDbsEnv);
    }
    dbsDisconnect(ptDbsEnv);
}


int kr_param_persist_odbc_load(T_KRParam *ptParam, char *psParamVersion, char *psParamClassName, void *ptResource)
{
    T_DbsEnv *ptDbsEnv = (T_DbsEnv *)ptResource;
    int nRet = 0, iResult = 0, iCnt = 0;
    
    T_ParamCur stParamCur = {0};
    strcpy(stParamCur.caInParamVersion, psParamVersion);
    strcpy(stParamCur.caInParamClassName, psParamClassName);
    
    iResult = dbsParamCur(ptDbsEnv, KR_DBCUROPEN, &stParamCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsParamCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsParamCur(ptDbsEnv, KR_DBCURFETCH, &stParamCur);
        if (iResult != KR_DBNOTFOUND && 
                iResult != KR_DBOK && iResult != KR_DBOKWITHINFO) {
            KR_LOG(KR_LOGERROR, "dbsParamCur Fetch Error[%d]![%s]:[%s]",
                    iResult, ptDbsEnv->sqlstate, ptDbsEnv->sqlerrmsg);
            nRet = -1;
            break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Load [%d] Objects of [%s] Totally!", \
                iCnt, psParamClassName);
            break;
        }
        
        kr_string_rtrim(stParamCur.caOutParamClassName);
        kr_string_rtrim(stParamCur.caOutParamObjectKey);
        kr_string_rtrim(stParamCur.caOutParamObjectString);
        
        kr_param_object_add(ptParam, 
                stParamCur.caOutParamClassName, 
                stParamCur.caOutParamObjectKey, 
                stParamCur.caOutParamObjectString);

        iCnt++;
    }

    iResult = dbsParamCur(ptDbsEnv, KR_DBCURCLOSE, &stParamCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsParamCur Close Error!");
        return -1;
    }
    
    return nRet;
}


void *kr_param_persist_odbc_dump_pre(T_KRParamPersistConfig *ptParamPersistConfig)
{
    char *psDBName = ptParamPersistConfig->value.odbc.name;
    char *psDBUser = ptParamPersistConfig->value.odbc.user;
    char *psDBPass = ptParamPersistConfig->value.odbc.pass;
    T_DbsEnv *ptDbsEnv = dbsConnect(psDBName, psDBUser, psDBPass);
    if (ptDbsEnv == NULL) {
        KR_LOG(KR_LOGERROR, "dbsConnect [%s] [%s] [%s] failed!", \
                    psDBName, psDBUser, psDBPass);
        return NULL;
    }
    return ptDbsEnv;
}


void kr_param_persist_odbc_dump_post(void *ptResource, T_KRParamPersistConfig *ptParamPersistConfig)
{
    T_DbsEnv *ptDbsEnv = (T_DbsEnv *)ptResource;
    if (ptDbsEnv->sqlcode != KR_DBOK) {
        dbsRollback(ptDbsEnv);
    } else {
        dbsCommit(ptDbsEnv);
    }
    dbsDisconnect(ptDbsEnv);
}


int kr_param_persist_odbc_dump(char *psParamVersion, char *psParamClassName, char *psParamObjectKey, char *psParamObjectString, void *ptResource)
{
    T_DbsEnv *ptDbsEnv = (T_DbsEnv *)ptResource;

    T_ParamObjectIns stParamObjectIns = {0};
    strcpy(stParamObjectIns.caInParamVersion, psParamVersion);
    strcpy(stParamObjectIns.caInParamClassName, psParamClassName);
    strcpy(stParamObjectIns.caInParamObjectKey, psParamObjectKey);
    strcpy(stParamObjectIns.caInParamObjectString, psParamObjectString);
    if (dbsParamObjectIns(ptDbsEnv, &stParamObjectIns) != 0) {
        KR_LOG(KR_LOGERROR, "insert kr_param_object failed![%d],[%s]", \
            ptDbsEnv->sqlcode, ptDbsEnv->sqlerrmsg);
        return -1;
    }

    return 0;
}
