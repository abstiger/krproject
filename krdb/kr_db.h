#ifndef __KR_DB_H__
#define __KR_DB_H__

#include <stdio.h>
#include "krdb/kr_db_kernel.h"
#include "dbs/dbs_basopr.h"

extern T_KRDB* kr_db_startup(T_DbsEnv *dbsenv, char *dbname, T_KRModule *krdbmodule);
extern int kr_db_shutdown(T_KRDB *ptKRDB);
extern int kr_db_load(T_KRDB *ptKRDB);
extern T_KRRecord *kr_db_insert(T_KRDB *ptKRDB, int iTableId, void *ptReqData);
extern T_KRList* kr_db_select(T_KRDB *ptKRDB, int iTableId, int iIndexId, void *key);
extern void kr_db_dump(T_KRDB *ptKRDB, int iInd, FILE *fp);
extern void kr_db_dump_record(T_KRRecord *ptRecord, FILE *fp);

extern void kr_db_dump_field_def(T_KRDB *ptKRDB, int iTableId, FILE *fp);
extern int kr_db_set_field_def(T_DbsEnv *dbsenv, T_KRFieldDef *ptFieldDef, int iTableId, int iFieldId);

#endif /* __KR_DB_H__ */
