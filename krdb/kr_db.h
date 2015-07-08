#ifndef __KR_DB_H__
#define __KR_DB_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krinput/kr_input.h"

typedef struct _kr_db_t T_KRDB;

extern T_KRDB* kr_db_new(char *name, T_KRParam *ptParam);
extern void kr_db_free(T_KRDB *ptDB);

extern int kr_db_insert(T_KRDB *ptDB, T_KRRecord *ptRecord);
extern int kr_db_delete(T_KRDB *ptDB, T_KRRecord *ptRecord);

extern T_KRList* kr_db_select(T_KRDB *ptDB, int iIndexId, void *key, \
        time_t tBeginTime, time_t tEndTime, int iSortFieldId);

#endif /* __KR_DB_H__ */
