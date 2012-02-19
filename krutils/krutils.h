#ifndef __KR_UTILS_H__
#define __KR_UTILS_H__

#include "krtypes.h"

typedef enum {
    KR_INTEGER = '0', 
    KR_DOUBLE, 
    KR_CHAR
}KRType;

/*record description, referred to transaction in our system mainly*/
typedef struct _kr_field_t
{
    gint        id;
    gchar       name[30+1];
    KRType      type;
    gint        length;
    gint        precision;
    gint        scale;
    gint        offset;
}T_KRFieldDef;

typedef union {
    gint       i;
    gdouble    d;
    gchar     *s;
}T_KRFieldVal;

typedef enum {
    KR_INDEX_TABLE = '0', 
    KR_INDEX_DB 
}KRIndexType;

typedef enum {
    KR_KEEP_RECORD_COUNT = '0', 
    KR_KEEP_TRANS_INTERVAL 
}KRSizeKeepMode;

typedef struct _kr_record_t
{
    gpointer        ptTable;
    gchar          *pRecBuf;
}T_KRRecord;


static inline void*
kr_get_hash_func(KRType fieldtype)
{
    switch(fieldtype)
    {
        case KR_INTEGER:
            return g_int_hash;
        case KR_DOUBLE:
            return g_double_hash;
        case KR_CHAR:
            return g_str_hash;
        default:
            return g_direct_hash;
    }
}


static inline void*
kr_get_key_equal_func(KRType fieldtype)
{
    switch(fieldtype)
    {
        case KR_INTEGER:
            return g_int_equal;
        case KR_DOUBLE:
            return g_double_equal;
        case KR_CHAR:
            return g_str_equal;
        default:
            return g_direct_equal;
    }
}

#endif /* __KR_UTILS_H__ */
