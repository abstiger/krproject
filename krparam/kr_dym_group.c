#include "kr_dym_group.h"

/*groups*/
T_KRGroup *
kr_group_construct(T_KRShmGroupDef *group_def, T_KRModule *groupmodule)
{
    T_KRGroup *krgroup = kr_calloc(sizeof(T_KRGroup));
    if (krgroup == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krgroup failed!");
        return NULL;
    }
    krgroup->ptShmGroupDef = group_def;
    krgroup->lGroupId = group_def->lGroupId;
    krgroup->ptGroupCalc = kr_calc_construct(group_def->caGroupCalcFormat[0], \
            group_def->caGroupCalcString);
    if (krgroup->ptGroupCalc == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calc_construct [%s] Failed!", \
                group_def->caGroupCalcString);
        return NULL;
    }
    if (group_def->caGroupFunc[0] != '\0') {
        krgroup->GroupFunc = (KRGroupFunc )kr_module_symbol(groupmodule,
                group_def->caGroupFunc);
        if (krgroup->GroupFunc == NULL) {
            KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", \
                    group_def->caGroupFunc);
            return NULL;
        }
    }
    krgroup->ptRuleList = \
        kr_rule_list_construct(&group_def->stShmRule, groupmodule);
    if (krgroup->ptRuleList == NULL) {
        KR_LOG(KR_LOGERROR, "kr_rule_list_construct Failed!");
        return NULL;
    }

    return krgroup;
}


void kr_group_destruct(void *group)
{
    T_KRGroup *krgroup = (T_KRGroup *)group;
    kr_calc_destruct(krgroup->ptGroupCalc);
    kr_free(krgroup);
}


T_KRGroupList *
kr_group_list_construct(T_KRShmGroup *shm_group, T_KRModule *groupmodule)
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


void kr_group_list_destruct(T_KRGroupList *krgrouplist)
{
    kr_list_destroy(krgrouplist->ptGroupList);
    kr_free(krgrouplist);
}
