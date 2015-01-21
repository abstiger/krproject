#ifndef __KR_DYM_SET_H__
#define __KR_DYM_SET_H__

#include "krutils/kr_utils.h"
#include "krdb/kr_db.h"
#include "kr_shm_set.h"

typedef struct _kr_set_t
{
    T_KRShmSetDef         *ptShmSetDef;
    long                  lSetId;
    E_KRType              eValueType;
    T_KRHashSet           *ptHashSet;
}T_KRSet;

typedef struct _kr_set_table_t
{
    T_KRShmSet            *ptShmSet;
    long                  lSetCnt;
    T_KRHashTable         *ptSetTable;
    time_t                tConstructTime;
}T_KRSetTable;

/* set functions */
T_KRSet *kr_set_construct(T_KRShmSetDef *krshmsetdef, T_DbsEnv *dbsenv);
void kr_set_destruct(T_KRSet *krset);
void kr_set_init(T_KRSet *krset);
cJSON *kr_set_info(T_KRSet *krset);

T_KRSetTable *kr_set_table_construct(T_KRShmSet *krshmset, T_DbsEnv *dbsenv);
void kr_set_table_destruct(T_KRSetTable *krsettable);
void kr_set_table_init(T_KRSetTable *krsettable);
T_KRSet *kr_set_lookup(T_KRSetTable *krsettable, int id);

#endif /* __KR_DYM_SET_H__ */
