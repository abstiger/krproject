#include "kr_hdi_cache.h"


static T_KRHDICacheValue *kr_hdi_cache_value_new(long hid)
{
    T_KRHDICacheValue *cache_value = \
        (T_KRHDICacheValue *)kr_malloc(sizeof(T_KRHDICacheValue));
    if (cache_value == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc cache_value Failed!");
        return NULL;
    }
    cache_value->lHDIId = hid;
    cache_value->eValueType = KR_FIELDTYPE_UNKNOWN;
    memset(cache_value->caDate, 0x00, sizeof(cache_value->caDate));
    cache_value->eValueInd = KR_VALUE_UNSET;
    memset(&cache_value->uValue, 0x00, sizeof(cache_value->uValue));
    
    return cache_value;
}


static void kr_hdi_cache_value_free(T_KRHDICacheValue *cache_value)
{
    if (cache_value->eValueType == KR_FIELDTYPE_STRING) {
        kr_free(cache_value->uValue.s);
    }
    kr_free(cache_value);
}


static void 
kr_hdi_cache_value_dump(void *key, T_KRHDICacheValue *cache_value, FILE *fp)
{
    fprintf(fp, "    HDI lHDIId=[%ld], caDate=[%s], eValueInd=[%c], ", \
            cache_value->lHDIId, cache_value->caDate, cache_value->eValueInd);
    switch(cache_value->eValueType)    
    {
        case KR_FIELDTYPE_INT:
            fprintf(fp, "eValueType[%c], uValue.i=[%d]\n", \
                cache_value->eValueType, cache_value->uValue.i);
            break;
        case KR_FIELDTYPE_LONG:
            fprintf(fp, "eValueType[%c], uValue.l=[%ld]\n", \
                cache_value->eValueType, cache_value->uValue.l);
            break;
        case KR_FIELDTYPE_DOUBLE:
            fprintf(fp, "eValueType[%c], uValue.d=[%lf]\n", \
                cache_value->eValueType, cache_value->uValue.d);
            break;
        case KR_FIELDTYPE_STRING:
            fprintf(fp, "eValueType[%c], uValue.i=[%s]\n", \
                cache_value->eValueType, cache_value->uValue.s);
            break;
    }
}


static void kr_hdi_cache_node_dump(T_KRHashTable *cache_table, FILE *fp)
{
    kr_hashtable_foreach(cache_table, (KRHFunc )kr_hdi_cache_value_dump, fp);
}


T_KRCache *kr_hdi_cache_create(unsigned int cache_size)
{
    T_KRCache *cache = kr_cache_new(cache_size, 
                                    kr_string_hash, kr_string_equal,
                                    (KRDupFunc )kr_strdup, kr_free,
                                    NULL, (KRFreeFunc )kr_hashtable_destroy,
                                    (KRFunc )kr_hdi_cache_node_dump);
    if (cache == NULL) {
        KR_LOG(KR_LOGERROR, "kr_cache_new [%u] Failed!", cache_size);
        return NULL;
    }
    
    KR_LOG(KR_LOGDEBUG, "HDI Cache [%u] has been created!", cache_size);
    return cache;
}


void kr_hdi_cache_destroy(T_KRCache *cache)
{
    kr_cache_free(cache);
}


void kr_hdi_cache_dump(T_KRCache *cache, FILE *fp)
{
    kr_cache_dump(cache, fp);
}


T_KRHDICacheValue *kr_hdi_cache_get(T_KRCache *cache, void *key, long hid)
{
    T_KRHDICacheValue *hdi_cache_val = NULL;
    T_KRHashTable *node = kr_cache_get(cache, key);
    if (node != NULL) {
        hdi_cache_val = kr_hashtable_lookup(node, &hid);
    } else {
        node = kr_hashtable_new_full(kr_long_hash, kr_long_equal, \
                            NULL, (KRDestroyNotify )kr_hdi_cache_value_free);
        kr_cache_set(cache, key, node);
    }
    
    if (hdi_cache_val == NULL) {
        hdi_cache_val = kr_hdi_cache_value_new(hid);
        kr_hashtable_replace(node, &hdi_cache_val->lHDIId, hdi_cache_val);
    }

    return hdi_cache_val;
}