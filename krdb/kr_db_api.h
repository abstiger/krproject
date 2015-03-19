#ifndef __KR_DB_API_H__
#define __KR_DB_API_H__

#include "kr_db.h"

extern cJSON *kr_db_info(T_KRDB *krdb);
extern cJSON *kr_db_table_info(T_KRTable *krtable);
extern cJSON *kr_db_index_info(T_KRIndex *krindex);
extern cJSON *kr_db_record_info(T_KRRecord *krrecord);

#endif /* __KR_DB_API_H__ */
