#ifndef __KR_DATA_H__
#define __KR_DATA_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"


typedef struct _kr_data_t
{
    T_KRParam        *ptParam;
    T_KRModule       *ptModule;
    time_t           tConstructTime;
    
    T_KRRecord       *ptCurrRec;
    T_KRRecord       *ptTravRec;
    
    T_KRHashTable    *ptItemTable;
    T_KRHashTable    *ptTempTable;
    
    void             *ptExtra;
}T_KRData;


T_KRData *kr_data_construct(T_KRParam *ptParam, T_KRModule *ptModule);
void kr_data_destruct(T_KRData *ptData);
void kr_data_init(T_KRData *ptData);
int kr_data_check(T_KRData *ptData, T_KRParam *ptParam);

E_KRType kr_data_get_type(char kind, int id, void *param);
void *kr_data_get_value(char kind, int id, void *param);

#endif /* __KR_DATA_H__ */

