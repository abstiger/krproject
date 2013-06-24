#include "kr_rule.h"
#include "kr_rule_func.h"
#include "krutils/kr_json.h"

int kr_rule_func(T_KRRule *krrule, T_KRContext *krcontext)
{
    cJSON *rule=NULL, *fired=NULL, *root=NULL;
    if (krrule->ptRuleCalc->result_ind != KR_VALUE_SETED) {
        printf("***rule:[%ld] result_type:[%c] result_value:[%s]***\n", 
                krrule->lRuleId,
                krrule->ptRuleCalc->result_type, "KR_VALUE_UNSET"); 
        return 0;
    }

    if (krrule->ptRuleCalc->result_type == KR_CALCTYPE_BOOLEAN &&
        !krrule->ptRuleCalc->result_value.b) { 
        /* rule not fired */
        return 0;
    }

    /*set context's extra data with json format*/
    if (krcontext->ptExtra == NULL) {
        krcontext->ptExtra = cJSON_CreateObject();
        cJSON_AddItemToObject(krcontext->ptExtra, "fired", \
                              fired=cJSON_CreateArray());
    } else {
        fired = cJSON_GetObjectItem(krcontext->ptExtra, "fired");
    }
    root = (cJSON *)krcontext->ptExtra;
    cJSON_AddItemToArray(fired, rule=cJSON_CreateObject());
    cJSON_AddNumberToObject(rule, "id", krrule->lRuleId);
    cJSON_AddStringToObject(rule, "name", krrule->ptShmRuleDef->caRuleName);
    cJSON_AddNumberToObject(rule, "result", krrule->ptRuleCalc->result_value.b);

    /*
    switch(krrule->ptRuleCalc->result_type) 
    {
        case KR_CALCTYPE_BOOLEAN:   
            printf("***rule:[%ld] result_type:[%c] result_value:[%d]***\n", 
                    krrule->lRuleId,
                    krrule->ptRuleCalc->result_type, 
                    krrule->ptRuleCalc->result_value.b); 
            break;
        case KR_CALCTYPE_INTEGER:   
            printf("***rule:[%ld] result_type:[%c] result_value:[%d]***\n", 
                    krrule->lRuleId,
                    krrule->ptRuleCalc->result_type, 
                    krrule->ptRuleCalc->result_value.i); 
            break;
        case KR_CALCTYPE_DOUBLE:   
            printf("***rule:[%ld] result_type:[%c] result_value:[%lf]***\n", 
                    krrule->lRuleId,
                    krrule->ptRuleCalc->result_type, 
                    krrule->ptRuleCalc->result_value.d); 
            break;
        case KR_CALCTYPE_STRING:   
            printf("***rule:[%ld] result_type:[%c] result_value:[%s]***\n", 
                    krrule->lRuleId,
                    krrule->ptRuleCalc->result_type, 
                    krrule->ptRuleCalc->result_value.s); 
            break; 
        default:
            printf("***rule:[%ld] result_type:[%c] unknown!***\n", 
                    krrule->lRuleId,
                    krrule->ptRuleCalc->result_type);   
            return -1;
            break; 
    }
    */
    
    return 0;
}
