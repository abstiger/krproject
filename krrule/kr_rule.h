#ifndef __KR_RULE_H__
#define __KR_RULE_H__

#include "krutils/kr_utils.h"
#include "krshm/kr_shm.h"
#include "krcalc/kr_calc.h"
#include "kr_dynamicmem.h"
#include "kr_context.h"

typedef struct _kr_rule_t
{
    T_KRShmRuleDef        *ptShmRuleDef;
    long                  lRuleId;
    T_KRCalc              *ptRuleCalc;
    boolean               bViolated;
}T_KRRule;

struct _kr_rule_group_t
{
    T_KRShmRule           *ptShmRules;
	long                  lRuleCnt;
    T_KRList              *ptRuleList;
    time_t                tConstructTime;
};


/*rules*/
T_KRRule *kr_rule_construct(T_KRShmRuleDef *rule_def);
int kr_rule_detect(T_KRRule *krrule, void *krcontext);
void kr_rule_destruct(void *rule);

T_KRRuleGroup *kr_rule_group_construct(T_KRShmRule *rule_group_def);
int kr_rule_group_detect(T_KRContext *krcontext);
void kr_rule_group_destruct(T_KRRuleGroup *krrulegroup);

#endif /* __KR_RULE_H__ */
