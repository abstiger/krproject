#ifndef __KR_GROUP_H__
#define __KR_GROUP_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"
#include "kr_flow_rule.h"

typedef int  (*KRGroupFunc)(void *p1, void *p2);

typedef struct _kr_group_t
{
    T_KRParamGroupDef     *ptParamGroupDef;
    long                  lGroupId;
    T_KRCalc              *ptGroupCalc;
    KRGroupFunc           GroupFunc; 
    T_KRRuleList          *ptRuleList;
}T_KRGroup;

typedef struct _kr_group_list_t
{
    T_KRParamGroup        *ptParamGroups;
    long                  lGroupCnt;
    T_KRList              *ptGroupList;
    time_t                tConstructTime;
}T_KRGroupList;


/*groups*/
T_KRGroup *kr_group_construct(T_KRParamGroupDef *ptParamGroupDef, 
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue);
void kr_group_destruct(T_KRGroup *ptGroup);
void kr_group_init(T_KRGroup *ptGroup);
cJSON *kr_group_info(T_KRGroup *ptGroup);

T_KRGroupList *kr_group_list_construct(T_KRParamGroup *ptParamGroup, 
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue);
void kr_group_list_destruct(T_KRGroupList *ptGroupList);
void kr_group_list_init(T_KRGroupList *ptGroupList);
T_KRGroup *kr_group_lookup(T_KRGroupList *ptGroupList, int id);

#endif /* __KR_GROUP_H__ */
