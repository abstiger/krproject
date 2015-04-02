#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"
#include "dbs/dbs/set_def_cur.h"
#include "kr_param_set.h"

int kr_param_set_load(T_DbsEnv *ptDbsEnv, T_KRParamSet *ptParamSet)
{
    int nRet = 0;
    int iResult = 0;
    int iCnt = 0;    
    T_SetDefCur stSetDefCur = {0};
    T_KRParamSetDef *ptParamSetDef = &ptParamSet->stParamSetDef[0];
    
    iResult = dbsSetDefCur(ptDbsEnv, KR_DBCUROPEN, &stSetDefCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSetDefCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsSetDefCur(ptDbsEnv, KR_DBCURFETCH, &stSetDefCur);
        if (iResult != KR_DBNOTFOUND && 
                iResult != KR_DBOK && iResult != KR_DBOKWITHINFO) {
            KR_LOG(KR_LOGERROR, "dbsSetDefCur Fetch Error[%d]![%s]:[%s]",
                    iResult, ptDbsEnv->sqlstate, ptDbsEnv->sqlerrmsg);
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
        
        memset(ptParamSetDef, 0x00, sizeof(T_KRParamSetDef));
        ptParamSetDef->lSetId = stSetDefCur.lOutSetId;
        strncpy(ptParamSetDef->caSetName, \
                kr_string_rtrim(stSetDefCur.caOutSetName), \
                sizeof(ptParamSetDef->caSetName));
        strncpy(ptParamSetDef->caSetDesc, \
                kr_string_rtrim(stSetDefCur.caOutSetDesc), \
                sizeof(ptParamSetDef->caSetDesc));
        strncpy(ptParamSetDef->caSetUsage, \
                kr_string_rtrim(stSetDefCur.caOutSetUsage), \
                sizeof(ptParamSetDef->caSetUsage));
        strncpy(ptParamSetDef->caSetType, \
                kr_string_rtrim(stSetDefCur.caOutSetType), \
                sizeof(ptParamSetDef->caSetType));
        strncpy(ptParamSetDef->caElementType, \
                kr_string_rtrim(stSetDefCur.caOutElementType), \
                sizeof(ptParamSetDef->caElementType));
        ptParamSetDef->lElementLength = stSetDefCur.lOutElementLength;

        ptParamSet->lSetDefCnt++;
        ptParamSetDef++;
        iCnt++;
    }
    ptParamSet->tLastLoadTime = time(NULL);

    iResult = dbsSetDefCur(ptDbsEnv, KR_DBCURCLOSE, &stSetDefCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSetDefCur Close Error!");
        return -1;
    }
    
    return nRet;
}


int kr_param_set_dump(T_KRParamSet *ptParamSet, FILE *fp)
{
    char caTimeString[80];
    struct tm *ptmNow = localtime(&ptParamSet->tLastLoadTime);
    strftime(caTimeString, sizeof(caTimeString), "%c", ptmNow);
    
    fprintf(fp, "Dumping Set...\n");
    fprintf(fp, "Last Load Time[%s]\n", caTimeString);
    T_KRParamSetDef *ptParamSetDef = &ptParamSet->stParamSetDef[0];
    for (int i=0; i<ptParamSet->lSetDefCnt; i++) {
        fprintf(fp, "  lSetId=[%ld], caSetName=[%s], caSetDesc=[%s] \n", 
                ptParamSetDef->lSetId, ptParamSetDef->caSetName, ptParamSetDef->caSetDesc);
        fprintf(fp, "  caSetUsage=[%s], caSetType=[%s], caElementType=[%s], lElementLength=[%ld]\n", 
                ptParamSetDef->caSetUsage, ptParamSetDef->caSetType, ptParamSetDef->caElementType, ptParamSetDef->lElementLength);
        ptParamSetDef++;
    }
    
    return 0;
}


