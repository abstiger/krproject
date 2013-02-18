#include "kr_rule.h"
#include "kr_rule_func.h"

int kr_rule_func(T_KRRule *krrule, T_KRContext *krcontext)
{
    if (krrule->ptRuleCalc->result_ind != KR_VALUE_SETED) {
        printf("***rule:[%ld] result_type:[%c] result_value:[%s]***\n", 
                krrule->lRuleId,
                krrule->ptRuleCalc->result_type, "KR_VALUE_UNSET"); 
        return 0;
    }

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
    
    return 0;
}
