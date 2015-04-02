#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"
#include "dbs/dbs/sdi_cur.h"
#include "kr_param_sdi.h"

int kr_param_sdi_load(T_DbsEnv *ptDbsEnv, T_KRParamSDI *ptParamSDI)
{
    int nRet = 0;
    int iResult = 0;
    int iCnt = 0;    
    T_SdiCur stSdiCur = {0};
    T_KRParamSDIDef *ptParamSDIDef = &ptParamSDI->stParamSDIDef[0];
    
    iResult = dbsSdiCur(ptDbsEnv, KR_DBCUROPEN, &stSdiCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSdiCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsSdiCur(ptDbsEnv, KR_DBCURFETCH, &stSdiCur);
        if (iResult != KR_DBNOTFOUND && 
                iResult != KR_DBOK && iResult != KR_DBOKWITHINFO) {
            KR_LOG(KR_LOGERROR, "dbsSdiCur Fetch Error[%d]![%s]:[%s]",
                    iResult, ptDbsEnv->sqlstate, ptDbsEnv->sqlerrmsg);
            nRet = -1;
            break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Load [%d] SDIs Totally!", iCnt);
            break;
        }
        
        if (iCnt >= N_MAX_SDI_NUM) {
            KR_LOG(KR_LOGERROR, "Only [%d] SDIs Allowed!", N_MAX_SDI_NUM);
            nRet = -1;
            break;
        }
        
        memset(ptParamSDIDef, 0x00, sizeof(T_KRParamSDIDef));
        ptParamSDIDef->lSdiId = stSdiCur.lOutSdiId;
        strncpy(ptParamSDIDef->caSdiName, \
                kr_string_rtrim(stSdiCur.caOutSdiName), \
                sizeof(ptParamSDIDef->caSdiName));
        strncpy(ptParamSDIDef->caSdiDesc, \
                kr_string_rtrim(stSdiCur.caOutSdiDesc), \
                sizeof(ptParamSDIDef->caSdiDesc));
        strncpy(ptParamSDIDef->caSdiType, \
                kr_string_rtrim(stSdiCur.caOutSdiType), \
                sizeof(ptParamSDIDef->caSdiType));
        strncpy(ptParamSDIDef->caSdiValueType, \
                kr_string_rtrim(stSdiCur.caOutSdiValueType), \
                sizeof(ptParamSDIDef->caSdiValueType));
        strncpy(ptParamSDIDef->caSdiAggrFunc, \
                kr_string_rtrim(stSdiCur.caOutSdiAggrFunc), \
                sizeof(ptParamSDIDef->caSdiAggrFunc));
        strncpy(ptParamSDIDef->caSdiFreeFunc, \
                kr_string_rtrim(stSdiCur.caOutSdiFreeFunc), \
                sizeof(ptParamSDIDef->caSdiFreeFunc));
        ptParamSDIDef->lStatisticsDatasrc = stSdiCur.lOutStatisticsDatasrc;
        ptParamSDIDef->lStatisticsIndex = stSdiCur.lOutStatisticsIndex;
        ptParamSDIDef->lStatisticsField = stSdiCur.lOutStatisticsField;
        ptParamSDIDef->lStatisticsLocation = stSdiCur.lOutStatisticsLocation;
        strncpy(ptParamSDIDef->caLocationProperty, \
                kr_string_rtrim(stSdiCur.caOutLocationProperty), \
                sizeof(ptParamSDIDef->caLocationProperty));
        strncpy(ptParamSDIDef->caSdiFilterFormat, \
                kr_string_rtrim(stSdiCur.caOutSdiFilterFormat), \
                sizeof(ptParamSDIDef->caSdiFilterFormat));
        strncpy(ptParamSDIDef->caSdiFilterString, \
                kr_string_rtrim(stSdiCur.caOutSdiFilterString), \
                sizeof(ptParamSDIDef->caSdiFilterString));

        ptParamSDI->lSDIDefCnt++;
        ptParamSDIDef++;
        iCnt++;
    }
    ptParamSDI->tLastLoadTime = time(NULL);

    iResult = dbsSdiCur(ptDbsEnv, KR_DBCURCLOSE, &stSdiCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSdiCur Close Error!");
        return -1;
    }
    
    return nRet;
}


int kr_param_sdi_dump(T_KRParamSDI *ptParamSDI, FILE *fp)
{
    long l;
    T_KRParamSDIDef *ptParamSDIDef = &ptParamSDI->stParamSDIDef[0];
    char            caTimeString[80];
    struct tm *ptmNow = localtime(&ptParamSDI->tLastLoadTime);
    strftime(caTimeString, sizeof(caTimeString), "%c", ptmNow);
    
    fprintf(fp, "Dumping SDI...\n");
    fprintf(fp, "Last Load Time[%s]\n", caTimeString);
    for (l=0; l<ptParamSDI->lSDIDefCnt; l++) {
        fprintf(fp, "  lSdiId=[%ld], caSdiName=[%s], caSdiDesc=[%s] \n", 
                ptParamSDIDef->lSdiId, ptParamSDIDef->caSdiName, ptParamSDIDef->caSdiDesc);
        fprintf(fp, "  caSdiType=[%s], caSdiValueType=[%s],", 
                ptParamSDIDef->caSdiType, ptParamSDIDef->caSdiValueType);
        fprintf(fp, "  caSdiAggrFunc=[%s], caSdiFreeFunc=[%s] \n", 
                ptParamSDIDef->caSdiAggrFunc, ptParamSDIDef->caSdiFreeFunc);
        fprintf(fp, "  lStatisticsDatasrc=[%ld], lStatisticsIndex=[%ld], lStatisticsField=[%ld] \n", 
                ptParamSDIDef->lStatisticsDatasrc, ptParamSDIDef->lStatisticsIndex, ptParamSDIDef->lStatisticsField);
        fprintf(fp, "  lStatisticsLocation=[%ld], caLocationProperty=[%s] \n", 
                ptParamSDIDef->lStatisticsLocation, ptParamSDIDef->caLocationProperty);
        fprintf(fp, "  caSdiFilterFormat=[%s], caSdiFilterString=[%s] \n", ptParamSDIDef->caSdiFilterFormat, ptParamSDIDef->caSdiFilterString);
        ptParamSDIDef++;
    }
    
    return 0;
}

