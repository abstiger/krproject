#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"
#include "dbs/dbs/set_def_cur.h"
#include "kr_shm_set.h"

int kr_shm_set_load(T_DbsEnv *dbsenv, T_KRShmSet *ptShmSet)
{
    int nRet = 0;
    int iResult = 0;
    int iCnt = 0;    
    T_SetDefCur stSetDefCur = {0};
    T_KRShmSetDef *ptShmSetDef = &ptShmSet->stShmSetDef[0];
    
    iResult = dbsSetDefCur(dbsenv, KR_DBCUROPEN, &stSetDefCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSetDefCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsSetDefCur(dbsenv, KR_DBCURFETCH, &stSetDefCur);
        if (iResult != KR_DBNOTFOUND && 
                iResult != KR_DBOK && iResult != KR_DBOKWITHINFO) {
            KR_LOG(KR_LOGERROR, "dbsSetDefCur Fetch Error[%d]![%s]:[%s]",
                    iResult, dbsenv->sqlstate, dbsenv->sqlerrmsg);
            nRet = -1;
            break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Load [%d] Sets Totally!", iCnt);
            break;
        }
        
        if (iCnt >= N_MAX_SET_NUM) {
            KR_LOG(KR_LOGERROR, "Only [%d] Sets Allowed!", N_MAX_SET_NUM);
            nRet = -1;
            break;
        }
        
        memset(ptShmSetDef, 0x00, sizeof(T_KRShmSetDef));
        ptShmSetDef->lSetId = stSetDefCur.lOutSetId;
        strncpy(ptShmSetDef->caSetName, \
                kr_string_rtrim(stSetDefCur.caOutSetName), \
                sizeof(ptShmSetDef->caSetName));
        strncpy(ptShmSetDef->caSetDesc, \
                kr_string_rtrim(stSetDefCur.caOutSetDesc), \
                sizeof(ptShmSetDef->caSetDesc));
        strncpy(ptShmSetDef->caSetUsage, \
                kr_string_rtrim(stSetDefCur.caOutSetUsage), \
                sizeof(ptShmSetDef->caSetUsage));
        strncpy(ptShmSetDef->caSetType, \
                kr_string_rtrim(stSetDefCur.caOutSetType), \
                sizeof(ptShmSetDef->caSetType));
        strncpy(ptShmSetDef->caElementType, \
                kr_string_rtrim(stSetDefCur.caOutElementType), \
                sizeof(ptShmSetDef->caElementType));
        ptShmSetDef->lElementLength = stSetDefCur.lOutElementLength;

        ptShmSet->lSetDefCnt++;
        ptShmSetDef++;
        iCnt++;
    }
    ptShmSet->tLastLoadTime = time(NULL);

    iResult = dbsSetDefCur(dbsenv, KR_DBCURCLOSE, &stSetDefCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSetDefCur Close Error!");
        return -1;
    }
    
    return nRet;
}


int kr_shm_set_dump(T_KRShmSet *ptShmSet, FILE *fp)
{
    char caTimeString[80];
    struct tm *ptmNow = localtime(&ptShmSet->tLastLoadTime);
    strftime(caTimeString, sizeof(caTimeString), "%c", ptmNow);
    
    fprintf(fp, "Dumping Set...\n");
    fprintf(fp, "Last Load Time[%s]\n", caTimeString);
    T_KRShmSetDef *ptShmSetDef = &ptShmSet->stShmSetDef[0];
    for (int i=0; i<ptShmSet->lSetDefCnt; i++) {
        fprintf(fp, "  lSetId=[%ld], caSetName=[%s], caSetDesc=[%s] \n", 
                ptShmSetDef->lSetId, ptShmSetDef->caSetName, ptShmSetDef->caSetDesc);
        fprintf(fp, "  caSetUsage=[%s], caSetType=[%s], caElementType=[%s], lElementLength=[%ld]\n", 
                ptShmSetDef->caSetUsage, ptShmSetDef->caSetType, ptShmSetDef->caElementType, ptShmSetDef->lElementLength);
        ptShmSetDef++;
    }
    
    return 0;
}

cJSON *kr_shm_set_info(T_KRShmSetDef *ptShmSetDef)
{
    cJSON *set = cJSON_CreateObject();
    cJSON_AddNumberToObject(set, "id", ptShmSetDef->lSetId);
    cJSON_AddStringToObject(set, "name", ptShmSetDef->caSetName);
    cJSON_AddStringToObject(set, "desc", ptShmSetDef->caSetDesc);
    cJSON_AddStringToObject(set, "usage", ptShmSetDef->caSetUsage);
    cJSON_AddStringToObject(set, "type", ptShmSetDef->caSetType);
    cJSON_AddStringToObject(set, "ele_type", ptShmSetDef->caElementType);
    cJSON_AddNumberToObject(set, "ele_length", ptShmSetDef->lElementLength);
    return set;
}
