#include "kr_flow_api.h"
#include "krparam/kr_param_api.h"


cJSON *kr_rule_info(T_KRRule *ptRule)
{ 
    cJSON *rule = cJSON_CreateObject();
    cJSON *def = kr_param_rule_info(ptRule->ptParamRuleDef);
    cJSON_AddItemToObject(rule, "def", def);
    return rule;
}

cJSON *kr_group_info(T_KRGroup *ptGroup)
{ 
    cJSON *group = cJSON_CreateObject();

    cJSON *def = kr_param_group_info(ptGroup->ptParamGroupDef);
    cJSON_AddItemToObject(group, "def", def);

    cJSON *rules = cJSON_CreateArray();
    T_KRListNode *node = ptGroup->ptRuleList->ptRuleList->head;
    while(node) {
        T_KRRule *ptRule = kr_list_value(node);
        cJSON_AddItemToArray(rules, kr_rule_info(ptRule));
        node = node->next;
    }
    cJSON_AddItemToObject(group, "rules", rules);

    return group;
}


