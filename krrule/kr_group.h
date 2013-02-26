#ifndef __KR_GROUP_H__
#define __KR_GROUP_H__

#include "krutils/kr_utils.h"
#include "krshm/kr_shm.h"
#include "krcalc/kr_calc.h"
#include "kr_dynamicmem.h"
#include "kr_context.h"
#include "kr_group_func.h"

typedef struct _kr_group_t
{
    T_KRShmGroupDef       *ptShmGroupDef;
    long                  lGroupId;
    T_KRCalc              *ptGroupCalc;
    KRGroupFunc           GroupFunc; 
    T_KRRuleGroup         *ptRuleGroup;
}T_KRGroup;

struct _kr_group_list_t
{
    T_KRShmGroup          *ptShmGroups;
    long                  lGroupCnt;
    T_KRList              *ptGroupList;
    time_t                tConstructTime;
};


/*groups*/
T_KRGroup *kr_group_construct(T_KRShmGroupDef *group_def, T_KRModule *groupmodule);
int kr_group_route(T_KRGroup *krgroup, void *krcontext);
void kr_group_destruct(void *group);

T_KRGroupList *kr_group_list_construct(T_KRShmGroup *group_list_def, T_KRModule *groupmodule);
int kr_group_list_detect(T_KRContext *krcontext);
void kr_group_list_destruct(T_KRGroupList *krgrouplist);

int kr_group_func(T_KRGroup *krgroup, T_KRContext *krcontext);

#endif /* __KR_GROUP_H__ */
