#ifndef __KR_FUNC_TABLE_H__
#define __KR_FUNC_TABLE_H__

#include "kr_types.h"
#include "kr_hashtable.h"

typedef struct _kr_func_table_t
{
    char            *name;
    T_KRHashTable   *table;
}T_KRFuncTable;


T_KRFuncTable *kr_functable_create(char *name);
KRFunc kr_functable_search(T_KRFuncTable *func_table, char *func_name);
int kr_functable_register(T_KRFuncTable *func_table, char *func_name, KRFunc func);
int kr_functable_unregister(T_KRFuncTable *func_table, char *func_name);
void kr_functable_destroy(T_KRFuncTable *func_table);

#endif /* __KR_FUNC_TABLE_H__ */
