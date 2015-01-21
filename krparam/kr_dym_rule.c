#include "kr_dym_rule.h"

/*rules*/
T_KRRule *kr_rule_construct(T_KRShmRuleDef *rule_def, 
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func)
{
    T_KRRule *krrule = kr_calloc(sizeof(T_KRRule));
    if (krrule == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krrule failed!");
        return NULL;
    }
    krrule->ptShmRuleDef = rule_def;
    krrule->lRuleId = rule_def->lRuleId;
    krrule->lRuleWeight = rule_def->lRuleWeight;
    krrule->ptRuleCalc = kr_calc_construct(rule_def->caRuleCalcFormat[0], \
            rule_def->caRuleCalcString, get_type_func, get_value_func);

    krrule->bViolated = FALSE;
    krrule->ptRelated = kr_hashtable_new(kr_pointer_hash, kr_pointer_equal);

    return krrule;
}

void kr_rule_init(T_KRRule *krrule)
{
    krrule->bViolated = FALSE;
    kr_hashtable_remove_all(krrule->ptRelated);
}

void kr_rule_destruct(T_KRRule *krrule)
{
    kr_hashtable_destroy(krrule->ptRelated);
    kr_calc_destruct(krrule->ptRuleCalc);
    kr_free(krrule);
}

cJSON *kr_rule_info(T_KRRule *krrule)
{ 
    cJSON *rule = cJSON_CreateObject();
    cJSON *def = kr_shm_rule_info(krrule->ptShmRuleDef);
    cJSON_AddItemToObject(rule, "def", def);
    return rule;
}


T_KRRuleList *kr_rule_list_construct(T_KRShmRule *shm_rule, 
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func)
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
    kr_list_set_free(krrulegroup->ptRuleList, \
            (KRListFreeFunc )kr_rule_destruct);
    
    for (i=0; i<krrulegroup->lRuleCnt; ++i) {
        krrule = kr_rule_construct(&shm_rule->stShmRuleDef[i], 
                get_type_func, get_value_func);
        if (krrule == NULL) {
            KR_LOG(KR_LOGERROR, "kr_rule_construct [%d] failed!", i);
            kr_list_destroy(krrulegroup->ptRuleList);
            kr_free(krrulegroup);
            return NULL;
        }
        kr_list_add_tail(krrulegroup->ptRuleList, krrule);
    }
    krrulegroup->tConstructTime = shm_rule->tLastLoadTime;

    /*set while rule detecting*/
    krrulegroup->lFiredRules = 0;
    krrulegroup->lFiredWeights = 0;
    
    return krrulegroup;
}

static void _kr_rule_init_func(void *value, void *data)
{
    kr_rule_init((T_KRRule *)value);
}

void kr_rule_list_init(T_KRRuleList *krrulegroup)
{
    krrulegroup->lFiredRules = 0;
    krrulegroup->lFiredWeights = 0;
    kr_list_foreach(krrulegroup->ptRuleList, _kr_rule_init_func, NULL);
}

void kr_rule_list_destruct(T_KRRuleList *krrulegroup)
{
    kr_list_destroy(krrulegroup->ptRuleList);
    kr_free(krrulegroup);
}

T_KRRule *kr_rule_lookup(T_KRRuleList *krrulegroup, int id)
{
    T_KRListNode *node = krrulegroup->ptRuleList->head;
    while(node) {
        T_KRRule *krrule = kr_list_value(node);
        if (krrule->lRuleId == id) {
            return krrule;
        }
        node = node->next;
    }
    return NULL;
}
