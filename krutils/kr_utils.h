#ifndef __KR_UTILS_H__
#define __KR_UTILS_H__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "kr_macros.h"
#include "kr_types.h"
#include "kr_alloc.h"
#include "kr_json.h"
#include "kr_string.h"
#include "kr_datetime.h"
#include "kr_list.h"
#include "kr_ntree.h"
#include "kr_hashtable.h"
#include "kr_functable.h"
#include "kr_hashset.h"
#include "kr_regex.h"
#include "kr_module.h"
#include "kr_log.h"
#include "kr_net.h"


/* Various kinds of Hash Functions */
static inline unsigned int kr_int_hash(const void *v)
{
    return (unsigned int)*(const kr_int*) v;
}

static inline unsigned int kr_long_hash(const void *v)
{
    return (unsigned int) *(const kr_long*) v;
}

static inline unsigned int kr_double_hash(const void *v)
{
    return (unsigned int) *(const kr_double*) v;
}

static inline unsigned int kr_pointer_hash(const void *v)
{
    return (unsigned int) (const size_t) v;
}


/* Various kinds of Equal Functions */
static inline kr_bool kr_int_equal(const void * v1, const void * v2)
{
    return *((const kr_int*) v1) == *((const kr_int*) v2);
}

static inline kr_bool kr_long_equal(const void * v1, const void * v2)
{
    return *((const kr_long*) v1) == *((const kr_long*) v2);
}

static inline kr_bool kr_double_equal(const void * v1, const void * v2)
{
    return *((const kr_double*) v1) == *((const kr_double*) v2);
}

static inline kr_bool kr_pointer_equal(const void * v1, const void * v2)
{
    return v1 == v2;
}


/* Various kinds of Compare Functions */
static inline int kr_int_compare(const void * v1, const void * v2)
{
    return (int )(*((const kr_int*) v1) - *((const kr_int*) v2));
}

static inline int kr_long_compare(const void * v1, const void * v2)
{
    return (int )(*((const kr_long*) v1) - *((const kr_long*) v2));
}

static inline int kr_double_compare(const void * v1, const void * v2)
{
    return (int )(*((const kr_double*) v1) - *((const kr_double*) v2));
}

static inline int kr_pointer_compare(const void * v1, const void * v2)
{
    return (int )(v1 - v2);
}

/* Various kinds of Duplicate Functions */
static inline void *kr_int_dup(void *key)
{
    kr_int *i = (kr_int *)kr_malloc(sizeof(kr_int));
    *i = *(kr_int *)key;
    return (void *)i;
}

static inline void *kr_long_dup(void *key)
{
    kr_long *l = (kr_long *)kr_malloc(sizeof(kr_long));
    *l = *(kr_long *)key;
    return (void *)l;
}

static inline void *kr_double_dup(void *key)
{
    kr_double *d = (kr_double *)kr_malloc(sizeof(kr_double));
    *d = *(kr_double *)key;
    return (void *)d;
}

static inline void *kr_pointer_dup(void *key)
{
    kr_pointer p = (kr_pointer)kr_malloc(sizeof(kr_pointer));
    p = (kr_pointer)key;
    return (void *)p;
}


/*useful functions for krproject*/
static inline void* kr_get_hash_func(E_KRType type)
{
    switch(type) {
        case KR_TYPE_INT: return kr_int_hash;
        case KR_TYPE_LONG: return kr_long_hash;
        case KR_TYPE_DOUBLE: return kr_double_hash;
        case KR_TYPE_STRING: return kr_string_hash;
        case KR_TYPE_POINTER: return kr_pointer_hash;
        default: return NULL;
    }
}

static inline void* kr_get_equal_func(E_KRType type)
{
    switch(type) {
        case KR_TYPE_INT: return kr_int_equal;
        case KR_TYPE_LONG: return kr_long_equal;
        case KR_TYPE_DOUBLE: return kr_double_equal;
        case KR_TYPE_STRING: return kr_string_equal;
        case KR_TYPE_POINTER: return kr_pointer_equal;
        default: return NULL;
    }
}


static inline void* kr_get_compare_func(E_KRType type)
{
    switch(type) {
        case KR_TYPE_INT: return kr_int_compare;
        case KR_TYPE_LONG: return kr_long_compare;
        case KR_TYPE_DOUBLE: return kr_double_compare;
        case KR_TYPE_STRING: return kr_string_compare;
        case KR_TYPE_POINTER: return kr_pointer_compare;
        default: return NULL;
    }
}

static inline void* kr_get_dup_func(E_KRType type)
{
    switch(type) {
        case KR_TYPE_INT: return kr_int_dup;
        case KR_TYPE_LONG: return kr_long_dup;
        case KR_TYPE_DOUBLE: return kr_double_dup;
        case KR_TYPE_STRING: return kr_string_dup;
        case KR_TYPE_POINTER: return kr_pointer_dup;
        default: return NULL;
    }
}


static inline void* kr_get_value(U_KRValue *value, E_KRType type)
{
    switch(type) {
        case KR_TYPE_BOOL: return &value->b;
        case KR_TYPE_INT: return &value->i;
        case KR_TYPE_LONG: return &value->l;    
        case KR_TYPE_DOUBLE: return &value->d;
        case KR_TYPE_STRING: return value->s;
        case KR_TYPE_POINTER: return value->p;
        default: return NULL;
    }
}

#endif /* __KR_UTILS_H__ */
