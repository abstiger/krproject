#ifndef __KR_HDI_CACHE_H__
#define __KR_HDI_CACHE_H__

#include "krutils/kr_utils.h"
#include "krutils/kr_cache.h"

typedef struct _kr_hdi_cache_value_t
{
    long                 lHDIId;
    E_KRFieldType        eValueType;
    char                 caDate[8+1];
    E_KRValueInd         eValueInd;
    U_KRFieldVal         uValue;
}T_KRHDICacheValue;



T_KRCache *kr_hdi_cache_create(unsigned int cache_size);
void kr_hdi_cache_destroy(T_KRCache *cache);
void kr_hdi_cache_dump(T_KRCache *cache, FILE *fp);
T_KRHDICacheValue *kr_hdi_cache_get(T_KRCache *cache, void *key, long hid);


#endif /* __KR_HDI_CACHE_H__ */
