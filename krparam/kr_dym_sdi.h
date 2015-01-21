#ifndef __KR_SDI_H__
#define __KR_SDI_H__

#include "krutils/kr_utils.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"
#include "kr_shm_sdi.h"

typedef int  (*KRSDIAggrFunc)(void *p1, void *p2);

typedef struct _kr_sdi_t
{
    T_KRShmSDIDef         *ptShmSDIDef;
    long                  lSDIId;
    T_KRCalc              *ptSDICalc;
    E_KRType              eValueType;
    KRSDIAggrFunc         SDIAggrFunc;
    
    T_KRRecord            *ptCurrRec;
    E_KRValueInd          eValueInd;
    U_KRValue             uValue;
    T_KRHashTable         *ptRelated;
}T_KRSDI;

typedef struct _kr_sdi_table_t
{
    T_KRShmSDI            *ptShmSDIs;
    long                  lSDICnt;
    T_KRHashTable         *ptSDITable;
    time_t                tConstructTime;
}T_KRSDITable;

/*static dataitem*/
T_KRSDI *kr_sdi_construct(T_KRShmSDIDef *sdi_def, T_KRModule *datamodule,
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func);
void kr_sdi_destruct(T_KRSDI *krsdi);
void kr_sdi_init(T_KRSDI *krsdi);
cJSON *kr_sdi_info(T_KRSDI *krsdi);

T_KRSDITable *kr_sdi_table_construct(T_KRShmSDI *shm_sdi, T_KRModule *datamodule, KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func);
void kr_sdi_table_destruct(T_KRSDITable *krsditable);
void kr_sdi_table_init(T_KRSDITable *krsditable);
T_KRSDI *kr_sdi_lookup(T_KRSDITable *krsditable, int id);

#endif /* __KR_SDI_H__ */
