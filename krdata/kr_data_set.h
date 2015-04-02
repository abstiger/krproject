#ifndef __KR_DYM_SET_H__
#define __KR_DYM_SET_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"

typedef struct _kr_set_t
{
    T_KRParamSetDef       *ptParamSetDef;
    long                  lSetId;
    E_KRType              eValueType;
    T_KRHashSet           *ptHashSet;
}T_KRSet;

typedef struct _kr_set_table_t
{
    T_KRParamSet          *ptParamSet;
    long                  lSetCnt;
    T_KRHashTable         *ptSetTable;
    time_t                tConstructTime;
}T_KRSetTable;

/* set functions */
T_KRSet *kr_set_construct(T_KRParamSetDef *ptParamSetDef, T_DbsEnv *ptDbsEnv);
void kr_set_destruct(T_KRSet *ptSet);
void kr_set_init(T_KRSet *ptSet);

T_KRSetTable *kr_set_table_construct(T_KRParamSet *ptParamSet, T_DbsEnv *ptDbsEnv);
void kr_set_table_destruct(T_KRSetTable *ptSetTable);
void kr_set_table_init(T_KRSetTable *ptSetTable);
T_KRSet *kr_set_lookup(T_KRSetTable *ptSetTable, int id);

#endif /* __KR_DYM_SET_H__ */
