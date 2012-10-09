#ifndef __KR_HASHTABLE_H__
#define __KR_HASHTABLE_H__

#include <sys/types.h>
#include "kr_types.h"
#include "kr_alloc.h"

typedef struct _kr_hashtable_t  T_KRHashTable;

/* Hash table operating functions */
T_KRHashTable* kr_hashtable_new (KRHashFunc hash_func, KREqualFunc key_equal_func);
T_KRHashTable* kr_hashtable_new_full (KRHashFunc	     hash_func,
					                KREqualFunc	     key_equal_func,
					                KRDestroyNotify  key_destroy_func,
					                KRDestroyNotify  value_destroy_func);
void	     kr_hashtable_destroy (T_KRHashTable	   *hash_table);
void	     kr_hashtable_insert (T_KRHashTable   *hash_table, 
                                  void          *key, 
                                  void          *value);
void         kr_hashtable_replace (T_KRHashTable    *hash_table,
			 		              void           *key, 
			 		              void           *value);
boolean      kr_hashtable_remove (T_KRHashTable	   *hash_table,
			 		             const void        *key);
void         kr_hashtable_remove_all (T_KRHashTable  *hash_table);
boolean      kr_hashtable_steal (T_KRHashTable     *hash_table,
			 		            const void      *key);
void         kr_hashtable_steal_all (T_KRHashTable    *hash_table);
void*        kr_hashtable_lookup (T_KRHashTable      *hash_table,
			 		             const void       *key);
boolean      kr_hashtable_lookup_extended (T_KRHashTable *hash_table,
			 		                      const void  *lookup_key,
			 		                      void       **orig_key,
			 		                      void       **value);
unsigned int kr_hashtable_size (T_KRHashTable	   *hash_table);

			 		                      
/*WARN: functions below should not be used, Because they are very slow!*/
void	     kr_hashtable_foreach (T_KRHashTable  *hash_table,
			 		              KRHFunc	    func,
			 		              void          *user_data);
void*        kr_hashtable_find (T_KRHashTable    *hash_table,
					           KRHRFunc	       predicate,
					           void           *user_data);
unsigned int kr_hashtable_foreach_remove (T_KRHashTable *hash_table,
			                              KRHRFunc	   func,
			                              void        *user_data);
unsigned int kr_hashtable_foreach_steal (T_KRHashTable	  *hash_table,
			                             KRHRFunc	   func,
			                             void          *user_data);


/* Various kinds of Hash Functions */
static inline unsigned int
kr_int_hash (const void * v)
{
    return *(const int*) v;
}

static inline unsigned int
kr_long_hash (const void * v)
{
    return (unsigned int) *(const long*) v;
}

static inline unsigned int
kr_double_hash (const void * v)
{
    return (unsigned int) *(const double*) v;
}

static inline unsigned int
kr_string_hash (const void * v)
{
    const signed char *p;
    unsigned int h = 5381;

    for (p = v; *p != '\0'; p++)
        h = (h << 5) + h + *p;

    return h;
}

static inline unsigned int
kr_pointer_hash (const void * v)
{
    return (unsigned int )(unsigned long)(v);
}


/* Various kinds of Equal Functions */
static inline boolean
kr_int_equal (const void * v1, const void * v2)
{
    return *((const int*) v1) == *((const int*) v2);
}

static inline boolean
kr_long_equal (const void * v1, const void * v2)
{
    return *((const long*) v1) == *((const long*) v2);
}

static inline boolean
kr_double_equal (const void * v1, const void * v2)
{
    return *((const double*) v1) == *((const double*) v2);
}

static inline boolean
kr_string_equal (const void * v1, const void * v2)
{
    const char *string1 = v1;
    const char *string2 = v2;
  
    return strcmp(string1, string2) == 0;
}

static inline boolean
kr_pointer_equal (const void * v1, const void * v2)
{
    return v1 == v2;
}


/* Various kinds of Duplicate Functions */
static inline void *
kr_int_dup(void *key)
{
    int *i = (int *)kr_malloc(sizeof(int));
    *i = *(int *)key;
    return (void *)i;
}

static inline void *
kr_long_dup(void *key)
{
    long *l = (long *)kr_malloc(sizeof(long));
    *l = *(long *)key;
    return (void *)l;
}

static inline void *
kr_double_dup( void *key)
{
    int *d = (int *)kr_malloc(sizeof(double));
    *d = *(int *)key;
    return (void *)d;
}

static inline void *
kr_string_dup(void *key)
{
    return (void *)kr_strdup(key);;
}

static inline void *
kr_pointer_dup(void *key)
{
    void *p = (void *)kr_malloc(sizeof(void *));
    p = (void *)key;
    return (void *)p;
}
#endif /* __KR_HASHTABLE_H__ */
