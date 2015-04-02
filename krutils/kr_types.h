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
typedef int            kr_bool;
typedef uint8_t        kr_uchar;
typedef int8_t         kr_char;
typedef uint32_t       kr_uint;
typedef int32_t        kr_int;
typedef uint64_t       kr_ulong;
typedef int64_t        kr_long;
typedef double         kr_double;
typedef char*          kr_string;
typedef void*          kr_pointer;
typedef const void*    kr_constpointer;

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
    KR_VALUE_UNSET     = '0',
    KR_VALUE_SETED     = '1'  
}E_KRValueInd;

/*functions defined in krproject*/
typedef void (*KRFunc)(void *data, void *user_data);
typedef void (*KRFreeFunc)(void *data);
typedef unsigned int (*KRHashFunc)(const void *key);
typedef kr_bool (*KREqualFunc)(const void *a, const void *b);
typedef void *(*KRDupFunc)(void *data);
typedef void (*KRForEachFunc)(void *value, void *data);
typedef int (*KRCompareFunc)(const void *a, const void *b);
typedef int (*KRCompareDataFunc)(const void *a, const void *b, void *data);
typedef void (*KRDestroyNotify)(void *data);

#endif /* __KR_TYPES_H__ */
