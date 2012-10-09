#ifndef __KR_DB_MMAP_H__
#define __KR_DB_MMAP_H__

#include "kr_db_kernel.h"

typedef int (*MMapFunc)(T_KRRecord *ptRecord, void *user_data);

/* mmap file overview:
 * |T_KRTable|T_KRFieldDef*iFieldCnt|T_Record*lSizeKeepValue|
 */
 
int kr_db_mmap_sync(T_KRTable *krtable);
int kr_db_mmap_table(T_KRTable *ptTable);
int kr_db_mmap_file_load(T_KRTable *krtable, char *mmap_file, MMapFunc handle_record_func);
void kr_db_unmmap_table(T_KRTable *ptTable);

#endif /* __KR_DB_MMAP_H__ */
