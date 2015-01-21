#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"
#include "dbs/dbs/group_cur.h"
#include "kr_shm_group.h"

int kr_shm_group_load(T_DbsEnv *dbsenv, T_KRShmGroup *ptShmGroup)
{
    int nRet = 0;
    int iResult = 0;
    int iCnt = 0;    
    T_GroupCur stGroupCur = {0};
    T_KRShmGroupDef *ptShmGroupDef = &ptShmGroup->stShmGroupDef[0];
    
    iResult = dbsGroupCur(dbsenv, KR_DBCUROPEN, &stGroupCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsGroupCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsGroupCur(dbsenv, KR_DBCURFETCH, &stGroupCur);
        if (iResult != KR_DBNOTFOUND && 
                iResult != KR_DBOK && iResult != KR_DBOKWITHINFO) {
            KR_LOG(KR_LOGERROR, "dbsGroupCur Fetch Error[%d]![%s]:[%s]",  
                    iResult, dbsenv->sqlstate, dbsenv->sqlerrmsg);
            nRet = -1;
            break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Load [%d] Groups Totally!", iCnt);
            break;
        }
        
        if (iCnt >= N_MAX_GROUP_NUM) {
            KR_LOG(KR_LOGERROR, "Only [%d] Groups Allowed!", N_MAX_GROUP_NUM);
            nRet = -1;
            break;
        }
        
        memset(ptShmGroupDef, 0x00, sizeof(T_KRShmGroupDef));
        ptShmGroupDef->lGroupId = stGroupCur.lOutGroupId;
        strncpy(ptShmGroupDef->caGroupName, \
                kr_string_rtrim(stGroupCur.caOutGroupName), \
                sizeof(ptShmGroupDef->caGroupName));
        strncpy(ptShmGroupDef->caGroupDesc, \
                kr_string_rtrim(stGroupCur.caOutGroupDesc), \
                sizeof(ptShmGroupDef->caGroupDesc));
        strncpy(ptShmGroupDef->caGroupCalcFormat, \
                kr_string_rtrim(stGroupCur.caOutGroupCalcFormat), \
                sizeof(ptShmGroupDef->caGroupCalcFormat));
        strncpy(ptShmGroupDef->caGroupCalcString, \
                kr_string_rtrim(stGroupCur.caOutGroupCalcString), \
                sizeof(ptShmGroupDef->caGroupCalcString));
        strncpy(ptShmGroupDef->caGroupFunc, \
                kr_string_rtrim(stGroupCur.caOutGroupFunc), \
                sizeof(ptShmGroupDef->caGroupFunc));
        ptShmGroupDef->lGroupBlockLeft = stGroupCur.lOutGroupBlockLeft;
        ptShmGroupDef->lGroupBlockRight = stGroupCur.lOutGroupBlockRight;
        ptShmGroupDef->lGroupAlertLeft = stGroupCur.lOutGroupAlertLeft;
        ptShmGroupDef->lGroupAlertRight = stGroupCur.lOutGroupAlertRight;
        /*load rules of this group*/
        if (kr_shm_rule_load(dbsenv, &ptShmGroupDef->stShmRule, \
                    ptShmGroupDef->lGroupId) != 0) {
            KR_LOG(KR_LOGERROR, "kr_shm_rule_load [%ld] failed!", \
                    ptShmGroupDef->lGroupId);
            nRet = -1;
            break;
        }
        
        ptShmGroup->lGroupDefCnt++;
        ptShmGroupDef++;
        iCnt++;
    }
    ptShmGroup->tLastLoadTime = time(NULL);

    iResult = dbsGroupCur(dbsenv, KR_DBCURCLOSE, &stGroupCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsGroupCur Close Error!");
        return -1;
    }
    
    return nRet;
}


int kr_shm_group_dump(T_KRShmGroup *ptShmGroup, FILE *fp)
{
    long l;
    T_KRShmGroupDef *ptShmGroupDef = &ptShmGroup->stShmGroupDef[0];
    char            caTimeString[80];
    struct tm *ptmNow = localtime(&ptShmGroup->tLastLoadTime);
    strftime(caTimeString, sizeof(caTimeString), "%c", ptmNow);
    
    fprintf(fp, "Dumping Group...\n");
    fprintf(fp, "Last Load Time[%s]\n", caTimeString);
    for (l=0; l<ptShmGroup->lGroupDefCnt; l++) {
        fprintf(fp, "  lGroupId=[%ld], caGroupName=[%s], caGroupDesc=[%s],", 
                ptShmGroupDef->lGroupId, ptShmGroupDef->caGroupName, 
                ptShmGroupDef->caGroupDesc);
        fprintf(fp, " caGroupCalcFormat=[%s], caGroupCalcString=[%s], caGroupFunc=[%s] \n", 
                 ptShmGroupDef->caGroupCalcFormat, ptShmGroupDef->caGroupCalcString, ptShmGroupDef->caGroupFunc);
        /*dump rules of this group*/
        kr_shm_rule_dump(&ptShmGroupDef->stShmRule, fp);
        ptShmGroupDef++;
    }
    
    return 0;
}

cJSON *kr_shm_group_info(T_KRShmGroupDef *ptShmGroupDef)
{
    cJSON *group = cJSON_CreateObject();
    cJSON_AddNumberToObject(group, "id", ptShmGroupDef->lGroupId);
    cJSON_AddStringToObject(group, "name", ptShmGroupDef->caGroupName);
    cJSON_AddStringToObject(group, "desc", ptShmGroupDef->caGroupDesc);
    cJSON_AddStringToObject(group, "func", ptShmGroupDef->caGroupFunc);
    cJSON_AddStringToObject(group, "calc_format", ptShmGroupDef->caGroupCalcFormat);
    cJSON_AddStringToObject(group, "calc_string", ptShmGroupDef->caGroupCalcString);
    return group;
}

