#include "kr_flow_rule.h"

/*rules*/
T_KRRule *kr_rule_construct(T_KRParamRuleDef *ptParamRuleDef, 
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue)
{
    T_KRRule *ptRule = kr_calloc(sizeof(T_KRRule));
    if (ptRule == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptRule failed!");
        return NULL;
    }
    ptRule->ptParamRuleDef = ptParamRuleDef;
    ptRule->lRuleId = ptParamRuleDef->lRuleId;
    ptRule->lRuleWeight = ptParamRuleDef->lRuleWeight;
    ptRule->ptRuleCalc = kr_calc_construct(ptParamRuleDef->caRuleCalcFormat[0], \
            ptParamRuleDef->caRuleCalcString, pfGetType, pfGetValue);

    ptRule->bViolated = FALSE;
    ptRule->ptRelated = kr_hashtable_new(kr_pointer_hash, kr_pointer_equal);

    return ptRule;
}

void kr_rule_init(T_KRRule *ptRule)
{
    ptRule->bViolated = FALSE;
    kr_hashtable_remove_all(ptRule->ptRelated);
}

void kr_rule_destruct(T_KRRule *ptRule)
{
    kr_hashtable_destroy(ptRule->ptRelated);
    kr_calc_destruct(ptRule->ptRuleCalc);
    kr_free(ptRule);
}

T_KRRuleList *kr_rule_list_construct(T_KRParamRule *shm_rule, 
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue)
{
    int i = 0;
    T_KRRule *ptRule = NULL;
    if (shm_rule->lRuleDefCnt <= 0) {
        KR_LOG(KR_LOGERROR, "rulegroup count [%ld] not valid!", \
               shm_rule->lRuleDefCnt);
        return NULL;
    }
    
    T_KRRuleList *ptRuleList = kr_calloc(sizeof(T_KRRuleList));
    if (ptRuleList == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptRuleList failed!");
        return NULL;
    }
    ptRuleList->ptParamRules = shm_rule;
    ptRuleList->lRuleCnt = shm_rule->lRuleDefCnt;
    ptRuleList->ptRuleList = kr_list_new();
    kr_list_set_free(ptRuleList->ptRuleList, \
            (KRListFreeFunc )kr_rule_destruct);
    
    for (i=0; i<ptRuleList->lRuleCnt; ++i) {
        ptRule = kr_rule_construct(&shm_rule->stParamRuleDef[i], 
                pfGetType, pfGetValue);
        if (ptRule == NULL) {
            KR_LOG(KR_LOGERROR, "kr_rule_construct [%d] failed!", i);
            kr_list_destroy(ptRuleList->ptRuleList);
            kr_free(ptRuleList);
            return NULL;
        }
        kr_list_add_tail(ptRuleList->ptRuleList, ptRule);
    }
    ptRuleList->tConstructTime = shm_rule->tLastLoadTime;

    /*set while rule detecting*/
    ptRuleList->lFiredRules = 0;
    ptRuleList->lFiredWeights = 0;
    
    return ptRuleList;
}

static void _kr_rule_init_func(void *value, void *data)
{
    kr_rule_init((T_KRRule *)value);
}

void kr_rule_list_init(T_KRRuleList *ptRuleList)
{
    ptRuleList->lFiredRules = 0;
    ptRuleList->lFiredWeights = 0;
    kr_list_foreach(ptRuleList->ptRuleList, _kr_rule_init_func, NULL);
}

void kr_rule_list_destruct(T_KRRuleList *ptRuleList)
{
    kr_list_destroy(ptRuleList->ptRuleList);
    kr_free(ptRuleList);
}

T_KRRule *kr_rule_lookup(T_KRRuleList *ptRuleList, int id)
{
    T_KRListNode *node = ptRuleList->ptRuleList->head;
    while(node) {
        T_KRRule *ptRule = kr_list_value(node);
        if (ptRule->lRuleId == id) {
            return ptRule;
        }
        node = node->next;
    }
    return NULL;
}
