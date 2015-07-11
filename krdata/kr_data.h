#ifndef __KR_DATA_H__
#define __KR_DATA_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krcalc/kr_calc.h"


typedef struct _kr_data_t
{
    T_KRModule       *ptModule;
    KRGetTypeFunc    pfGetType;
    KRGetValueFunc   pfGetValue;
    T_KRHashTable    *ptItemTable;
    time_t           tConstructTime;
}T_KRData;


T_KRData *kr_data_construct(T_KRParam *ptParam, T_KRModule *ptModule,
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue);
void kr_data_destruct(T_KRData *ptData);
void kr_data_init(T_KRData *ptData);
int kr_data_check(T_KRData *ptData, T_KRParam *ptParam);

E_KRType kr_data_get_type(T_KRData *ptData, char kind, int id, T_KRContext *ptContext);
void *kr_data_get_value(T_KRData *ptData, char kind, int id, T_KRContext *ptContext);

#endif /* __KR_DATA_H__ */

