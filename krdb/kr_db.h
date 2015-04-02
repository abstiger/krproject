#ifndef __KR_DB_H__
#define __KR_DB_H__

#include "kr_db_define.h"
#include "kr_db_internal.h"
#include "kr_db_external.h"


extern T_KRDB* kr_db_new(char *psDBName, T_DbsEnv *ptDbsEnv, T_KRModule *ptModule);
extern void kr_db_free(T_KRDB *ptDB);

extern int kr_db_insert(T_KRDB *ptDB, T_KRRecord *ptRecord);
extern T_KRList* kr_db_select(T_KRDB *ptDB, int iIndexId, void *key, time_t tBeginTime, time_t tEndTime, int iSortFieldId);

#endif /* __KR_DB_H__ */
