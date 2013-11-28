#include "krdata/kr_data.h"

extern int kr_rule_func(T_KRRule *krrule, T_KRContext *krcontext);

static int kr_rule_detect(T_KRRule *krrule, void *krcontext)
{
    krrule->bViolated = FALSE;
    kr_hashtable_remove_all(krrule->ptRelated);

    /*check datasrc first*/
    T_KRTable *krtable = ((T_KRContext *)krcontext)->ptCurrRec->ptTable;
    if (krrule->ptShmRuleDef->lRuleDatasrc > 0 &&
            (krtable->iTableId != krrule->ptShmRuleDef->lRuleDatasrc)) {
        KR_LOG(KR_LOGDEBUG, "inputdatasrc [%d] isn't ruledatasrc[%ld] !", 
                krtable->iTableId, krrule->ptShmRuleDef->lRuleDatasrc);
        return 0;
    }

    /*calculate rule string*/
    if (kr_calc_eval(krrule->ptRuleCalc, krcontext) != 0) {
        KR_LOG(KR_LOGERROR, "kr_calc_eval rule[%ld] failed[%s]!", \
                krrule->lRuleId, krrule->ptRuleCalc->err_msg);
        return -1;
    }
    
    /*handle detect result and write reponse*/
    krrule->RuleFunc = krrule->RuleFunc?krrule->RuleFunc:kr_rule_func;
    if (krrule->RuleFunc(krrule, krcontext) != 0) {
        KR_LOG(KR_LOGERROR, "run Rule[%ld] Func[%s] failed!", 
                krrule->lRuleId, krrule->ptShmRuleDef->caRuleFunc);
        return -1;
    }

    return 0;
}



int kr_rule_list_detect(T_KRRuleList *ptRuleList, T_KRContext *krcontext)
{
    /* if no rules to be detected, return asap */
    if (ptRuleList == NULL || ptRuleList->lRuleCnt == 0) {
        KR_LOG(KR_LOGDEBUG, "no rules to be detected!");
        return 0;
    }

    /* detecting context check*/
    if (krcontext == NULL || krcontext->ptCurrRec == NULL) {
        KR_LOG(KR_LOGERROR, "detecting context not set correctly!");
        return -1;
    }
    
    /*traversal the rule list*/
    T_KRRule *krrule = NULL;
    T_KRListNode *node = ptRuleList->ptRuleList->head;
    while(node)
    {
        krrule = kr_list_value(node);
        if (kr_rule_detect(krrule, krcontext) != 0) {
            KR_LOG(KR_LOGERROR, "kr_rule_detect[%ld] failed!", krrule->lRuleId);
            return -1;
        }
        node = node->next;
    }
    
    return 0;
}

