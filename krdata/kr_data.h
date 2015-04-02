#ifndef __KR_DATA_H__
#define __KR_DATA_H__

#include "kr_data_set.h"
#include "kr_data_sdi.h"
#include "kr_data_ddi.h"
#include "kr_data_hdi.h"
#include "kr_data_hdi_cache.h"


typedef struct _kr_data_t
{
    T_KRParam        *ptParam;
    T_DbsEnv         *ptDbsEnv;
    T_KRModule       *ptModule;
    T_KRCache        *ptHDICache;
    KRGetTypeFunc     pfGetType;
    KRGetValueFunc    pfGetValue;
    
    T_KRSetTable     *ptSetTable;
    T_KRSDITable     *ptSdiTable;
    T_KRDDITable     *ptDdiTable;
    T_KRHDITable     *ptHdiTable;

    T_KRRecord       *ptCurrRec;
    T_KRRecord       *ptRecord;
}T_KRData;


T_KRData *kr_data_construct(T_KRParam *ptParam, 
        T_KRModule *ptModule, T_DbsEnv *ptDbsEnv, T_KRCache *ptHDICache,
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue);
void kr_data_destruct(T_KRData *ptData);
void kr_data_init(T_KRData *ptData);
int kr_data_check(T_KRData *ptData);

E_KRType kr_data_get_type(char kind, int id, void *param);
void *kr_data_get_value(char kind, int id, void *param);

#endif /* __KR_DATA_H__ */

