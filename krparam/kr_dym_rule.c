#include "kr_dym_rule.h"

/*rules*/
T_KRRule *kr_rule_construct(T_KRShmRuleDef *rule_def, T_KRModule *rulemodule)
{
    T_KRRule *krrule = kr_calloc(sizeof(T_KRRule));
    if (krrule == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krrule failed!");
        return NULL;
    }
    krrule->ptShmRuleDef = rule_def;
    krrule->lRuleId = rule_def->lRuleId;
    krrule->ptRuleCalc = kr_calc_construct(rule_def->caRuleCalcFormat[0], \
            rule_def->caRuleCalcString);
    if (rule_def->caRuleFunc[0] != '\0') {
        krrule->RuleFunc = (KRRuleFunc )kr_module_symbol(rulemodule, 
                rule_def->caRuleFunc);
        if (krrule->RuleFunc == NULL) {
            KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", \
                    rule_def->caRuleFunc);
            return NULL;
        }
    }
    krrule->bViolated = FALSE;
    krrule->ptRelated = kr_hashtable_new(kr_pointer_hash, kr_pointer_equal);

    return krrule;
}


void kr_rule_destruct(T_KRRule *krrule)
{
    kr_hashtable_destroy(krrule->ptRelated);
    kr_calc_destruct(krrule->ptRuleCalc);
    kr_free(krrule);
}


T_KRRuleList *kr_rule_list_construct(T_KRShmRule *shm_rule, T_KRModule *rulemodule)
{
    int i = 0;
    T_KRRule *krrule = NULL;
    if (shm_rule->lRuleDefCnt <= 0) {
        KR_LOG(KR_LOGERROR, "rulegroup count [%ld] not valid!", \
               shm_rule->lRuleDefCnt);
        return NULL;
    }
    
    T_KRRuleList *krrulegroup = kr_calloc(sizeof(T_KRRuleList));
    if (krrulegroup == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krrulegroup failed!");
        return NULL;
    }
    krrulegroup->ptShmRules = shm_rule;
    krrulegroup->lRuleCnt = shm_rule->lRuleDefCnt;
    krrulegroup->ptRuleList = kr_list_new();
    kr_list_set_free(krrulegroup->ptRuleList, (KRFreeFunc )kr_rule_destruct);
    
    for (i=0; i<krrulegroup->lRuleCnt; ++i) {
        krrule = kr_rule_construct(&shm_rule->stShmRuleDef[i], rulemodule);
        if (krrule == NULL) {
            KR_LOG(KR_LOGERROR, "kr_rule_construct [%d] failed!", i);
            kr_list_destroy(krrulegroup->ptRuleList);
            kr_free(krrulegroup);
            return NULL;
        }
        kr_list_add_tail(krrulegroup->ptRuleList, krrule);
    }
    krrulegroup->tConstructTime = shm_rule->tLastLoadTime;
    
    return krrulegroup;
}


void kr_rule_list_destruct(T_KRRuleList *krrulegroup)
{
    kr_list_destroy(krrulegroup->ptRuleList);
    kr_free(krrulegroup);
}
