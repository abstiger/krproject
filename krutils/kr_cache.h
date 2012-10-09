#ifndef __KR_CACHE_H__
#define __KR_CACHE_H__

#include "kr_types.h"
#include "kr_list.h"
#include "kr_hashtable.h"
#include <stdio.h>
#include <time.h>

typedef struct _kr_cache_t
{
	unsigned int    max_size;
	T_KRList       *list;
	T_KRHashTable  *hash_table;
	KRDupFunc       key_dup_func;
	KRFreeFunc      key_free_func;
	KRDupFunc       value_dup_func;
	KRFreeFunc      value_free_func;
	KRFunc          value_print_func;
}T_KRCache;


typedef struct _kr_cache_node_t
{
    T_KRCache      *cache;
    void           *key;
    void           *value;
    int             hits;
    time_t          last_hit_time;
}T_KRCacheNode;


T_KRCache*    kr_cache_new(unsigned int cache_size, 
                           KRHashFunc key_hash_func, KREqualFunc key_equal_func,
                           KRDupFunc key_dup_func, KRFreeFunc key_free_func,
                           KRDupFunc value_dup_func, KRFreeFunc value_free_func,
                           KRFunc value_print_func);
void          kr_cache_free(T_KRCache *cache);

void          kr_cache_set_max_size(T_KRCache *self, unsigned int max_size);
unsigned int  kr_cache_get_max_size(T_KRCache *self);

unsigned int  kr_cache_get_size(T_KRCache *self);

void*         kr_cache_get(T_KRCache *self, void *key);
void          kr_cache_set(T_KRCache *self, void *key, void *value);
void          kr_cache_dump(T_KRCache *cache, FILE *fp);

#endif /* __KR_CACHE_H__ */
