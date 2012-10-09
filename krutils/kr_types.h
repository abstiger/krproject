#ifndef __KR_TYPES_H__
#define __KR_TYPES_H__

#include "kr_macros.h"

typedef int (*KRCompareFunc)(const void *a, const void *b);
typedef int (*KRCompareDataFunc)(const void *a, const void *b, void *data);
typedef void (*KRFunc)(void *data, void *user_data);
typedef void (*KRHFunc)(void *key, void *value, void *data);
typedef void (*KRFreeFunc)(void *data);
typedef void *(*KRDupFunc)(void *data);

typedef boolean  (*KRHRFunc)(void *key, void *value, void *user_data);
typedef unsigned int (*KRHashFunc)(const void *key);
typedef boolean (*KREqualFunc)(const void *a, const void *b);
typedef void (*KRDestroyNotify)(void *data);

typedef void (*KRForEachFunc)(void *value, void *data);

#endif /* __KR_TYPES_H__ */
