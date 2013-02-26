#include "kr_group.h"
#include "kr_rule.h"
#include "kr_aid.h"
#include "kr_sdi.h"
#include "kr_ddi.h"
#include "kr_hdi.h"

/*groups*/
T_KRGroup *kr_group_construct(T_KRShmGroupDef *group_def, T_KRModule *groupmodule)
{
    T_KRGroup *krgroup = kr_calloc(sizeof(T_KRGroup));
    if (krgroup == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krgroup failed!");
        return NULL;
    }
    krgroup->ptShmGroupDef = group_def;
    krgroup->lGroupId = group_def->lGroupId;
    krgroup->ptGroupCalc = kr_calc_construct(KR_CALCBEHOOF_RULE, \
        group_def->caGroupString, kr_rule_get_type, kr_rule_get_value);
    if (krgroup->ptGroupCalc == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calc_construct [%s] Failed!", \
                group_def->caGroupString);
        return NULL;
    }
    krgroup->GroupFunc = (KRGroupFunc )kr_group_func;
    if (group_def->caGroupFunc[0] != '\0') {
        krgroup->GroupFunc = (KRGroupFunc )kr_module_symbol(groupmodule,
                group_def->caGroupFunc);
        if (krgroup->GroupFunc == NULL) {
            KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", \
                    group_def->caGroupFunc);
            return NULL;
        }
    }
    krgroup->ptRuleGroup = \
        kr_rule_group_construct(&group_def->stShmRule, groupmodule);
    if (krgroup->ptRuleGroup == NULL) {
        KR_LOG(KR_LOGERROR, "kr_rule_group_construct Failed!");
        return NULL;
    }

    return krgroup;
}


int kr_group_route(T_KRGroup *krgroup, void *krcontext)
{
    /*calculate group string*/
    if (kr_calc_eval(krgroup->ptGroupCalc, krcontext) != 0) {
        KR_LOG(KR_LOGERROR, "kr_calc_eval group [%ld] failed!", 
                krgroup->lGroupId);
        return -1;
    }
    
    /*handle calculator result:matched*/
    if (krgroup->ptGroupCalc->result_ind == KR_VALUE_SETED &&
            krgroup->ptGroupCalc->result_type == KR_CALCTYPE_BOOLEAN &&
            krgroup->ptGroupCalc->result_value.b != 0) {
        return 1;
    }

    return 0;
}


void kr_group_destruct(void *group)
{
    T_KRGroup *krgroup = (T_KRGroup *)group;
    kr_calc_destruct(krgroup->ptGroupCalc);
    kr_free(krgroup);
}


T_KRGroupList *kr_group_list_construct(T_KRShmGroup *shm_group, T_KRModule *groupmodule)
{
    int i = 0;
    T_KRGroup *krgroup = NULL;
    if (shm_group->lGroupDefCnt <= 0) {
        KR_LOG(KR_LOGERROR, "grouplist count [%ld] not valid!", \
               shm_group->lGroupDefCnt);
        return NULL;
    }
    
    T_KRGroupList *krgrouplist = kr_calloc(sizeof(T_KRGroupList));
    if (krgrouplist == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krgrouplist failed!");
        return NULL;
    }
    krgrouplist->ptShmGroups = shm_group;
    krgrouplist->lGroupCnt = shm_group->lGroupDefCnt;
    krgrouplist->ptGroupList = kr_list_new();
    kr_list_set_free(krgrouplist->ptGroupList, kr_group_destruct);
    
    for (i=0; i<krgrouplist->lGroupCnt; ++i) {
        krgroup = kr_group_construct(&shm_group->stShmGroupDef[i], groupmodule);
        if (krgroup == NULL) {
            KR_LOG(KR_LOGERROR, "kr_group_construct [%d] failed!", i);
            kr_list_destroy(krgrouplist->ptGroupList);
            kr_free(krgrouplist);
            return NULL;
        }
        kr_list_add_tail(krgrouplist->ptGroupList, krgroup);
    }
    krgrouplist->tConstructTime = shm_group->tLastLoadTime;
    
    return krgrouplist;
}


int kr_group_list_detect(T_KRContext *krcontext)
{
    int ret = 0;
    /* detecting context check*/
    if (krcontext == NULL || krcontext->ptDym->grouplist == NULL ||
        krcontext->ptCurrRec == NULL) {
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
    T_KRGroup *krgroup = NULL;
    T_KRListNode *node = ptGroupList->ptGroupList->head;
    while(node)
    {
        krgroup = kr_list_value(node);
        ret = kr_group_route(krgroup, krcontext); 
        if (ret == 1) {
            /*set context rulegroup*/
            krcontext->ptDym->rulegroup = krgroup->ptRuleGroup;
            break;
        } else if (ret != 0) {
            KR_LOG(KR_LOGERROR, "kr_group_route failed!");
            return -1;
        }
        node = node->next;
    }

    /* if no group routed, return asap */
    if (krcontext->ptDym->rulegroup == NULL) {
        KR_LOG(KR_LOGDEBUG, "no group routed!");
        return 0;
    }

    /*run rule group detect*/
    if (kr_rule_group_detect(krcontext) != 0) {
        KR_LOG(KR_LOGERROR, "kr_rule_group_detect failed!\n");
        return -1;
    }

    /*run the group func*/
    if (krgroup->GroupFunc != NULL) {
        if (krgroup->GroupFunc(krgroup, krcontext) != 0) {
            KR_LOG(KR_LOGERROR, "run Group[%ld] Func[%s] failed!", 
                    krgroup->lGroupId, krgroup->ptShmGroupDef->caGroupFunc);
            return -1;
        }
    }
    
    return 0;
}


void kr_group_list_destruct(T_KRGroupList *krgrouplist)
{
    kr_list_destroy(krgrouplist->ptGroupList);
    kr_free(krgrouplist);
}
