#ifndef __KR_DB_EXTERNAL_H__
#define __KR_DB_EXTERNAL_H__

#include "kr_db.h"

extern int kr_db_external_insert(T_KRRecord *ptRecord, void *ptExtEnv);
extern int kr_db_external_select(T_KRList *pRecList, int iTableId,
        E_KRPublicFieldId eQueryFieldId, time_t tBeginTime, time_t tEndTime,
        int iSortFieldId, void *ptExtEnv);

#endif /* __KR_DB_EXTERNAL_H__ */
