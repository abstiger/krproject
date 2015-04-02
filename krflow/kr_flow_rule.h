#ifndef __KR_RULE_H__
#define __KR_RULE_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"

typedef int  (*KRRuleFunc)(void *p1, void *p2);

typedef struct _kr_rule_t
{
    T_KRParamRuleDef      *ptParamRuleDef;
    long                  lRuleId;
    long                  lRuleWeight;
    T_KRCalc              *ptRuleCalc;
    KRRuleFunc            RuleFunc;

    kr_bool               bViolated;
    T_KRHashTable         *ptRelated;
}T_KRRule;

typedef struct _kr_rule_list_t
{
    T_KRParamRule         *ptParamRules;
    long                  lRuleCnt;
    T_KRList              *ptRuleList;
    time_t                tConstructTime;

    long                  lFiredRules;
    long                  lFiredWeights;
}T_KRRuleList;


/*rules*/
T_KRRule *kr_rule_construct(T_KRParamRuleDef *ptParamRuleDef, 
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue);
void kr_rule_destruct(T_KRRule *ptRule);
void kr_rule_init(T_KRRule *ptRule);
cJSON *kr_rule_info(T_KRRule *ptRule);

T_KRRuleList *kr_rule_list_construct(T_KRParamRule *ptParamRule, 
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue);
void kr_rule_list_destruct(T_KRRuleList *ptRuleList);
void kr_rule_list_init(T_KRRuleList *ptRuleList);
T_KRRule *kr_rule_lookup(T_KRRuleList *ptRuleList, int id);

#endif /* __KR_RULE_H__ */
