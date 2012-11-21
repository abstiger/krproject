#include "kr_shm_hdi.h"
#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"
#include "dbs/dbs/hdi_cur.h"

int LoadShmHDI(T_DbsEnv *dbsenv, T_KRShmHDI *ptShmHDI)
{
    int nRet = 0;
    int iResult = 0;
    int iCnt = 0;    
    T_HdiCur stHdiCur = {0};
    T_KRShmHDIDef *ptShmHDIDef = &ptShmHDI->stShmHDIDef[0];
    
    iResult = dbsHdiCur(dbsenv, KR_DBCUROPEN, &stHdiCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsHdiCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsHdiCur(dbsenv, KR_DBCURFETCH, &stHdiCur);
        if (iResult != KR_DBNOTFOUND && iResult != KR_DBOK) {
            KR_LOG(KR_LOGERROR, "dbsHdiCur Fetch Error!");
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
        
        memset(ptShmHDIDef, 0x00, sizeof(T_KRShmHDIDef));
        ptShmHDIDef->lHdiId = stHdiCur.lOutHdiId;
        strncpy(ptShmHDIDef->caHdiName, \
                kr_string_rtrim(stHdiCur.caOutHdiName), \
                sizeof(ptShmHDIDef->caHdiName));
        strncpy(ptShmHDIDef->caHdiDesc, \
                kr_string_rtrim(stHdiCur.caOutHdiDesc), \
                sizeof(ptShmHDIDef->caHdiDesc));
        strncpy(ptShmHDIDef->caHdiType, \
                kr_string_rtrim(stHdiCur.caOutHdiType), \
                sizeof(ptShmHDIDef->caHdiType));
        strncpy(ptShmHDIDef->caHdiValueType, \
                kr_string_rtrim(stHdiCur.caOutHdiValueType), \
                sizeof(ptShmHDIDef->caHdiValueType));
        strncpy(ptShmHDIDef->caHdiAggrFunc, \
                kr_string_rtrim(stHdiCur.caOutHdiAggrFunc), \
                sizeof(ptShmHDIDef->caHdiAggrFunc));
        strncpy(ptShmHDIDef->caHdiFreeFunc, \
                kr_string_rtrim(stHdiCur.caOutHdiFreeFunc), \
                sizeof(ptShmHDIDef->caHdiFreeFunc));
        ptShmHDIDef->lStatisticsDatasrc = stHdiCur.lOutStatisticsDatasrc;
        ptShmHDIDef->lStatisticsIndex = stHdiCur.lOutStatisticsIndex;
        strncpy(ptShmHDIDef->caStatisticsType, \
                kr_string_rtrim(stHdiCur.caOutStatisticsType), \
                sizeof(ptShmHDIDef->caStatisticsType));
        ptShmHDIDef->lStatisticsValue = stHdiCur.lOutStatisticsValue;
        strncpy(ptShmHDIDef->caStatisticsMethod, \
                kr_string_rtrim(stHdiCur.caOutStatisticsMethod), \
                sizeof(ptShmHDIDef->caStatisticsMethod));

        ptShmHDI->lHDIDefCnt++;
        ptShmHDIDef++;
        iCnt++;
    }
    ptShmHDI->tLastLoadTime = time(NULL);

    iResult = dbsHdiCur(dbsenv, KR_DBCURCLOSE, &stHdiCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsHdiCur Close Error!");
        return -1;
    }
    
    return nRet;
}


int DumpShmHDI(T_KRShmHDI *ptShmHDI, FILE *fp)
{
    long l;
    T_KRShmHDIDef *ptShmHDIDef = &ptShmHDI->stShmHDIDef[0];
    char            caTimeString[80];
    struct tm *ptmNow = localtime(&ptShmHDI->tLastLoadTime);
    strftime(caTimeString, sizeof(caTimeString), "%c", ptmNow);
    
    fprintf(fp, "Dumping HDI...\n");
    fprintf(fp, "Last Load Time[%s]\n", caTimeString);
    for (l=0; l<ptShmHDI->lHDIDefCnt; l++) {
        fprintf(fp, "  lHdiId=[%ld], caHdiName=[%s], caHdiDesc=[%s] \n", 
                ptShmHDIDef->lHdiId, ptShmHDIDef->caHdiName, ptShmHDIDef->caHdiDesc);
        fprintf(fp, "  caHdiType=[%s], caHdiValueType=[%s], caHdiAggrFunc=[%s], caHdiFreeFunc=[%s]\n", 
                ptShmHDIDef->caHdiType, ptShmHDIDef->caHdiValueType, ptShmHDIDef->caHdiAggrFunc, ptShmHDIDef->caHdiFreeFunc);
        fprintf(fp, "  lStatisticsDatasrc=[%ld], lStatisticsIndex=[%ld] \n", 
                ptShmHDIDef->lStatisticsDatasrc, ptShmHDIDef->lStatisticsIndex);
        fprintf(fp, "  caStatisticsType=[%s], lStatisticsValue=[%ld], caStatisticsMethod=[%s] \n", 
                ptShmHDIDef->caStatisticsType, ptShmHDIDef->lStatisticsValue, 
                ptShmHDIDef->caStatisticsMethod);
        ptShmHDIDef++;
    }
    
    return 0;
}
