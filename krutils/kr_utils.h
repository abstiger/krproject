#ifndef __KR_UTILS_H__
#define __KR_UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "kr_macros.h"
#include "kr_types.h"
#include "kr_alloc.h"
#include "kr_string.h"
#include "kr_datetime.h"
#include "kr_list.h"
#include "kr_hashtable.h"
#include "kr_set.h"
#include "kr_regex.h"
#include "kr_module.h"
#include "kr_log.h"
#include "kr_cfgparser.h"
#include "kr_net.h"
#include "kr_event.h"

/*value indicator*/
typedef enum {
    KR_VALUE_UNSET     = '0', /*Œ¥…Ë÷√*/
    KR_VALUE_SETED     = '1'  /*“—…Ë÷√*/
}E_KRValueInd;


/*field types used in the krproject*/
typedef enum {
    KR_FIELDTYPE_INT     = 'I', 
    KR_FIELDTYPE_LONG    = 'L',
    KR_FIELDTYPE_DOUBLE  = 'D',
    KR_FIELDTYPE_STRING  = 'S',
    KR_FIELDTYPE_POINTER = 'P',
    KR_FIELDTYPE_UNKNOWN = 'U'
}E_KRFieldType;

/*field description*/
typedef struct _kr_field_def_t
{
    int           id;
    char          name[30+1];
    E_KRFieldType type;
    int           length;
    int           offset;
}T_KRFieldDef;

/*field value union*/
typedef union {
    int           i;
    long          l;
    double        d;
    char         *s;
}U_KRFieldVal;


static inline void*
kr_get_hash_func(E_KRFieldType field_type)
{
    switch(field_type)
    {
        case KR_FIELDTYPE_INT: return kr_int_hash;
        case KR_FIELDTYPE_LONG: return kr_long_hash;    
        case KR_FIELDTYPE_DOUBLE: return kr_double_hash;
        case KR_FIELDTYPE_STRING: return kr_string_hash;
        case KR_FIELDTYPE_POINTER: return kr_pointer_hash;
        default: return NULL;
    }
}


static inline void*
kr_get_equal_func(E_KRFieldType field_type)
{
    switch(field_type)
    {
        case KR_FIELDTYPE_INT: return kr_int_equal;
        case KR_FIELDTYPE_LONG: return kr_long_equal;    
        case KR_FIELDTYPE_DOUBLE: return kr_double_equal;
        case KR_FIELDTYPE_STRING: return kr_string_equal;
        case KR_FIELDTYPE_POINTER: return kr_pointer_equal;
        default: return NULL;
    }
}

static inline void*
kr_get_dup_func(E_KRFieldType field_type)
{
    switch(field_type)
    {
        case KR_FIELDTYPE_INT: return kr_int_dup;
        case KR_FIELDTYPE_LONG: return kr_long_dup;    
        case KR_FIELDTYPE_DOUBLE: return kr_double_dup;
        case KR_FIELDTYPE_STRING: return kr_string_dup;
        case KR_FIELDTYPE_POINTER: return kr_pointer_dup;
        default: return NULL;
    }
}
#endif /* __KR_UTILS_H__ */
