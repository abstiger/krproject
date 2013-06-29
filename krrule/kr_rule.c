#include "kr_rule.h"
#include "kr_aid.h"
#include "kr_sdi.h"
#include "kr_ddi.h"
#include "kr_hdi.h"

E_KRFieldType kr_rule_get_type(char kind, int id, void *param)
{
    T_KRContext *ptContext = (T_KRContext *)param;
    switch(kind) 
    {
        case KR_CALCKIND_SET:
            return kr_aid_get_type(ptContext->ptDbs, id);
            break;
        case KR_CALCKIND_CID:
            return kr_get_field_type(ptContext->ptCurrRec, id);
            break;
        case KR_CALCKIND_FID:
            return kr_get_field_type(ptContext->ptRecord, id);
            break;
        case KR_CALCKIND_SID:
            return kr_sdi_get_type(id, ptContext);
            break;
        case KR_CALCKIND_DID: 
            return kr_ddi_get_type(id, ptContext);
            break;
        case KR_CALCKIND_HID: 
            return kr_hdi_get_type(id, ptContext);
            break;
        default:
            return KR_FIELDTYPE_UNKNOWN;
    }
}


void *kr_rule_get_value(char kind, int id, void *param)
{
    T_KRContext *ptContext = (T_KRContext *)param;
    switch(kind) 
    {
        case KR_CALCKIND_SET:
            return kr_aid_get_value(ptContext->ptDbs, id);
            break;
        case KR_CALCKIND_CID:
            return kr_get_field_value(ptContext->ptCurrRec, id);
            break;
        case KR_CALCKIND_FID:
            return kr_get_field_value(ptContext->ptRecord, id);
            break;
        case KR_CALCKIND_SID:
            return kr_sdi_get_value(id, ptContext);
            break;
        case KR_CALCKIND_DID: 
            return kr_ddi_get_value(id, ptContext);
            break;
        case KR_CALCKIND_HID: 
            return kr_hdi_get_value(id, ptContext);
            break;
        default:
            return NULL;
    }
}


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
        rule_def->caRuleCalcString, kr_rule_get_type, kr_rule_get_value);
    krrule->RuleFunc = (KRRuleFunc )kr_rule_func;
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
    return krrule;
}


int kr_rule_detect(T_KRRule *krrule, void *krcontext)
{
    krrule->bViolated = FALSE;

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
        KR_LOG(KR_LOGERROR, "kr_calc_eval rule[%ld] failed!", krrule->lRuleId);
        return -1;
    }
    
    /*handle detect result and write reponse*/
    if (krrule->RuleFunc != NULL) {
        if (krrule->RuleFunc(krrule, krcontext) != 0) {
            KR_LOG(KR_LOGERROR, "run Rule[%ld] Func[%s] failed!", 
                    krrule->lRuleId, krrule->ptShmRuleDef->caRuleFunc);
            return -1;
        }
    }

    return 0;
}


void kr_rule_destruct(void *rule)
{
    T_KRRule *krrule = (T_KRRule *)rule;
    kr_calc_destruct(krrule->ptRuleCalc);
    kr_free(krrule);
}


T_KRRuleGroup *kr_rule_group_construct(T_KRShmRule *shm_rule, T_KRModule *rulemodule)
{
    int i = 0;
    T_KRRule *krrule = NULL;
    if (shm_rule->lRuleDefCnt <= 0) {
        KR_LOG(KR_LOGERROR, "rulegroup count [%ld] not valid!", \
               shm_rule->lRuleDefCnt);
        return NULL;
    }
    
    T_KRRuleGroup *krrulegroup = kr_calloc(sizeof(T_KRRuleGroup));
    if (krrulegroup == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krrulegroup failed!");
        return NULL;
    }
    krrulegroup->ptShmRules = shm_rule;
    krrulegroup->lRuleCnt = shm_rule->lRuleDefCnt;
    krrulegroup->ptRuleList = kr_list_new();
    kr_list_set_free(krrulegroup->ptRuleList, kr_rule_destruct);
    
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


int kr_rule_group_detect(T_KRContext *krcontext)
{
    /* detecting context check*/
    if (krcontext == NULL || krcontext->ptDym->rulegroup == NULL ||
        krcontext->ptCurrRec == NULL) {
        KR_LOG(KR_LOGERROR, "detecting context not set correctly!");
        return -1;
    }

    /* if no rules to be detected, return asap */
    T_KRRuleGroup *ptRuleGroup = krcontext->ptDym->rulegroup;
    if (ptRuleGroup == NULL || ptRuleGroup->lRuleCnt == 0) {
        KR_LOG(KR_LOGDEBUG, "no rules to be detected!");
        return 0;
    }
    
    /*traversal the rule group*/
    T_KRRule *krrule = NULL;
    T_KRListNode *node = ptRuleGroup->ptRuleList->head;
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


void kr_rule_group_destruct(T_KRRuleGroup *krrulegroup)
{
    kr_list_destroy(krrulegroup->ptRuleList);
    kr_free(krrulegroup);
}
