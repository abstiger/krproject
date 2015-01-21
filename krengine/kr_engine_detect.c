#include "krdata/kr_data.h"


static int kr_rule_detect(T_KRRule *krrule, void *krcontext)
{
    KR_LOG(KR_LOGDEBUG, "kr_rule_detect rule[%ld]... !", krrule->lRuleId);

    /*initialize first*/
    kr_rule_init(krrule);

    /*check datasrc first*/
    T_KRTable *krtable = ((T_KRContext *)krcontext)->ptCurrRec->ptTable;
    if (krtable->iTableId != krrule->ptShmRuleDef->lRuleDatasrc) {
        KR_LOG(KR_LOGDEBUG, "inputdatasrc [%d] isn't ruledatasrc[%ld] !", 
                krtable->iTableId, krrule->ptShmRuleDef->lRuleDatasrc);
        return 0;
    }

    /*calculate rule string*/
    T_KRCalc *krcalc = krrule->ptRuleCalc;
    if (kr_calc_eval(krcalc, krcontext) != 0) {
        KR_LOG(KR_LOGERROR, "kr_calc_eval rule[%ld] failed[%s]!", \
                krrule->lRuleId, kr_calc_errmsg(krcalc));
        return -1;
    }

    /*rule fired*/
    if (kr_calc_ind(krcalc) == KR_VALUE_SETED &&
        kr_calc_type(krcalc) == KR_TYPE_BOOL &&
        kr_calc_value(krcalc)->b) {
        KR_LOG(KR_LOGDEBUG, "rule [%ld] fired!", krrule->lRuleId);
        krrule->bViolated = TRUE;
        return 1;
    }

    return 0;
}


static int kr_rule_list_detect(T_KRRuleList *ptRuleList, T_KRContext *krcontext)
{
    /*initialize first*/
    kr_rule_list_init(ptRuleList);

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
    T_KRListNode *node = ptRuleList->ptRuleList->head;
    while(node)
    {
        /*rule detect*/
        T_KRRule *krrule = kr_list_value(node);
        int ret = kr_rule_detect(krrule, krcontext);
        if (ret == 1) {
            /* rule fired, add related variables*/
            ptRuleList->lFiredRules++;
            ptRuleList->lFiredWeights+=krrule->lRuleWeight;
        } else if (ret != 0) {
            KR_LOG(KR_LOGERROR, "kr_rule_detect[%ld] failed!", krrule->lRuleId);
            return -1;
        }

        node = node->next;
    }
    
    return 0;
}


static int kr_group_match(T_KRGroup *krgroup, void *krcontext)
{
    /*calculate group string*/
    if (kr_calc_eval(krgroup->ptGroupCalc, krcontext) != 0) {
        KR_LOG(KR_LOGERROR, "kr_calc_eval group [%ld] failed!", 
                krgroup->lGroupId);
        return -1;
    }
    
    /*handle calculator result:matched*/
    if (kr_calc_ind(krgroup->ptGroupCalc) == KR_VALUE_SETED &&
        kr_calc_type(krgroup->ptGroupCalc) == KR_TYPE_BOOL &&
        kr_calc_value(krgroup->ptGroupCalc)->b) {
        KR_LOG(KR_LOGDEBUG, "kr_group_match [%ld]!", krgroup->lGroupId);
        return 1;
    }

    /* not matched */
    return 0;
}


int kr_engine_detect(T_KRContext *krcontext)
{
    /* detecting context check*/
    if (krcontext == NULL || krcontext->ptCurrRec == NULL || 
            krcontext->ptDym->grouplist == NULL) {
        KR_LOG(KR_LOGERROR, "detecting context not set correctly!");
        return -1;
    }

    /* if no groups to be detected, return asap */
    T_KRGroupList *ptGroupList = krcontext->ptDym->grouplist;
    if (ptGroupList->lGroupCnt == 0) {
        KR_LOG(KR_LOGDEBUG, "no groups to be detected!");
        return 0;
    }
    
    /*traversal the group list*/
    T_KRListNode *node = ptGroupList->ptGroupList->head;
    while(node) {
        T_KRGroup *krgroup = kr_list_value(node);
        int ret = kr_group_match(krgroup, krcontext); 
        if (ret == 1) {
            krcontext->ptGroup = krgroup;
            /* rule list detect */
            if (kr_rule_list_detect(krgroup->ptRuleList, krcontext) != 0) {
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

