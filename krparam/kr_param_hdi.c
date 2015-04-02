#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"
#include "dbs/dbs/hdi_cur.h"
#include "kr_param_hdi.h"

int kr_param_hdi_load(T_DbsEnv *ptDbsEnv, T_KRParamHDI *ptParamHDI)
{
    int nRet = 0;
    int iResult = 0;
    int iCnt = 0;    
    T_HdiCur stHdiCur = {0};
    T_KRParamHDIDef *ptParamHDIDef = &ptParamHDI->stParamHDIDef[0];
    
    iResult = dbsHdiCur(ptDbsEnv, KR_DBCUROPEN, &stHdiCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsHdiCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsHdiCur(ptDbsEnv, KR_DBCURFETCH, &stHdiCur);
        if (iResult != KR_DBNOTFOUND && 
                iResult != KR_DBOK && iResult != KR_DBOKWITHINFO) {
            KR_LOG(KR_LOGERROR, "dbsHdiCur Fetch Error[%d]![%s]:[%s]",
                    iResult, ptDbsEnv->sqlstate, ptDbsEnv->sqlerrmsg);
            nRet = -1;
            break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Load [%d] HDIs Totally!", iCnt);
            break;
        }
        
        if (iCnt >= N_MAX_HDI_NUM) {
            KR_LOG(KR_LOGERROR, "Only [%d] HDIs Allowed!", N_MAX_HDI_NUM);
            nRet = -1;
            break;
        }
        
        memset(ptParamHDIDef, 0x00, sizeof(T_KRParamHDIDef));
        ptParamHDIDef->lHdiId = stHdiCur.lOutHdiId;
        strncpy(ptParamHDIDef->caHdiName, \
                kr_string_rtrim(stHdiCur.caOutHdiName), \
                sizeof(ptParamHDIDef->caHdiName));
        strncpy(ptParamHDIDef->caHdiDesc, \
                kr_string_rtrim(stHdiCur.caOutHdiDesc), \
                sizeof(ptParamHDIDef->caHdiDesc));
        strncpy(ptParamHDIDef->caHdiType, \
                kr_string_rtrim(stHdiCur.caOutHdiType), \
                sizeof(ptParamHDIDef->caHdiType));
        strncpy(ptParamHDIDef->caHdiValueType, \
                kr_string_rtrim(stHdiCur.caOutHdiValueType), \
                sizeof(ptParamHDIDef->caHdiValueType));
        strncpy(ptParamHDIDef->caHdiAggrFunc, \
                kr_string_rtrim(stHdiCur.caOutHdiAggrFunc), \
                sizeof(ptParamHDIDef->caHdiAggrFunc));
        strncpy(ptParamHDIDef->caHdiFreeFunc, \
                kr_string_rtrim(stHdiCur.caOutHdiFreeFunc), \
                sizeof(ptParamHDIDef->caHdiFreeFunc));
        ptParamHDIDef->lStatisticsDatasrc = stHdiCur.lOutStatisticsDatasrc;
        ptParamHDIDef->lStatisticsIndex = stHdiCur.lOutStatisticsIndex;
        strncpy(ptParamHDIDef->caStatisticsType, \
                kr_string_rtrim(stHdiCur.caOutStatisticsType), \
                sizeof(ptParamHDIDef->caStatisticsType));
        ptParamHDIDef->lStatisticsValue = stHdiCur.lOutStatisticsValue;
        strncpy(ptParamHDIDef->caStatisticsMethod, \
                kr_string_rtrim(stHdiCur.caOutStatisticsMethod), \
                sizeof(ptParamHDIDef->caStatisticsMethod));

        ptParamHDI->lHDIDefCnt++;
        ptParamHDIDef++;
        iCnt++;
    }
    ptParamHDI->tLastLoadTime = time(NULL);

    iResult = dbsHdiCur(ptDbsEnv, KR_DBCURCLOSE, &stHdiCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsHdiCur Close Error!");
        return -1;
    }
    
    return nRet;
}


int kr_param_hdi_dump(T_KRParamHDI *ptParamHDI, FILE *fp)
{
    long l;
    T_KRParamHDIDef *ptParamHDIDef = &ptParamHDI->stParamHDIDef[0];
    char            caTimeString[80];
    struct tm *ptmNow = localtime(&ptParamHDI->tLastLoadTime);
    strftime(caTimeString, sizeof(caTimeString), "%c", ptmNow);
    
    fprintf(fp, "Dumping HDI...\n");
    fprintf(fp, "Last Load Time[%s]\n", caTimeString);
    for (l=0; l<ptParamHDI->lHDIDefCnt; l++) {
        fprintf(fp, "  lHdiId=[%ld], caHdiName=[%s], caHdiDesc=[%s] \n", 
                ptParamHDIDef->lHdiId, ptParamHDIDef->caHdiName, ptParamHDIDef->caHdiDesc);
        fprintf(fp, "  caHdiType=[%s], caHdiValueType=[%s], caHdiAggrFunc=[%s], caHdiFreeFunc=[%s]\n", 
                ptParamHDIDef->caHdiType, ptParamHDIDef->caHdiValueType, ptParamHDIDef->caHdiAggrFunc, ptParamHDIDef->caHdiFreeFunc);
        fprintf(fp, "  lStatisticsDatasrc=[%ld], lStatisticsIndex=[%ld] \n", 
                ptParamHDIDef->lStatisticsDatasrc, ptParamHDIDef->lStatisticsIndex);
        fprintf(fp, "  caStatisticsType=[%s], lStatisticsValue=[%ld], caStatisticsMethod=[%s] \n", 
                ptParamHDIDef->caStatisticsType, ptParamHDIDef->lStatisticsValue, 
                ptParamHDIDef->caStatisticsMethod);
        ptParamHDIDef++;
    }
    
    return 0;
}


