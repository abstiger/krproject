#include "kr_rule.h"
#include "kr_aid.h"
#include "kr_sdi.h"
#include "kr_ddi.h"
#include "kr_hdi.h"

E_KRFieldType kr_rule_get_type(char *id, void *param)
{
    char cIdType;
    char caIdValue[20] = {0};
    int  iIdValue;
    cIdType = id[0];
    strncpy(caIdValue, &id[2], sizeof(caIdValue));
    iIdValue = atoi(caIdValue);
    
    T_KRContext *ptContext = (T_KRContext *)param;
    switch(cIdType) 
    {
        case 'A':
            return kr_aid_get_type(iIdValue);
            break;
        case 'C':
            return kr_get_field_type(ptContext->ptCurrRec, iIdValue);
            break;
        case 'F':
            return kr_get_field_type(ptContext->ptRecord, iIdValue);
            break;
        case 'S':
            return kr_sdi_get_type(iIdValue, ptContext);
            break;
        case 'D': 
            return kr_ddi_get_type(iIdValue, ptContext);
            break;
        case 'H': 
            return kr_hdi_get_type(iIdValue, ptContext);
            break;
        default:
            return KR_FIELDTYPE_UNKNOWN;
    }
}


void *kr_rule_get_value(char *id, void *param)
{
    char cIdType;
    char caIdValue[20] = {0};
    int  iIdValue;
    cIdType = id[0];
    strncpy(caIdValue, &id[2], sizeof(caIdValue));
    iIdValue = atoi(caIdValue);
    
    T_KRContext *ptContext = (T_KRContext *)param;
    switch(id[0]) {
    case 'A':
        return kr_aid_get_value(iIdValue);
        break;
    case 'C':
        return kr_get_field_value(ptContext->ptCurrRec, iIdValue);
        break;
    case 'F':
        return kr_get_field_value(ptContext->ptRecord, iIdValue);
        break;
    case 'S':
        return kr_sdi_get_value(iIdValue, ptContext);
        break;
    case 'D': 
        return kr_ddi_get_value(iIdValue, ptContext);
        break;
    case 'H': 
        return kr_hdi_get_value(iIdValue, ptContext);
        break;
    default:
        return NULL;
    }
}


/*rules*/
T_KRRule *kr_rule_construct(T_KRShmRuleDef *rule_def)
{
    T_KRRule *krrule = kr_calloc(sizeof(T_KRRule));
    if (krrule == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krrule failed!");
		return NULL;
    }
    krrule->ptShmRuleDef = rule_def;
    krrule->lRuleId = rule_def->lRuleId;
    krrule->ptRuleCalc = kr_calc_construct(KR_CALCBEHOOF_RULE, \
        rule_def->caRuleString, kr_rule_get_type, kr_rule_get_value);
    
    krrule->bViolated = FALSE;
    return krrule;
}


int kr_rule_detect(T_KRRule *krrule, void *krcontext)
{
    int nRet = 0;
    nRet = kr_calc_eval(krrule->ptRuleCalc, krcontext);
    if (nRet != 0) {
        KR_LOG(KR_LOGERROR, "kr_calc_eval failed!");
		return -1;
    }
    
    if (krrule->ptRuleCalc->result_ind != KR_VALUE_SETED) {
        printf("***result_type:[%c] result_value:[%s]***\n", 
          krrule->ptRuleCalc->result_type, "KR_VALUE_UNSET"); 
        return 0;
    }
    
    switch(krrule->ptRuleCalc->result_type) {
    case KR_CALCTYPE_BOOLEAN:   
        printf("***result_type:[%c] result_value:[%d]***\n", 
          krrule->ptRuleCalc->result_type, krrule->ptRuleCalc->result_value.b); 
        break;
    case KR_CALCTYPE_INTEGER:   
        printf("***result_type:[%c] result_value:[%d]***\n", 
          krrule->ptRuleCalc->result_type, krrule->ptRuleCalc->result_value.i); 
        break;
    case KR_CALCTYPE_DOUBLE:   
        printf("***result_type:[%c] result_value:[%lf]***\n", 
          krrule->ptRuleCalc->result_type, krrule->ptRuleCalc->result_value.d); 
        break;
    case KR_CALCTYPE_STRING:   
        printf("***result_type:[%c] result_value:[%s]***\n", 
          krrule->ptRuleCalc->result_type, krrule->ptRuleCalc->result_value.s); 
        break; 
    default:
        printf("***result_type:[%c] unknown!***\n", 
          krrule->ptRuleCalc->result_type);   
        break; 
    }
    
    return 0;
}


void kr_rule_destruct(void *rule)
{
    T_KRRule *krrule = (T_KRRule *)rule;
    kr_calc_destruct(krrule->ptRuleCalc);
    kr_free(krrule);
}


T_KRRuleGroup *kr_rule_group_construct(T_KRShmRule *shm_rule)
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
        krrule = kr_rule_construct(&shm_rule->stShmRuleDef[i]);
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
    if (ptRuleGroup->lRuleCnt == 0) {
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
