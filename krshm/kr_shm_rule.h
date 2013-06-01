#ifndef __KR_SHM_RULE_H__
#define __KR_SHM_RULE_H__

#include <stdio.h>
#include <time.h>
#include "dbs/dbs_basopr.h"

#define N_MAX_RULE_NUM 20

typedef struct _kr_shm_rule_def_t
{
    long        lRuleId;
    char        caRuleName[30+1];
    char        caRuleDesc[100+1];
    long        lRuleDatasrc;
    char        caRuleString[500+1];
    char        caRuleType[1+1];
    char        caRuleFunc[50+1];
    long        lRuleWeight;
}T_KRShmRuleDef;


typedef struct _kr_shm_rule_t
{
    long             lRuleDefCnt;
    T_KRShmRuleDef   stShmRuleDef[N_MAX_RULE_NUM];
    time_t           tLastLoadTime;           /*share memory last loaded time*/
}T_KRShmRule;


extern int LoadShmRule(T_DbsEnv *dbsenv, T_KRShmRule *ptShmRule, long lGroupId);
extern int DumpShmRule(T_KRShmRule *ptShmRule, FILE *fp);

#endif  /*__KR_SHM_RULE_H__*/
