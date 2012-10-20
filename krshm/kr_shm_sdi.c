#include "kr_shm_sdi.h"
#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"
#include "dbs/sdi_cur.h"

int LoadShmSDI(T_KRShmSDI *ptShmSDI)
{
    int nRet = 0;
    int iResult = 0;
    int iCnt = 0;    
    T_SdiCur stSdiCur = {0};
    T_KRShmSDIDef *ptShmSDIDef = &ptShmSDI->stShmSDIDef[0];
    
    iResult = dbsSdiCur(KR_DBCUROPEN, &stSdiCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSdiCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsSdiCur(KR_DBCURFETCH, &stSdiCur);
        if (iResult != KR_DBNOTFOUND && iResult != KR_DBOK) {
            KR_LOG(KR_LOGERROR, "dbsSdiCur Fetch Error!");
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
        
        memset(ptShmSDIDef, 0x00, sizeof(T_KRShmSDIDef));
        ptShmSDIDef->lSdiId = stSdiCur.lOutSdiId;
        strncpy(ptShmSDIDef->caSdiName, \
                kr_string_rtrim(stSdiCur.caOutSdiName), \
                sizeof(ptShmSDIDef->caSdiName));
        strncpy(ptShmSDIDef->caSdiDesc, \
                kr_string_rtrim(stSdiCur.caOutSdiDesc), \
                sizeof(ptShmSDIDef->caSdiDesc));
        strncpy(ptShmSDIDef->caSdiType, \
                kr_string_rtrim(stSdiCur.caOutSdiType), \
                sizeof(ptShmSDIDef->caSdiType));
        strncpy(ptShmSDIDef->caSdiValueType, \
                kr_string_rtrim(stSdiCur.caOutSdiValueType), \
                sizeof(ptShmSDIDef->caSdiValueType));
        strncpy(ptShmSDIDef->caSdiAggrFunc, \
                kr_string_rtrim(stSdiCur.caOutSdiAggrFunc), \
                sizeof(ptShmSDIDef->caSdiAggrFunc));
        strncpy(ptShmSDIDef->caSdiFreeFunc, \
                kr_string_rtrim(stSdiCur.caOutSdiFreeFunc), \
                sizeof(ptShmSDIDef->caSdiFreeFunc));
        ptShmSDIDef->lStatisticsDatasrc = stSdiCur.lOutStatisticsDatasrc;
        ptShmSDIDef->lStatisticsIndex = stSdiCur.lOutStatisticsIndex;
        ptShmSDIDef->lStatisticsField = stSdiCur.lOutStatisticsField;
        ptShmSDIDef->lStatisticsLocation = stSdiCur.lOutStatisticsLocation;
        strncpy(ptShmSDIDef->caLocationProperty, \
                kr_string_rtrim(stSdiCur.caOutLocationProperty), \
                sizeof(ptShmSDIDef->caLocationProperty));
        strncpy(ptShmSDIDef->caSdiFilterString, \
                kr_string_rtrim(stSdiCur.caOutSdiFilterString), \
                sizeof(ptShmSDIDef->caSdiFilterString));

        ptShmSDI->lSDIDefCnt++;
        ptShmSDIDef++;
        iCnt++;
    }
    ptShmSDI->tLastLoadTime = time(NULL);

    iResult = dbsSdiCur(KR_DBCURCLOSE, &stSdiCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSdiCur Close Error!");
        return -1;
    }
    
    return nRet;
}


int DumpShmSDI(T_KRShmSDI *ptShmSDI, FILE *fp)
{
    long l;
    T_KRShmSDIDef *ptShmSDIDef = &ptShmSDI->stShmSDIDef[0];
    char            caTimeString[80];
    struct tm *ptmNow = localtime(&ptShmSDI->tLastLoadTime);
    strftime(caTimeString, sizeof(caTimeString), "%c", ptmNow);
    
    fprintf(fp, "Dumping SDI...\n");
    fprintf(fp, "Last Load Time[%s]\n", caTimeString);
    for (l=0; l<ptShmSDI->lSDIDefCnt; l++) {
        fprintf(fp, "  lSdiId=[%ld], caSdiName=[%s], caSdiDesc=[%s] \n", 
                ptShmSDIDef->lSdiId, ptShmSDIDef->caSdiName, ptShmSDIDef->caSdiDesc);
        fprintf(fp, "  caSdiType=[%s], caSdiValueType=[%s],", 
                ptShmSDIDef->caSdiType, ptShmSDIDef->caSdiValueType);
        fprintf(fp, "  caSdiAggrFunc=[%s], caSdiFreeFunc=[%s] \n", 
                ptShmSDIDef->caSdiAggrFunc, ptShmSDIDef->caSdiFreeFunc);
        fprintf(fp, "  lStatisticsDatasrc=[%ld], lStatisticsIndex=[%ld], lStatisticsField=[%ld] \n", 
                ptShmSDIDef->lStatisticsDatasrc, ptShmSDIDef->lStatisticsIndex, ptShmSDIDef->lStatisticsField);
        fprintf(fp, "  lStatisticsLocation=[%ld], caLocationProperty=[%s] \n", 
                ptShmSDIDef->lStatisticsLocation, ptShmSDIDef->caLocationProperty);
        fprintf(fp, "  caSdiFilterString=[%s] \n", ptShmSDIDef->caSdiFilterString);
        ptShmSDIDef++;
    }
    
    return 0;
}
