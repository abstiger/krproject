#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"
#include "dbs/dbs/rule_cur.h"
#include "kr_param_rule.h"

int kr_param_rule_load(T_DbsEnv *ptDbsEnv, T_KRParamRule *ptParamRule, long lGroupId)
{
    int nRet = 0;
    int iResult = 0;
    int iCnt = 0;    
    T_RuleCur stRuleCur = {0};
    T_KRParamRuleDef *ptParamRuleDef = &ptParamRule->stParamRuleDef[0];
    
    stRuleCur.lInGroupId = lGroupId;
    iResult = dbsRuleCur(ptDbsEnv, KR_DBCUROPEN, &stRuleCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsRuleCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsRuleCur(ptDbsEnv, KR_DBCURFETCH, &stRuleCur);
        if (iResult != KR_DBNOTFOUND && 
                iResult != KR_DBOK && iResult != KR_DBOKWITHINFO) {
            KR_LOG(KR_LOGERROR, "dbsRuleCur Fetch Error[%d]![%s]:[%s]",
                    iResult, ptDbsEnv->sqlstate, ptDbsEnv->sqlerrmsg);
            nRet = -1;
            break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Load [%d] Rules Totally!", iCnt);
            break;
        }
        
        if (iCnt >= N_MAX_RULE_NUM) {
            KR_LOG(KR_LOGERROR, "Only [%d] Rules Allowed!", N_MAX_RULE_NUM);
            nRet = -1;
            break;
        }
        
        memset(ptParamRuleDef, 0x00, sizeof(T_KRParamRuleDef));
        ptParamRuleDef->lRuleId = stRuleCur.lOutRuleId;
        strncpy(ptParamRuleDef->caRuleName, \
                kr_string_rtrim(stRuleCur.caOutRuleName), \
                sizeof(ptParamRuleDef->caRuleName));
        strncpy(ptParamRuleDef->caRuleDesc, \
                kr_string_rtrim(stRuleCur.caOutRuleDesc), \
                sizeof(ptParamRuleDef->caRuleDesc));
        ptParamRuleDef->lRuleDatasrc = stRuleCur.lOutRuleDatasrc;
        strncpy(ptParamRuleDef->caRuleCalcFormat, \
                kr_string_rtrim(stRuleCur.caOutRuleCalcFormat), \
                sizeof(ptParamRuleDef->caRuleCalcFormat));
        strncpy(ptParamRuleDef->caRuleCalcString, \
                kr_string_rtrim(stRuleCur.caOutRuleCalcString), \
                sizeof(ptParamRuleDef->caRuleCalcString));
        strncpy(ptParamRuleDef->caRuleType, \
                kr_string_rtrim(stRuleCur.caOutRuleType), \
                sizeof(ptParamRuleDef->caRuleType));        
        strncpy(ptParamRuleDef->caRuleFunc, \
                kr_string_rtrim(stRuleCur.caOutRuleFunc), \
                sizeof(ptParamRuleDef->caRuleFunc));        
        ptParamRuleDef->lRuleWeight = stRuleCur.lOutRuleWeight;
        
        ptParamRule->lRuleDefCnt++;
        ptParamRuleDef++;
        iCnt++;
    }
    ptParamRule->tLastLoadTime = time(NULL);

    iResult = dbsRuleCur(ptDbsEnv, KR_DBCURCLOSE, &stRuleCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsRuleCur Close Error!");
        return -1;
    }
    
    return nRet;
}


int kr_param_rule_dump(T_KRParamRule *ptParamRule, FILE *fp)
{
    long l;
    T_KRParamRuleDef *ptParamRuleDef = &ptParamRule->stParamRuleDef[0];
    char            caTimeString[80];
    struct tm *ptmNow = localtime(&ptParamRule->tLastLoadTime);
    strftime(caTimeString, sizeof(caTimeString), "%c", ptmNow);
    
    fprintf(fp, "Dumping Rule...\n");
    fprintf(fp, "Last Load Time[%s]\n", caTimeString);
    for (l=0; l<ptParamRule->lRuleDefCnt; l++) {
        fprintf(fp, "  lRuleId=[%ld], caRuleName=[%s], caRuleDesc=[%s],", 
                ptParamRuleDef->lRuleId, ptParamRuleDef->caRuleName, 
                ptParamRuleDef->caRuleDesc);
        fprintf(fp, " lRuleDatasrc=[%ld], caRuleCalcFormat=[%s], caRuleCalcString=[%s],",
                ptParamRuleDef->lRuleDatasrc, ptParamRuleDef->caRuleCalcFormat, ptParamRuleDef->caRuleCalcString);
        fprintf(fp, " caRuleType=[%s], caRuleFunc=[%s],",
                ptParamRuleDef->caRuleType, ptParamRuleDef->caRuleFunc);
        fprintf(fp, " lRuleWeight=[%ld] \n ", ptParamRuleDef->lRuleWeight);
        ptParamRuleDef++;
    }
    
    return 0;
}

