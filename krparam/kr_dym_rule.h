#ifndef __KR_RULE_H__
#define __KR_RULE_H__

#include "krutils/kr_utils.h"
#include "krcalc/kr_calc.h"
#include "kr_shm_rule.h"

typedef int  (*KRRuleFunc)(void *p1, void *p2);

typedef struct _kr_rule_t
{
    T_KRShmRuleDef        *ptShmRuleDef;
    long                  lRuleId;
    long                  lRuleWeight;
    T_KRCalc              *ptRuleCalc;
    KRRuleFunc            RuleFunc;

    kr_bool               bViolated;
    T_KRHashTable         *ptRelated;
}T_KRRule;

typedef struct _kr_rule_list_t
{
    T_KRShmRule           *ptShmRules;
    long                  lRuleCnt;
    T_KRList              *ptRuleList;
    time_t                tConstructTime;

    long                  lFiredRules;
    long                  lFiredWeights;
}T_KRRuleList;


/*rules*/
T_KRRule *kr_rule_construct(T_KRShmRuleDef *rule_def, 
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func);
void kr_rule_destruct(T_KRRule *krrule);
void kr_rule_init(T_KRRule *krrule);
cJSON *kr_rule_info(T_KRRule *krrule);

T_KRRuleList *kr_rule_list_construct(T_KRShmRule *rule_list_def, 
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func);
void kr_rule_list_destruct(T_KRRuleList *krrulegroup);
void kr_rule_list_init(T_KRRuleList *krrulegroup);
T_KRRule *kr_rule_lookup(T_KRRuleList *krrulegroup, int id);

#endif /* __KR_RULE_H__ */
