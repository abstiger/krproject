#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"
#include "dbs/dbs/group_cur.h"
#include "kr_param_group.h"

int kr_param_group_load(T_DbsEnv *ptDbsEnv, T_KRParamGroup *ptParamGroup)
{
    int nRet = 0;
    int iResult = 0;
    int iCnt = 0;    
    T_GroupCur stGroupCur = {0};
    T_KRParamGroupDef *ptParamGroupDef = &ptParamGroup->stParamGroupDef[0];
    
    iResult = dbsGroupCur(ptDbsEnv, KR_DBCUROPEN, &stGroupCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsGroupCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsGroupCur(ptDbsEnv, KR_DBCURFETCH, &stGroupCur);
        if (iResult != KR_DBNOTFOUND && 
                iResult != KR_DBOK && iResult != KR_DBOKWITHINFO) {
            KR_LOG(KR_LOGERROR, "dbsGroupCur Fetch Error[%d]![%s]:[%s]",  
                    iResult, ptDbsEnv->sqlstate, ptDbsEnv->sqlerrmsg);
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
        
        memset(ptParamGroupDef, 0x00, sizeof(T_KRParamGroupDef));
        ptParamGroupDef->lGroupId = stGroupCur.lOutGroupId;
        strncpy(ptParamGroupDef->caGroupName, \
                kr_string_rtrim(stGroupCur.caOutGroupName), \
                sizeof(ptParamGroupDef->caGroupName));
        strncpy(ptParamGroupDef->caGroupDesc, \
                kr_string_rtrim(stGroupCur.caOutGroupDesc), \
                sizeof(ptParamGroupDef->caGroupDesc));
        strncpy(ptParamGroupDef->caGroupCalcFormat, \
                kr_string_rtrim(stGroupCur.caOutGroupCalcFormat), \
                sizeof(ptParamGroupDef->caGroupCalcFormat));
        strncpy(ptParamGroupDef->caGroupCalcString, \
                kr_string_rtrim(stGroupCur.caOutGroupCalcString), \
                sizeof(ptParamGroupDef->caGroupCalcString));
        strncpy(ptParamGroupDef->caGroupFunc, \
                kr_string_rtrim(stGroupCur.caOutGroupFunc), \
                sizeof(ptParamGroupDef->caGroupFunc));
        ptParamGroupDef->lGroupBlockLeft = stGroupCur.lOutGroupBlockLeft;
        ptParamGroupDef->lGroupBlockRight = stGroupCur.lOutGroupBlockRight;
        ptParamGroupDef->lGroupAlertLeft = stGroupCur.lOutGroupAlertLeft;
        ptParamGroupDef->lGroupAlertRight = stGroupCur.lOutGroupAlertRight;
        /*load rules of this group*/
        if (kr_param_rule_load(ptDbsEnv, &ptParamGroupDef->stParamRule, \
                    ptParamGroupDef->lGroupId) != 0) {
            KR_LOG(KR_LOGERROR, "kr_param_rule_load [%ld] failed!", \
                    ptParamGroupDef->lGroupId);
            nRet = -1;
            break;
        }
        
        ptParamGroup->lGroupDefCnt++;
        ptParamGroupDef++;
        iCnt++;
    }
    ptParamGroup->tLastLoadTime = time(NULL);

    iResult = dbsGroupCur(ptDbsEnv, KR_DBCURCLOSE, &stGroupCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsGroupCur Close Error!");
        return -1;
    }
    
    return nRet;
}


int kr_param_group_dump(T_KRParamGroup *ptParamGroup, FILE *fp)
{
    long l;
    T_KRParamGroupDef *ptParamGroupDef = &ptParamGroup->stParamGroupDef[0];
    char            caTimeString[80];
    struct tm *ptmNow = localtime(&ptParamGroup->tLastLoadTime);
    strftime(caTimeString, sizeof(caTimeString), "%c", ptmNow);
    
    fprintf(fp, "Dumping Group...\n");
    fprintf(fp, "Last Load Time[%s]\n", caTimeString);
    for (l=0; l<ptParamGroup->lGroupDefCnt; l++) {
        fprintf(fp, "  lGroupId=[%ld], caGroupName=[%s], caGroupDesc=[%s],", 
                ptParamGroupDef->lGroupId, ptParamGroupDef->caGroupName, 
                ptParamGroupDef->caGroupDesc);
        fprintf(fp, " caGroupCalcFormat=[%s], caGroupCalcString=[%s], caGroupFunc=[%s] \n", 
                 ptParamGroupDef->caGroupCalcFormat, ptParamGroupDef->caGroupCalcString, ptParamGroupDef->caGroupFunc);
        /*dump rules of this group*/
        kr_param_rule_dump(&ptParamGroupDef->stParamRule, fp);
        ptParamGroupDef++;
    }
    
    return 0;
}



