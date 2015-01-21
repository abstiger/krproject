#ifndef __KR_DDI_H__
#define __KR_DDI_H__

#include "krutils/kr_utils.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"
#include "kr_shm_ddi.h"

typedef int  (*KRDDIAggrFunc)(void *p1, void *p2);

typedef struct _kr_ddi_t
{
    T_KRShmDDIDef         *ptShmDDIDef;
    long                  lDDIId;
    T_KRCalc              *ptDDICalc;
    E_KRType              eValueType;
    KRDDIAggrFunc         DDIAggrFunc;
    
    T_KRRecord            *ptCurrRec;
    E_KRValueInd          eValueInd;
    U_KRValue             uValue;
    T_KRHashTable         *ptRelated;
}T_KRDDI;

typedef struct _kr_ddi_table_t
{
    T_KRShmDDI            *ptShmDDIs;
    long                  lDDICnt;
    T_KRHashTable         *ptDDITable;
    time_t                tConstructTime;
}T_KRDDITable;


/*dynamic dataitem*/
T_KRDDI *kr_ddi_construct(T_KRShmDDIDef *ddi_def, T_KRModule *datamodule, 
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func);
void kr_ddi_destruct(T_KRDDI *krddi);
void kr_ddi_init(T_KRDDI *krddi);
cJSON *kr_ddi_info(T_KRDDI *krddi);

T_KRDDITable *kr_ddi_table_construct(T_KRShmDDI *shm_ddi, T_KRModule *datamodule, KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func);
void kr_ddi_table_destruct(T_KRDDITable *krdditable);
void kr_ddi_table_init(T_KRDDITable *krdditable);
T_KRDDI *kr_ddi_lookup(T_KRDDITable *krdditable, int id);


#endif /* __KR_DDI_H__ */
