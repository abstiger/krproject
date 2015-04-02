#ifndef __KR_DDI_H__
#define __KR_DDI_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"

typedef int  (*KRDDIAggrFunc)(void *p1, void *p2);

typedef struct _kr_ddi_t
{
    T_KRParamDDIDef       *ptParamDDIDef;
    long                  lDDIId;
    T_KRCalc              *ptDDICalc;
    E_KRType              eValueType;
    KRDDIAggrFunc         pfDDIAggr;
    
    T_KRRecord            *ptCurrRec;
    E_KRType              eKeyType;
    void                  *pKeyValue;
    T_KRList              *ptRecList;
    
    E_KRValueInd          eValueInd;
    U_KRValue             uValue;
    T_KRHashTable         *ptRelated;
}T_KRDDI;

typedef struct _kr_ddi_table_t
{
    T_KRParamDDI          *ptParamDDI;
    long                  lDDICnt;
    T_KRHashTable         *ptDDITable;
    time_t                tConstructTime;
}T_KRDDITable;


/*dynamic dataitem*/
T_KRDDI *kr_ddi_construct(T_KRParamDDIDef *ptParamDDIDef, T_KRModule *ptModule, 
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue);
void kr_ddi_destruct(T_KRDDI *ptDDI);
void kr_ddi_init(T_KRDDI *ptDDI);

T_KRDDITable *kr_ddi_table_construct(T_KRParamDDI *ptParamDDI, T_KRModule *ptModule, 
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue);
void kr_ddi_table_destruct(T_KRDDITable *ptDdiTable);
void kr_ddi_table_init(T_KRDDITable *ptDdiTable);
T_KRDDI *kr_ddi_lookup(T_KRDDITable *ptDdiTable, int id);


#endif /* __KR_DDI_H__ */
