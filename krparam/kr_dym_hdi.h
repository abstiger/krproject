#ifndef __KR_HDI_H__
#define __KR_HDI_H__

#include "krutils/kr_utils.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"
#include "kr_shm_hdi.h"

typedef int (*KRHDIAggrFunc)(void *p1, void *p2);

typedef struct _kr_hdi_t
{
    T_KRShmHDIDef        *ptShmHDIDef;
    long                  lHDIId;
    E_KRType              eValueType;
    KRHDIAggrFunc         HDIAggrFunc;
    
    T_KRRecord           *ptCurrRec;
    E_KRValueInd          eValueInd;
    U_KRValue             uValue;
}T_KRHDI;

typedef struct _kr_hdi_table_t
{
    T_KRShmHDI           *ptShmHDIs;
    long                  lHDICnt;
    T_KRHashTable        *ptHDITable;
    time_t                tConstructTime;
}T_KRHDITable;


/*dynamic dataitem*/
T_KRHDI *kr_hdi_construct(T_KRShmHDIDef *hdi_def, T_KRModule *datamodule);
void kr_hdi_destruct(T_KRHDI *krhdi);
void kr_hdi_init(T_KRHDI *krhdi);
cJSON *kr_hdi_info(T_KRHDI *krhdi);

T_KRHDITable *kr_hdi_table_construct(T_KRShmHDI *shm_hdi, T_KRModule *datamodule);
void kr_hdi_table_destruct(T_KRHDITable *krhditable);
void kr_hdi_table_init(T_KRHDITable *krhditable);
T_KRHDI *kr_hdi_lookup(T_KRHDITable *krhditable, int id);

#endif /* __KR_HDI_H__ */
