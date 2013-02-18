#ifndef __KR_DYNAMICMEM_H__
#define __KR_DYNAMICMEM_H__

#include "krutils/kr_utils.h"
#include "krshm/kr_shm.h"

typedef struct _kr_sdi_table_t T_KRSDITable;
typedef struct _kr_ddi_table_t T_KRDDITable;
typedef struct _kr_hdi_table_t T_KRHDITable;
typedef struct _kr_rule_group_t T_KRRuleGroup;
typedef struct _kr_group_list_t T_KRGroupList;
typedef struct _kr_context_env_t T_KRContextEnv;

typedef struct _kr_dynamic_mem_t
{
    T_KRSDITable     *sditable;
    T_KRDDITable     *dditable;
    T_KRHDITable     *hditable;
    T_KRGroupList    *grouplist;
    T_KRRuleGroup    *rulegroup;
}T_KRDynamicMem;


T_KRDynamicMem *kr_dynamicmem_init(T_KRContextEnv *ptEnv);
void kr_dynamicmem_fini(T_KRDynamicMem *ptDyn);
int kr_dynamicmem_check(T_KRDynamicMem *ptDyn, T_KRContextEnv *ptEnv);

#endif  /*__KR_DYNAMICMEM_H__*/

