#include "kr_flow_group.h"

/*groups*/
T_KRGroup *kr_group_construct(T_KRParamGroupDef *ptParamGroupDef, 
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue)
{
    T_KRGroup *ptGroup = kr_calloc(sizeof(T_KRGroup));
    if (ptGroup == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptGroup failed!");
        return NULL;
    }
    ptGroup->ptParamGroupDef = ptParamGroupDef;
    ptGroup->lGroupId = ptParamGroupDef->lGroupId;
    ptGroup->ptGroupCalc = kr_calc_construct(
            ptParamGroupDef->caGroupCalcFormat[0], \
            ptParamGroupDef->caGroupCalcString, \
            pfGetType, pfGetValue);
    if (ptGroup->ptGroupCalc == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calc_construct [%s] Failed!", \
                ptParamGroupDef->caGroupCalcString);
        return NULL;
    }

    ptGroup->ptRuleList = kr_rule_list_construct(
            &ptParamGroupDef->stParamRule, \
            pfGetType, pfGetValue);
    if (ptGroup->ptRuleList == NULL) {
        KR_LOG(KR_LOGERROR, "kr_rule_list_construct Failed!");
        return NULL;
    }

    return ptGroup;
}

void kr_group_init(T_KRGroup *ptGroup)
{
    kr_rule_list_init(ptGroup->ptRuleList);
}

void kr_group_destruct(T_KRGroup *ptGroup)
{
    kr_rule_list_destruct(ptGroup->ptRuleList);
    kr_calc_destruct(ptGroup->ptGroupCalc);
    kr_free(ptGroup);
}


T_KRGroupList *kr_group_list_construct(T_KRParamGroup *ptParamGroup, 
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue)
{
    int i = 0;
    T_KRGroup *ptGroup = NULL;
    if (ptParamGroup->lGroupDefCnt <= 0) {
        KR_LOG(KR_LOGERROR, "ptGroupList count [%ld] not valid!", \
               ptParamGroup->lGroupDefCnt);
        return NULL;
    }
    
    T_KRGroupList *ptGroupList = kr_calloc(sizeof(T_KRGroupList));
    if (ptGroupList == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptGroupList failed!");
        return NULL;
    }
    ptGroupList->ptParamGroups = ptParamGroup;
    ptGroupList->lGroupCnt = ptParamGroup->lGroupDefCnt;
    ptGroupList->ptGroupList = kr_list_new();
    kr_list_set_free(ptGroupList->ptGroupList, \
            (KRListFreeFunc )kr_group_destruct);
    
    for (i=0; i<ptGroupList->lGroupCnt; ++i) {
        ptGroup = kr_group_construct(&ptParamGroup->stParamGroupDef[i], 
                pfGetType, pfGetValue);
        if (ptGroup == NULL) {
            KR_LOG(KR_LOGERROR, "kr_group_construct [%d] failed!", i);
            kr_list_destroy(ptGroupList->ptGroupList);
            kr_free(ptGroupList);
            return NULL;
        }
        kr_list_add_tail(ptGroupList->ptGroupList, ptGroup);
    }
    ptGroupList->tConstructTime = ptParamGroup->tLastLoadTime;
    
    return ptGroupList;
}

static void _kr_group_init_func(void *value, void *data)
{
    kr_group_init((T_KRGroup *)value);
}

void kr_group_list_init(T_KRGroupList *ptGroupList)
{
    kr_list_foreach(ptGroupList->ptGroupList, _kr_group_init_func, NULL);
}

void kr_group_list_destruct(T_KRGroupList *ptGroupList)
{
    kr_list_destroy(ptGroupList->ptGroupList);
    kr_free(ptGroupList);
}

T_KRGroup *kr_group_lookup(T_KRGroupList *ptGroupList, int id)
{
    T_KRListNode *node = ptGroupList->ptGroupList->head;
    while(node) {
        T_KRGroup *ptGroup = kr_list_value(node);
        if (ptGroup->lGroupId == id) {
            return ptGroup;
        }
        node = node->next;
    }
    return NULL;
}
