#ifndef __KR_HDI_H__
#define __KR_HDI_H__

#include "krcalc/kr_calc.h"
#include "kr_context.h"
#include "kr_hdi_func.h"

typedef struct _kr_hdi_t
{
    T_KRShmHDIDef        *ptShmHDIDef;
    long                  lHDIId;
    E_KRFieldType         eValueType;
    KRHDIAggrFunc         HDIAggrFunc;
    
    T_KRRecord           *ptCurrRec;
    E_KRValueInd          eValueInd;
    T_KRDataValue         eValue;
}T_KRHDI;

struct _kr_hdi_table_t
{
    T_KRShmHDI           *ptShmHDIs;
    long                  lHDICnt;
    T_KRHashTable        *ptHDITable;
    time_t                tConstructTime;
};


/*dynamic dataitem*/
T_KRHDI *kr_hdi_construct(T_KRShmHDIDef *hdi_def, T_KRModule *datamodule);
int kr_hdi_compute(T_KRHDI *krhdi, void *param);
void kr_hdi_destruct(T_KRHDI *krhdi);

T_KRHDITable *kr_hdi_table_construct(T_KRShmHDI *shm_hdi, T_KRModule *datamodule);
void kr_hdi_table_destruct(T_KRHDITable *krhditable);

E_KRFieldType kr_hdi_get_type(int hid, T_KRContext *krcontext);
void *kr_hdi_get_value(int hid, T_KRContext *krcontext);

int kr_hdi_aggr_func(T_KRHDI *krhdi, T_KRContext *calc_param);

#endif /* __KR_HDI_H__ */
