#ifndef __KR_TYPES_H__
#define __KR_TYPES_H__

#ifdef HAVE_CONFIG_H
#include "kr_config.h"
#endif

#include "kr_macros.h"
#include "kr_alloc.h"

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#elif defined HAVE_STDINT_H 
#include <stdint.h>
#else
#error "Need inttypes.h or stdint.h"
#endif

/*types defined in krproject*/
typedef int        kr_bool;
typedef int8_t     kr_char;
typedef int32_t    kr_int;
typedef int64_t    kr_long;
typedef double     kr_double;
typedef char*      kr_string;
typedef void*      kr_pointer;

/*types used in krproject*/
typedef enum {
    KR_TYPE_BOOL    = 'B',
    KR_TYPE_CHAR    = 'C',
    KR_TYPE_INT     = 'I',
    KR_TYPE_LONG    = 'L',
    KR_TYPE_DOUBLE  = 'D',
    KR_TYPE_STRING  = 'S',
    KR_TYPE_POINTER = 'P',
    KR_TYPE_UNKNOWN = 'U'
}E_KRType;

/* Values is used for storing value */
typedef union {
    kr_bool    b;
    kr_char    c;
    kr_int     i;
    kr_long    l;
    kr_double  d;
    kr_string  s;
    kr_pointer p;
}U_KRValue;

/*value indicator*/
typedef enum {
    KR_VALUE_UNSET     = '0', /*未设置*/
    KR_VALUE_SETED     = '1'  /*已设置*/
}E_KRValueInd;

/*external definitions*/
extern unsigned int kr_string_hash(const kr_string s);
extern kr_bool kr_string_equal(kr_string s1, kr_string s2);
extern kr_string kr_string_dup(kr_string s);

/*functions defined in krproject*/
typedef void (*KRFunc)(void *data, void *user_data);
typedef void (*KRFreeFunc)(void *data);
typedef unsigned int (*KRHashFunc)(const void *key);
typedef kr_bool (*KREqualFunc)(const void *a, const void *b);
typedef void *(*KRDupFunc)(void *data);
typedef void (*KRForEachFunc)(void *value, void *data);
typedef int (*KRCompareFunc)(const void *a, const void *b);
typedef int (*KRCompareDataFunc)(const void *a, const void *b, void *data);


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

#endif /* __KR_TYPES_H__ */
