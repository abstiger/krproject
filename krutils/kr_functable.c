#include "kr_utils.h"
#include "kr_functable.h"

/*create a new table*/
T_KRFuncTable *kr_functable_create(char *name)
{
    T_KRFuncTable *func_table = kr_calloc(sizeof(*func_table));
    func_table->name = (char *)kr_strdup(name);
    func_table->table = kr_hashtable_new_full(
            (KRHashFunc )kr_string_hash, 
            (KREqualFunc )kr_string_equal, 
            kr_free, NULL);
    
    return func_table;
}

/* search an func_name in a func_table
 * return TURE while found, FALSE for else
 */
KRFunc kr_functable_search(T_KRFuncTable *func_table, char *func_name)
{
    return kr_hashtable_lookup(func_table->table, func_name);
}

/* register an function
 * return 0 while success, -1 for else
 */
int 
kr_functable_register(T_KRFuncTable *func_table, char *func_name, KRFunc func)
{
    char *dup_key = kr_string_dup(func_name);
    kr_hashtable_insert(func_table->table, dup_key, func);
    return 0;
}

/* unregister an function
 * return 0 while success, -1 for else
 */
int kr_functable_unregister(T_KRFuncTable *func_table, char *func_name)
{
    kr_hashtable_remove(func_table->table, func_name);
    return 0;
}

/* destroy this func_table */
void kr_functable_destroy(T_KRFuncTable *func_table)
{
    if (func_table != NULL) {
        kr_hashtable_destroy(func_table->table);
        kr_free(func_table->name);
        kr_free(func_table); 
    }
}

