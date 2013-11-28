#include "kr_cache.h"


static T_KRCacheNode *
kr_cache_node_new(T_KRCache *cache, void *key, void *value)
{
    T_KRCacheNode *cachenode= (T_KRCacheNode *)kr_malloc(sizeof(T_KRCacheNode));
    if (cachenode == NULL) {
        fprintf(stderr, "kr_malloc T_KRCacheNode failed!\n");
        return NULL;
    }
    cachenode->cache = cache;
    if (cache->key_dup_func)
        cachenode->key = cache->key_dup_func(key);
    else 
        cachenode->key = key;
        
    if (cache->value_dup_func)
        cachenode->value = cache->value_dup_func(value);
    else 
        cachenode->value = value;

    cachenode->hits = 0;
    cachenode->last_hit_time = (time_t)0;
    
    return cachenode;
}


static void kr_cache_node_free(T_KRCacheNode *cachenode)
{
    T_KRCache *cache = cachenode->cache;
    if (cache->key_free_func)
        cache->key_free_func(cachenode->key);

    if (cache->value_free_func)
        cache->value_free_func(cachenode->value);

    kr_free(cachenode);
}


static void kr_cache_node_dump(T_KRCacheNode *cachenode, FILE *fp)
{
    char caTime[80] = {0};
    T_KRCache *cache = cachenode->cache;
    struct tm *ptmNow = localtime(&cachenode->last_hit_time);
    strftime(caTime, sizeof(caTime), "%c", ptmNow);
    fprintf(fp, "key:%s, hits:%d, last_hit_time:%s \n", \
           (char *)cachenode->key, cachenode->hits, caTime);
    if (cache->value_print_func)
        cache->value_print_func(cachenode->value, fp);
}



T_KRCache* kr_cache_new(unsigned int cache_size,
                        KRHashFunc key_hash_func, KREqualFunc key_equal_func,
                        KRDupFunc key_dup_func, KRFreeFunc key_free_func,
                        KRDupFunc value_dup_func, KRFreeFunc value_free_func,
                        KRFunc value_print_func)
{
    if (cache_size <= 0) {
        fprintf(stderr, "cache_size[%u] less than zero!\n", cache_size);
        return NULL;
    }
    
    T_KRCache *cache = (T_KRCache *)kr_malloc(sizeof(T_KRCache));
    if (cache == NULL) {
        fprintf(stderr, "kr_malloc T_KRCache failed!\n");
        return NULL;
    }
    
    cache->max_size = cache_size;
    cache->list = kr_list_new();
    kr_list_set_free(cache->list, (KRFreeFunc)kr_cache_node_free);
    cache->hash_table = kr_hashtable_new(key_hash_func, key_equal_func);
    
    cache->key_dup_func = key_dup_func;
    cache->key_free_func = key_free_func;
    cache->value_dup_func = value_dup_func;
    cache->value_free_func = value_free_func;
    cache->value_print_func = value_print_func;
    
    return cache;
}


void kr_cache_free(T_KRCache *cache)
{
    kr_list_destroy(cache->list);
    cache->list = NULL;
    
    kr_hashtable_destroy(cache->hash_table);
    cache->hash_table = NULL;
    
    kr_free(cache);
}


static void kr_cache_evict_n_oldest(T_KRCache *cache, int n)
{
    int   i;
    T_KRListNode *oldest;
    T_KRCacheNode *cachenode;
    for (i = 0; i < n; i++) {
        oldest = kr_list_last(cache->list);
        if (oldest == NULL) {
            return;
        }
        
        cachenode = kr_list_value(oldest);
        kr_hashtable_remove(cache->hash_table, cachenode->key);
        kr_list_delete(cache->list, oldest);
    }
}


void kr_cache_set_max_size(T_KRCache *cache, unsigned int max_size)
{
    unsigned int old_max_size = cache->max_size;
    
    cache->max_size = max_size;
    if (old_max_size > max_size)
        kr_cache_evict_n_oldest(cache, old_max_size - max_size);
}


unsigned int kr_cache_get_max_size(T_KRCache *cache)
{
    return cache->max_size;
}


unsigned int kr_cache_get_size(T_KRCache *cache)
{
    return kr_hashtable_size(cache->hash_table);
}


void *kr_cache_get(T_KRCache *cache, void *key)
{
    T_KRListNode *listnode = kr_hashtable_lookup(cache->hash_table, key);
    if (listnode) {
        /*move to head*/
        kr_list_move_head(cache->list, listnode);
        
        T_KRCacheNode *cachenode = kr_list_value(listnode);
        cachenode->hits++;
        cachenode->last_hit_time = time(NULL);
        return cachenode->value;
    }
    
    return NULL;
}


void kr_cache_set(T_KRCache *cache, void *key, void *value)
{
    int n = kr_hashtable_size(cache->hash_table) - cache->max_size;
    if (n >= 0)
        kr_cache_evict_n_oldest(cache, n+1);
    
    T_KRCacheNode *cachenode = kr_cache_node_new(cache, key, value);
    if (cachenode != NULL) {
        kr_list_add_head(cache->list, cachenode);
        kr_hashtable_replace(cache->hash_table, \
            cachenode->key, kr_list_first(cache->list));
    }
}


void kr_cache_dump(T_KRCache *cache, FILE *fp)
{
    kr_list_foreach(cache->list, (KRForEachFunc )kr_cache_node_dump, fp);
}
