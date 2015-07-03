#ifndef __KR_DB_H__
#define __KR_DB_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"

typedef struct _kr_db_t T_KRDB;
typedef struct _kr_record_t T_KRRecord;

/*the two public fieldno of ptDB'field definition*/
typedef enum {
    KR_FIELDID_PROCTIME    = 0,   /*timestamp of processing*/
    KR_FIELDID_TRANSTIME   = 1    /*timestamp of transaction*/
}E_KRPublicFieldId;


extern T_KRDB* kr_db_new(char *name, T_KRParam *ptParam);
extern void kr_db_free(T_KRDB *ptDB);

extern int kr_db_insert(T_KRDB *ptDB, T_KRRecord *ptRecord);
extern T_KRList* kr_db_select(T_KRDB *ptDB, int iIndexId, void *key, time_t tBeginTime, time_t tEndTime, int iSortFieldId);
extern T_KRList* kr_db_select_by_record(T_KRRecord *ptRecord, int iIndexId, time_t tBeginTime, time_t tEndTime, int iSortFieldId);

extern int kr_record_get_table_id(T_KRRecord *ptRecord);

#endif /* __KR_DB_H__ */
