#ifndef __KR_HDI_H__
#define __KR_HDI_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"

typedef int (*KRHDIAggrFunc)(void *p1, void *p2);

typedef struct _kr_hdi_t
{
    T_KRParamHDIDef       *ptParamHDIDef;
    long                  lHDIId;
    E_KRType              eValueType;
    KRHDIAggrFunc         pfHDIAggr;
    
    T_KRRecord            *ptCurrRec;
    E_KRType              eKeyType;
    void                  *pKeyValue;
    
    E_KRValueInd          eValueInd;
    U_KRValue             uValue;
}T_KRHDI;

typedef struct _kr_hdi_table_t
{
    T_KRParamHDI          *ptParamHDI;
    long                  lHDICnt;
    T_KRHashTable         *ptHDITable;
    time_t                tConstructTime;
}T_KRHDITable;


/*dynamic dataitem*/
T_KRHDI *kr_hdi_construct(T_KRParamHDIDef *ptParamHDIDef, T_KRModule *ptModule);
void kr_hdi_destruct(T_KRHDI *ptHDI);
void kr_hdi_init(T_KRHDI *ptHDI);

T_KRHDITable *kr_hdi_table_construct(T_KRParamHDI *ptParamHDI, T_KRModule *ptModule);
void kr_hdi_table_destruct(T_KRHDITable *ptHdiTable);
void kr_hdi_table_init(T_KRHDITable *ptHdiTable);
T_KRHDI *kr_hdi_lookup(T_KRHDITable *ptHdiTable, int id);

#endif /* __KR_HDI_H__ */
