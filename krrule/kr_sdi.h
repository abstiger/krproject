#ifndef __KR_SDI_H__
#define __KR_SDI_H__

#include "kr_context.h"
#include "krcalc/kr_calc.h"
#include "kr_sdi_func.h"


typedef struct _kr_sdi_t
{
    T_KRShmSDIDef         *ptShmSDIDef;
    long                  lSDIId;
    T_KRCalc              *ptSDICalc;
    E_KRFieldType         eValueType;
    KRSDIAggrFunc         SDIAggrFunc;
    
    T_KRRecord            *ptCurrRec;
    E_KRValueInd          eValueInd;
    T_KRDataValue         eValue;
}T_KRSDI;

struct _kr_sdi_table_t
{
    T_KRShmSDI            *ptShmSDIs;
    long                  lSDICnt;
    T_KRHashTable         *ptSDITable;
    time_t                tConstructTime;
};

/*static dataitem*/
T_KRSDI *kr_sdi_construct(T_KRShmSDIDef *sdi_def, T_KRModule *datamodule);
int kr_sdi_compute(T_KRSDI *krsdi, void *param);
void kr_sdi_destruct(T_KRSDI *krsdi);

T_KRSDITable *kr_sdi_table_construct(T_KRShmSDI *shm_sdi, T_KRModule *datamodule);
void kr_sdi_table_destruct(T_KRSDITable *krsditable);

E_KRFieldType kr_sdi_get_type(int sid, T_KRContext *krcontext);
void *kr_sdi_get_value(int sid, T_KRContext *krcontext);

int kr_sdi_aggr_func(T_KRSDI *krsdi, T_KRContext *calc_param);

#endif /* __KR_SDI_H__ */
