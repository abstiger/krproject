#ifndef __KR_DYM_H__
#define __KR_DYM_H__

#include "kr_dym_set.h"
#include "kr_dym_sdi.h"
#include "kr_dym_ddi.h"
#include "kr_dym_hdi.h"
#include "kr_dym_rule.h"
#include "kr_dym_group.h"
#include "kr_shm.h"

typedef struct _kr_dynamic_mem_t
{
    T_KRSetTable     *settable;
    T_KRSDITable     *sditable;
    T_KRDDITable     *dditable;
    T_KRHDITable     *hditable;
    T_KRGroupList    *grouplist;
    T_KRRuleList     *rulelist;
}T_KRDynamicMem;


T_KRDynamicMem *kr_dym_construct(
        T_KRShareMem *ptShm, T_KRModule *ptDataModule, T_DbsEnv *dbsenv,
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func);
void kr_dym_destruct(T_KRDynamicMem *ptDyn);
void kr_dym_init(T_KRDynamicMem *ptDyn);
cJSON *kr_dym_info(T_KRDynamicMem *ptDyn);

int kr_dym_check(T_KRDynamicMem *ptDyn, 
        T_KRShareMem *ptShm, T_KRModule *ptDataModule, T_DbsEnv *dbsenv,
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func);

#endif  /*__KR_DYM_H__*/

