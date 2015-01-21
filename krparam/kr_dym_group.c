#include "kr_dym_group.h"

/*groups*/
T_KRGroup *kr_group_construct(T_KRShmGroupDef *group_def, 
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func)
{
    T_KRGroup *krgroup = kr_calloc(sizeof(T_KRGroup));
    if (krgroup == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krgroup failed!");
        return NULL;
    }
    krgroup->ptShmGroupDef = group_def;
    krgroup->lGroupId = group_def->lGroupId;
    krgroup->ptGroupCalc = kr_calc_construct(group_def->caGroupCalcFormat[0], \
            group_def->caGroupCalcString, get_type_func, get_value_func);
    if (krgroup->ptGroupCalc == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calc_construct [%s] Failed!", \
                group_def->caGroupCalcString);
        return NULL;
    }

    krgroup->ptRuleList = kr_rule_list_construct(&group_def->stShmRule, 
            get_type_func, get_value_func);
    if (krgroup->ptRuleList == NULL) {
        KR_LOG(KR_LOGERROR, "kr_rule_list_construct Failed!");
        return NULL;
    }

    return krgroup;
}

void kr_group_init(T_KRGroup *krgroup)
{
    kr_rule_list_init(krgroup->ptRuleList);
}

void kr_group_destruct(T_KRGroup *krgroup)
{
    kr_rule_list_destruct(krgroup->ptRuleList);
    kr_calc_destruct(krgroup->ptGroupCalc);
    kr_free(krgroup);
}

cJSON *kr_group_info(T_KRGroup *krgroup)
{ 
    cJSON *group = cJSON_CreateObject();

    cJSON *def = kr_shm_group_info(krgroup->ptShmGroupDef);
    cJSON_AddItemToObject(group, "def", def);

    cJSON *rules = cJSON_CreateArray();
    T_KRListNode *node = krgroup->ptRuleList->ptRuleList->head;
    while(node) {
        T_KRRule *krrule = kr_list_value(node);
        cJSON_AddItemToArray(rules, kr_rule_info(krrule));
        node = node->next;
    }
    cJSON_AddItemToObject(group, "rules", rules);

    return group;
}


T_KRGroupList *kr_group_list_construct(T_KRShmGroup *shm_group, 
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func)
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
    kr_list_set_free(krgrouplist->ptGroupList, \
            (KRListFreeFunc )kr_group_destruct);
    
    for (i=0; i<krgrouplist->lGroupCnt; ++i) {
        krgroup = kr_group_construct(&shm_group->stShmGroupDef[i], 
                get_type_func, get_value_func);
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

static void _kr_group_init_func(void *value, void *data)
{
    kr_group_init((T_KRGroup *)value);
}

void kr_group_list_init(T_KRGroupList *krgrouplist)
{
    kr_list_foreach(krgrouplist->ptGroupList, _kr_group_init_func, NULL);
}

void kr_group_list_destruct(T_KRGroupList *krgrouplist)
{
    kr_list_destroy(krgrouplist->ptGroupList);
    kr_free(krgrouplist);
}

T_KRGroup *kr_group_lookup(T_KRGroupList *krgrouplist, int id)
{
    T_KRListNode *node = krgrouplist->ptGroupList->head;
    while(node) {
        T_KRGroup *krgroup = kr_list_value(node);
        if (krgroup->lGroupId == id) {
            return krgroup;
        }
        node = node->next;
    }
    return NULL;
}
