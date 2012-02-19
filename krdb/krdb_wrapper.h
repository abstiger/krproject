#ifndef __KRDB_WRAPPER_H__
#define __KRDB_WRAPPER_H__

#include <stdio.h>
#include "krdb/krdb.h"
#include "krdb/krdb_interface.h"

extern int SetFieldDef(T_KRFieldDef *ptFieldDef, int iTableId, int iFieldId);
extern T_KRDB* KRDBStartup(char *dbname);
extern int KRDBInsert(T_KRDB *ptKRDB, int iTableId, void *ptReqData);
extern GList* KRDBSelect(T_KRDB *ptKRDB, int iTableId, int iIndexId, void *key);
extern int KRDBShutdown(T_KRDB *ptKRDB);

extern void DumpKRDB(FILE *fp, T_KRDB *ptKRDB, int iInd);
extern void DumpFieldDef(FILE *fp, T_KRDB *ptKRDB, int iTableId);

#endif /* __KRDB_WRAPPER_H__ */
