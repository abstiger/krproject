#ifndef __KR_SDI_H__
#define __KR_SDI_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"

typedef int  (*KRSDIAggrFunc)(void *p1, void *p2);

typedef struct _kr_sdi_t
{
    T_KRParamSDIDef       *ptParamSDIDef;
    long                  lSDIId;
    T_KRCalc              *ptSDICalc;
    E_KRType              eValueType;
    KRSDIAggrFunc         pfSDIAggr;
    
    T_KRRecord            *ptCurrRec;
    E_KRType              eKeyType;
    void                  *pKeyValue;
    T_KRList              *ptRecList;

    E_KRValueInd          eValueInd;
    U_KRValue             uValue;
    T_KRHashTable         *ptRelated;
}T_KRSDI;

typedef struct _kr_sdi_table_t
{
    T_KRParamSDI          *ptParamSDI;
    long                  lSDICnt;
    T_KRHashTable         *ptSDITable;
    time_t                tConstructTime;
}T_KRSDITable;

/*static dataitem*/
T_KRSDI *kr_sdi_construct(T_KRParamSDIDef *ptParamSDIDef, T_KRModule *ptModule,
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue);
void kr_sdi_destruct(T_KRSDI *ptSDI);
void kr_sdi_init(T_KRSDI *ptSDI);

T_KRSDITable *kr_sdi_table_construct(T_KRParamSDI *ptParamSDI, T_KRModule *ptModule, 
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue);
void kr_sdi_table_destruct(T_KRSDITable *ptSdiTable);
void kr_sdi_table_init(T_KRSDITable *ptSdiTable);
T_KRSDI *kr_sdi_lookup(T_KRSDITable *ptSdiTable, int id);

#endif /* __KR_SDI_H__ */
