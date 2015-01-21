#ifndef __KR_GROUP_H__
#define __KR_GROUP_H__

#include "krutils/kr_utils.h"
#include "krcalc/kr_calc.h"
#include "kr_shm_group.h"
#include "kr_dym_rule.h"

typedef int  (*KRGroupFunc)(void *p1, void *p2);

typedef struct _kr_group_t
{
    T_KRShmGroupDef       *ptShmGroupDef;
    long                  lGroupId;
    T_KRCalc              *ptGroupCalc;
    KRGroupFunc           GroupFunc; 
    T_KRRuleList          *ptRuleList;
}T_KRGroup;

typedef struct _kr_group_list_t
{
    T_KRShmGroup          *ptShmGroups;
    long                  lGroupCnt;
    T_KRList              *ptGroupList;
    time_t                tConstructTime;
}T_KRGroupList;


/*groups*/
T_KRGroup *kr_group_construct(T_KRShmGroupDef *group_def, 
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func);
void kr_group_destruct(T_KRGroup *krgroup);
void kr_group_init(T_KRGroup *krgroup);
cJSON *kr_group_info(T_KRGroup *krgroup);

T_KRGroupList *kr_group_list_construct(T_KRShmGroup *group_list_def, 
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func);
void kr_group_list_destruct(T_KRGroupList *krgrouplist);
void kr_group_list_init(T_KRGroupList *krgrouplist);
T_KRGroup *kr_group_lookup(T_KRGroupList *krgrouplist, int id);

#endif /* __KR_GROUP_H__ */
