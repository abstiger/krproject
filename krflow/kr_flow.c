#include "kr_flow.h"

static int kr_rule_detect(T_KRRule *ptRule, T_KRData *ptData)
{
    KR_LOG(KR_LOGDEBUG, "kr_rule_detect rule[%ld]... ", ptRule->lRuleId);

    /*initialize first*/
    kr_rule_init(ptRule);

    /*check datasrc first*/
    T_KRTable *krtable = ((T_KRData *)ptData)->ptCurrRec->ptTable;
    if (krtable->iTableId != ptRule->ptParamRuleDef->lRuleDatasrc) {
        KR_LOG(KR_LOGDEBUG, "inputdatasrc [%d] isn't ruledatasrc[%ld] !", 
                krtable->iTableId, ptRule->ptParamRuleDef->lRuleDatasrc);
        return 0;
    }

    /*calculate rule string*/
    T_KRCalc *krcalc = ptRule->ptRuleCalc;
    if (kr_calc_eval(krcalc, ptData) != 0) {
        KR_LOG(KR_LOGERROR, "kr_calc_eval rule[%ld] failed[%s]!", \
                ptRule->lRuleId, kr_calc_errmsg(krcalc));
        return -1;
    }

    /*rule fired*/
    if (kr_calc_ind(krcalc) == KR_VALUE_SETED &&
        kr_calc_type(krcalc) == KR_TYPE_BOOL &&
        kr_calc_value(krcalc)->b) {
        KR_LOG(KR_LOGDEBUG, "rule [%ld] fired!", ptRule->lRuleId);
        ptRule->bViolated = TRUE;
        return 1;
    }

    return 0;
}


static int kr_rule_list_detect(T_KRRuleList *ptRuleList, T_KRData *ptData)
{
    /*initialize first*/
    kr_rule_list_init(ptRuleList);

    /* if no rules to be detected, return asap */
    if (ptRuleList == NULL || ptRuleList->lRuleCnt == 0) {
        KR_LOG(KR_LOGDEBUG, "no rules to be detected!");
        return 0;
    }

    /* detecting context check*/
    if (ptData == NULL || ptData->ptCurrRec == NULL) {
        KR_LOG(KR_LOGERROR, "detecting context not set correctly!");
        return -1;
    }
    
    /*traversal the rule list*/
    T_KRListNode *node = ptRuleList->ptRuleList->head;
    while(node)
    {
        /*rule detect*/
        T_KRRule *ptRule = kr_list_value(node);
        int ret = kr_rule_detect(ptRule, ptData);
        if (ret == 1) {
            /* rule fired, add related variables*/
            ptRuleList->lFiredRules++;
            ptRuleList->lFiredWeights+=ptRule->lRuleWeight;
        } else if (ret != 0) {
            KR_LOG(KR_LOGERROR, "kr_rule_detect[%ld] failed!", ptRule->lRuleId);
            return -1;
        }

        node = node->next;
    }
    
    return 0;
}


static int kr_group_match(T_KRGroup *ptGroup, T_KRData *ptData)
{
    /*calculate group string*/
    if (kr_calc_eval(ptGroup->ptGroupCalc, ptData) != 0) {
        KR_LOG(KR_LOGERROR, "kr_calc_eval group [%ld] failed!", 
                ptGroup->lGroupId);
        return -1;
    }
    
    /*handle calculator result:matched*/
    if (kr_calc_ind(ptGroup->ptGroupCalc) == KR_VALUE_SETED &&
        kr_calc_type(ptGroup->ptGroupCalc) == KR_TYPE_BOOL &&
        kr_calc_value(ptGroup->ptGroupCalc)->b) {
        KR_LOG(KR_LOGDEBUG, "kr_group_match [%ld]", ptGroup->lGroupId);
        return 1;
    }

    /* not matched */
    return 0;
}


int kr_flow_detect(T_KRFlow *ptFlow, T_KRRecord *ptCurrRec)
{
    ptFlow->ptData->ptCurrRec = ptCurrRec;
    
    /* if no groups to be detected, return asap */
    T_KRGroupList *ptGroupList = ptFlow->ptGroupList;
    if (ptGroupList->lGroupCnt == 0) {
        KR_LOG(KR_LOGDEBUG, "no groups to be detected!");
        return 0;
    }
    
    /*traversal the group list*/
    T_KRListNode *node = ptGroupList->ptGroupList->head;
    while(node) {
        T_KRGroup *ptGroup = kr_list_value(node);
        int ret = kr_group_match(ptGroup, ptFlow->ptData); 
        if (ret == 1) {
            ptFlow->ptRoutedGroup = ptGroup;
            /* rule list detect */
            if (kr_rule_list_detect(ptGroup->ptRuleList, ptFlow->ptData) != 0) {
                KR_LOG(KR_LOGERROR, "kr_rule_list_detect failed!");
                return -1;
            }
            break;
        } else if (ret != 0) {
            KR_LOG(KR_LOGERROR, "kr_group_match failed!");
            return -1;
        }
        node = node->next;
    }
    
    return 0;
}

