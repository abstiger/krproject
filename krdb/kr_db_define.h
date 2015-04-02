#ifndef __KR_DB_DEFINE_H__
#define __KR_DB_DEFINE_H__

#include "kr_db_internal.h"

int _kr_table_load_field_def(T_KRTable *ptTable, T_DbsEnv *dbsenv);
int  _kr_db_build_table(T_KRDB *ptDB);
int  _kr_db_build_index(T_KRDB *ptDB);
int  _kr_db_build_index_table(T_KRDB *ptDB);

#endif /* __KR_DB_DEFINE_H__ */


