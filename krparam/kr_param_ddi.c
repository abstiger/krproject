#include "krutils/kr_utils.h"
#include "dbs/dbs/ddi_cur.h"
#include "kr_param_ddi.h"

int kr_param_ddi_load(T_DbsEnv *ptDbsEnv, T_KRParamDDI *ptParamDDI)
{
    int nRet = 0;
    int iResult = 0;
    int iCnt = 0;    
    T_DdiCur stDdiCur = {0};
    T_KRParamDDIDef *ptParamDDIDef = &ptParamDDI->stParamDDIDef[0];
    
    iResult = dbsDdiCur(ptDbsEnv, KR_DBCUROPEN, &stDdiCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDdiCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsDdiCur(ptDbsEnv, KR_DBCURFETCH, &stDdiCur);
        if (iResult != KR_DBNOTFOUND && 
                iResult != KR_DBOK && iResult != KR_DBOKWITHINFO) {
            KR_LOG(KR_LOGERROR, "dbsDdiCur Fetch Error![%s]:[%s]", 
                    ptDbsEnv->sqlstate, ptDbsEnv->sqlerrmsg);
            nRet = -1;
            break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Load [%d] DDIs Totally!", iCnt);
            break;
        }
        
        if (iCnt >= N_MAX_DDI_NUM) {
            KR_LOG(KR_LOGERROR, "Only [%d] DDIs Allowed!", N_MAX_DDI_NUM);
            nRet = -1;
            break;
        }
        
        memset(ptParamDDIDef, 0x00, sizeof(T_KRParamDDIDef));
        ptParamDDIDef->lDdiId = stDdiCur.lOutDdiId;
        strncpy(ptParamDDIDef->caDdiName, \
                kr_string_rtrim(stDdiCur.caOutDdiName), \
                sizeof(ptParamDDIDef->caDdiName));
        strncpy(ptParamDDIDef->caDdiDesc, \
                kr_string_rtrim(stDdiCur.caOutDdiDesc), \
                sizeof(ptParamDDIDef->caDdiDesc));
        strncpy(ptParamDDIDef->caDdiType, \
                kr_string_rtrim(stDdiCur.caOutDdiType), \
                sizeof(ptParamDDIDef->caDdiType));
        strncpy(ptParamDDIDef->caDdiValueType, \
                kr_string_rtrim(stDdiCur.caOutDdiValueType), \
                sizeof(ptParamDDIDef->caDdiValueType));
        strncpy(ptParamDDIDef->caDdiAggrFunc, \
                kr_string_rtrim(stDdiCur.caOutDdiAggrFunc), \
                sizeof(ptParamDDIDef->caDdiAggrFunc));
        strncpy(ptParamDDIDef->caDdiFreeFunc, \
                kr_string_rtrim(stDdiCur.caOutDdiFreeFunc), \
                sizeof(ptParamDDIDef->caDdiFreeFunc));
        ptParamDDIDef->lStatisticsDatasrc = stDdiCur.lOutStatisticsDatasrc;
        ptParamDDIDef->lStatisticsIndex = stDdiCur.lOutStatisticsIndex;
        ptParamDDIDef->lStatisticsField = stDdiCur.lOutStatisticsField;
        strncpy(ptParamDDIDef->caStatisticsType, \
                kr_string_rtrim(stDdiCur.caOutStatisticsType), \
                sizeof(ptParamDDIDef->caStatisticsType));
        ptParamDDIDef->lStatisticsValue = stDdiCur.lOutStatisticsValue;
        ptParamDDIDef->lStatisticsCount = stDdiCur.lOutStatisticsCount;
        strncpy(ptParamDDIDef->caStatisticsMethod, \
                kr_string_rtrim(stDdiCur.caOutStatisticsMethod), \
                sizeof(ptParamDDIDef->caStatisticsMethod));
        strncpy(ptParamDDIDef->caDdiFilterFormat, \
                kr_string_rtrim(stDdiCur.caOutDdiFilterFormat), \
                sizeof(ptParamDDIDef->caDdiFilterFormat));
        strncpy(ptParamDDIDef->caDdiFilterString, \
                kr_string_rtrim(stDdiCur.caOutDdiFilterString), \
                sizeof(ptParamDDIDef->caDdiFilterString));

        ptParamDDI->lDDIDefCnt++;
        ptParamDDIDef++;
        iCnt++;
    }
    ptParamDDI->tLastLoadTime = time(NULL);

    iResult = dbsDdiCur(ptDbsEnv, KR_DBCURCLOSE, &stDdiCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDdiCur Close Error!");
        return -1;
    }
    
    return nRet;
}


int kr_param_ddi_dump(T_KRParamDDI *ptParamDDI, FILE *fp)
{
    long l;
    T_KRParamDDIDef *ptParamDDIDef = &ptParamDDI->stParamDDIDef[0];
    char            caTimeString[80];
    struct tm *ptmNow = localtime(&ptParamDDI->tLastLoadTime);
    strftime(caTimeString, sizeof(caTimeString), "%c", ptmNow);
    
    fprintf(fp, "Dumping DDI...\n");
    fprintf(fp, "Last Load Time[%s]\n", caTimeString);
    for (l=0; l<ptParamDDI->lDDIDefCnt; l++) {
        fprintf(fp, "  lDdiId=[%ld], caDdiName=[%s], caDdiDesc=[%s] \n", 
                ptParamDDIDef->lDdiId, ptParamDDIDef->caDdiName, ptParamDDIDef->caDdiDesc);
        fprintf(fp, "  caDdiType=[%s], caDdiValueType=[%s], caDdiAggrFunc=[%s], caDdiFreeFunc=[%s]\n", 
                ptParamDDIDef->caDdiType, ptParamDDIDef->caDdiValueType, ptParamDDIDef->caDdiAggrFunc, ptParamDDIDef->caDdiFreeFunc);
        fprintf(fp, "  lStatisticsDatasrc=[%ld], lStatisticsIndex=[%ld], lStatisticsField=[%ld] \n", 
                ptParamDDIDef->lStatisticsDatasrc, ptParamDDIDef->lStatisticsIndex, ptParamDDIDef->lStatisticsField);
        fprintf(fp, "  caStatisticsType=[%s], lStatisticsValue=[%ld] \n", 
                ptParamDDIDef->caStatisticsType, ptParamDDIDef->lStatisticsValue);        
        fprintf(fp, "  lStatisticsCount=[%ld], caStatisticsMethod=[%s] \n", 
                ptParamDDIDef->lStatisticsCount, ptParamDDIDef->caStatisticsMethod);
        fprintf(fp, "  caDdiFilterFormat=[%s], caDdiFilterString=[%s] \n", ptParamDDIDef->caDdiFilterFormat, ptParamDDIDef->caDdiFilterString);        
        ptParamDDIDef++;
    }
    
    return 0;
}

