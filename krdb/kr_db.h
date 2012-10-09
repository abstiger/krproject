#ifndef __KR_DB_H__
#define __KR_DB_H__

#include "krdb/kr_db_kernel.h"

extern T_KRDB* kr_db_startup(char *dbname);
extern int kr_db_shutdown(T_KRDB *ptKRDB);
extern int kr_db_load(T_KRDB *ptKRDB);
extern T_KRRecord *kr_db_insert(T_KRDB *ptKRDB, int iTableId, void *ptReqData);
extern T_KRList* kr_db_select(T_KRDB *ptKRDB, int iTableId, int iIndexId, void *key);
extern void kr_db_dump(FILE *fp, T_KRDB *ptKRDB, int iInd);

extern void kr_db_dump_field_def(FILE *fp, T_KRDB *ptKRDB, int iTableId);
extern int kr_db_set_field_def(T_KRFieldDef *ptFieldDef, int iTableId, int iFieldId);

#endif /* __KR_DB_H__ */
