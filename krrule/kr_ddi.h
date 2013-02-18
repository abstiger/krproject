#ifndef __KR_DDI_H__
#define __KR_DDI_H__

#include "kr_context.h"
#include "krcalc/kr_calc.h"
#include "kr_ddi_func.h"


typedef struct _kr_ddi_t
{
    T_KRShmDDIDef         *ptShmDDIDef;
    long                  lDDIId;
    T_KRCalc              *ptDDICalc;
    E_KRFieldType         eValueType;
    KRDDIAggrFunc         DDIAggrFunc;
    
    T_KRRecord            *ptCurrRec;
    E_KRValueInd          eValueInd;
    T_KRDataValue         eValue;
}T_KRDDI;

struct _kr_ddi_table_t
{
    T_KRShmDDI            *ptShmDDIs;
    long                  lDDICnt;
    T_KRHashTable           *ptDDITable;
    time_t                tConstructTime;
};


/*dynamic dataitem*/
T_KRDDI *kr_ddi_construct(T_KRShmDDIDef *ddi_def, T_KRModule *datamodule);
int kr_ddi_compute(T_KRDDI *krddi, void *param);
void kr_ddi_destruct(T_KRDDI *krddi);

T_KRDDITable *kr_ddi_table_construct(T_KRShmDDI *shm_ddi, T_KRModule *datamodule);
void kr_ddi_table_destruct(T_KRDDITable *krdditable);

E_KRFieldType kr_ddi_get_type(int did, T_KRContext *krcontext);
void *kr_ddi_get_value(int did, T_KRContext *krcontext);

int kr_ddi_aggr_func(T_KRDDI *krddi, T_KRContext *calc_param);

#endif /* __KR_DDI_H__ */
