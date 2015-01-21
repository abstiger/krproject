#include "krutils/kr_utils.h"
#include "dbs/dbs/ddi_cur.h"
#include "kr_shm_ddi.h"

int kr_shm_ddi_load(T_DbsEnv *dbsenv, T_KRShmDDI *ptShmDDI)
{
    int nRet = 0;
    int iResult = 0;
    int iCnt = 0;    
    T_DdiCur stDdiCur = {0};
    T_KRShmDDIDef *ptShmDDIDef = &ptShmDDI->stShmDDIDef[0];
    
    iResult = dbsDdiCur(dbsenv, KR_DBCUROPEN, &stDdiCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDdiCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsDdiCur(dbsenv, KR_DBCURFETCH, &stDdiCur);
        if (iResult != KR_DBNOTFOUND && 
                iResult != KR_DBOK && iResult != KR_DBOKWITHINFO) {
            KR_LOG(KR_LOGERROR, "dbsDdiCur Fetch Error![%s]:[%s]", 
                    dbsenv->sqlstate, dbsenv->sqlerrmsg);
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
        
        memset(ptShmDDIDef, 0x00, sizeof(T_KRShmDDIDef));
        ptShmDDIDef->lDdiId = stDdiCur.lOutDdiId;
        strncpy(ptShmDDIDef->caDdiName, \
                kr_string_rtrim(stDdiCur.caOutDdiName), \
                sizeof(ptShmDDIDef->caDdiName));
        strncpy(ptShmDDIDef->caDdiDesc, \
                kr_string_rtrim(stDdiCur.caOutDdiDesc), \
                sizeof(ptShmDDIDef->caDdiDesc));
        strncpy(ptShmDDIDef->caDdiType, \
                kr_string_rtrim(stDdiCur.caOutDdiType), \
                sizeof(ptShmDDIDef->caDdiType));
        strncpy(ptShmDDIDef->caDdiValueType, \
                kr_string_rtrim(stDdiCur.caOutDdiValueType), \
                sizeof(ptShmDDIDef->caDdiValueType));
        strncpy(ptShmDDIDef->caDdiAggrFunc, \
                kr_string_rtrim(stDdiCur.caOutDdiAggrFunc), \
                sizeof(ptShmDDIDef->caDdiAggrFunc));
        strncpy(ptShmDDIDef->caDdiFreeFunc, \
                kr_string_rtrim(stDdiCur.caOutDdiFreeFunc), \
                sizeof(ptShmDDIDef->caDdiFreeFunc));
        ptShmDDIDef->lStatisticsDatasrc = stDdiCur.lOutStatisticsDatasrc;
        ptShmDDIDef->lStatisticsIndex = stDdiCur.lOutStatisticsIndex;
        ptShmDDIDef->lStatisticsField = stDdiCur.lOutStatisticsField;
        strncpy(ptShmDDIDef->caStatisticsType, \
                kr_string_rtrim(stDdiCur.caOutStatisticsType), \
                sizeof(ptShmDDIDef->caStatisticsType));
        ptShmDDIDef->lStatisticsValue = stDdiCur.lOutStatisticsValue;
        ptShmDDIDef->lStatisticsCount = stDdiCur.lOutStatisticsCount;
        strncpy(ptShmDDIDef->caStatisticsMethod, \
                kr_string_rtrim(stDdiCur.caOutStatisticsMethod), \
                sizeof(ptShmDDIDef->caStatisticsMethod));
        strncpy(ptShmDDIDef->caDdiFilterFormat, \
                kr_string_rtrim(stDdiCur.caOutDdiFilterFormat), \
                sizeof(ptShmDDIDef->caDdiFilterFormat));
        strncpy(ptShmDDIDef->caDdiFilterString, \
                kr_string_rtrim(stDdiCur.caOutDdiFilterString), \
                sizeof(ptShmDDIDef->caDdiFilterString));

        ptShmDDI->lDDIDefCnt++;
        ptShmDDIDef++;
        iCnt++;
    }
    ptShmDDI->tLastLoadTime = time(NULL);

    iResult = dbsDdiCur(dbsenv, KR_DBCURCLOSE, &stDdiCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDdiCur Close Error!");
        return -1;
    }
    
    return nRet;
}


int kr_shm_ddi_dump(T_KRShmDDI *ptShmDDI, FILE *fp)
{
    long l;
    T_KRShmDDIDef *ptShmDDIDef = &ptShmDDI->stShmDDIDef[0];
    char            caTimeString[80];
    struct tm *ptmNow = localtime(&ptShmDDI->tLastLoadTime);
    strftime(caTimeString, sizeof(caTimeString), "%c", ptmNow);
    
    fprintf(fp, "Dumping DDI...\n");
    fprintf(fp, "Last Load Time[%s]\n", caTimeString);
    for (l=0; l<ptShmDDI->lDDIDefCnt; l++) {
        fprintf(fp, "  lDdiId=[%ld], caDdiName=[%s], caDdiDesc=[%s] \n", 
                ptShmDDIDef->lDdiId, ptShmDDIDef->caDdiName, ptShmDDIDef->caDdiDesc);
        fprintf(fp, "  caDdiType=[%s], caDdiValueType=[%s], caDdiAggrFunc=[%s], caDdiFreeFunc=[%s]\n", 
                ptShmDDIDef->caDdiType, ptShmDDIDef->caDdiValueType, ptShmDDIDef->caDdiAggrFunc, ptShmDDIDef->caDdiFreeFunc);
        fprintf(fp, "  lStatisticsDatasrc=[%ld], lStatisticsIndex=[%ld], lStatisticsField=[%ld] \n", 
                ptShmDDIDef->lStatisticsDatasrc, ptShmDDIDef->lStatisticsIndex, ptShmDDIDef->lStatisticsField);
        fprintf(fp, "  caStatisticsType=[%s], lStatisticsValue=[%ld] \n", 
                ptShmDDIDef->caStatisticsType, ptShmDDIDef->lStatisticsValue);        
        fprintf(fp, "  lStatisticsCount=[%ld], caStatisticsMethod=[%s] \n", 
                ptShmDDIDef->lStatisticsCount, ptShmDDIDef->caStatisticsMethod);
        fprintf(fp, "  caDdiFilterFormat=[%s], caDdiFilterString=[%s] \n", ptShmDDIDef->caDdiFilterFormat, ptShmDDIDef->caDdiFilterString);        
        ptShmDDIDef++;
    }
    
    return 0;
}

cJSON *kr_shm_ddi_info(T_KRShmDDIDef *ptShmDDIDef)
{
    cJSON *ddi = cJSON_CreateObject();
    cJSON_AddNumberToObject(ddi, "id", ptShmDDIDef->lDdiId);
    cJSON_AddStringToObject(ddi, "name", ptShmDDIDef->caDdiName);
    cJSON_AddStringToObject(ddi, "desc", ptShmDDIDef->caDdiDesc);
    cJSON_AddStringToObject(ddi, "type", ptShmDDIDef->caDdiType);
    cJSON_AddStringToObject(ddi, "value_type", ptShmDDIDef->caDdiValueType);
    cJSON_AddStringToObject(ddi, "aggr_func", ptShmDDIDef->caDdiAggrFunc);
    cJSON_AddNumberToObject(ddi, "datasrc", ptShmDDIDef->lStatisticsDatasrc);
    cJSON_AddNumberToObject(ddi, "index", ptShmDDIDef->lStatisticsIndex);
    cJSON_AddNumberToObject(ddi, "field", ptShmDDIDef->lStatisticsField);
    cJSON_AddStringToObject(ddi, "statictics_type", ptShmDDIDef->caStatisticsType);
    cJSON_AddNumberToObject(ddi, "statictics_value", ptShmDDIDef->lStatisticsValue);
    cJSON_AddNumberToObject(ddi, "statictics_count", ptShmDDIDef->lStatisticsCount);
    cJSON_AddStringToObject(ddi, "statictics_method", ptShmDDIDef->caStatisticsMethod);
    cJSON_AddStringToObject(ddi, "filter_format", ptShmDDIDef->caDdiFilterFormat);
    cJSON_AddStringToObject(ddi, "filter_string", ptShmDDIDef->caDdiFilterString);
    return ddi;
}
