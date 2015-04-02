#ifndef __KR_PARAM_RULE_H__
#define __KR_PARAM_RULE_H__

#include <stdio.h>
#include <time.h>
#include "dbs/dbs_basopr.h"

#define N_MAX_RULE_NUM 20

typedef struct _kr_param_rule_def_t
{
    long        lRuleId;
    char        caRuleName[30+1];
    char        caRuleDesc[100+1];
    long        lRuleDatasrc;
    char        caRuleCalcFormat[1+1];
    char        caRuleCalcString[500+1];
    char        caRuleType[1+1];
    char        caRuleFunc[50+1];
    long        lRuleWeight;
}T_KRParamRuleDef;


typedef struct _kr_param_rule_t
{
    long               lRuleDefCnt;
    T_KRParamRuleDef   stParamRuleDef[N_MAX_RULE_NUM];
    time_t             tLastLoadTime;           /*share memory last loaded time*/
}T_KRParamRule;


int kr_param_rule_load(T_DbsEnv *ptDbsEnv, T_KRParamRule *ptParamRule, long lGroupId);
int kr_param_rule_dump(T_KRParamRule *ptParamRule, FILE *fp);

#endif  /*__KR_PARAM_RULE_H__*/
