#ifndef __KR_HASHTABLE_H__
#define __KR_HASHTABLE_H__

#include <sys/types.h>
#include "kr_alloc.h"
#include "kr_types.h"

typedef struct _kr_hashtable_t  T_KRHashTable;

typedef void (*KRHFunc)(void *key, void *value, void *data);
typedef kr_bool  (*KRHRFunc)(void *key, void *value, void *user_data);

/* Hash table operating functions */
T_KRHashTable* kr_hashtable_new (KRHashFunc hash_func, KREqualFunc key_equal_func);
T_KRHashTable* kr_hashtable_new_full (KRHashFunc         hash_func,
                                    KREqualFunc         key_equal_func,
                                    KRDestroyNotify  key_destroy_func,
                                    KRDestroyNotify  value_destroy_func);
void         kr_hashtable_destroy (T_KRHashTable       *hash_table);
void         kr_hashtable_insert (T_KRHashTable   *hash_table, 
                                  void          *key, 
                                  void          *value);
void         kr_hashtable_replace (T_KRHashTable    *hash_table,
                                   void           *key, 
                                   void           *value);
kr_bool      kr_hashtable_remove (T_KRHashTable       *hash_table,
                                  const void        *key);
void         kr_hashtable_remove_all (T_KRHashTable  *hash_table);
kr_bool      kr_hashtable_steal (T_KRHashTable     *hash_table,
                                 const void      *key);
void         kr_hashtable_steal_all (T_KRHashTable    *hash_table);
void*        kr_hashtable_lookup (T_KRHashTable      *hash_table,
                                  const void       *key);
kr_bool      kr_hashtable_lookup_extended (T_KRHashTable *hash_table,
                                           const void  *lookup_key,
                                           void       **orig_key,
                                           void       **value);
unsigned int kr_hashtable_size (T_KRHashTable       *hash_table);

                                           
/*WARN: functions below should not be used, Because they are very slow!*/
void         kr_hashtable_foreach (T_KRHashTable  *hash_table,
                                   KRHFunc        func,
                                   void          *user_data);
void*        kr_hashtable_find (T_KRHashTable    *hash_table,
                               KRHRFunc           predicate,
                               void           *user_data);
unsigned int kr_hashtable_foreach_remove (T_KRHashTable *hash_table,
                                          KRHRFunc       func,
                                          void        *user_data);
unsigned int kr_hashtable_foreach_steal (T_KRHashTable      *hash_table,
                                         KRHRFunc       func,
                                         void          *user_data);


#endif /* __KR_HASHTABLE_H__ */
