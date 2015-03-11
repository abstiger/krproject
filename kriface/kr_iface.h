#ifndef __KR_IFACE_H__
#define __KR_IFACE_H__

#include "krutils/kr_utils.h"
#include "dbs/dbs/datasrc_cur.h"
#include "dbs/dbs/datasrc_field_cur.h"

typedef void *(*DatasrcFieldPreFunc)(T_DatasrcCur *ptDatasrcCur);
typedef int (*DatasrcFieldFunc)(T_DatasrcFieldCur *ptDatasrcFieldCur, void *data);
typedef void (*DatasrcFieldPostFunc)(void *data, int flag);

/*fixed format*/
void *fixed_define_pre_func(T_DatasrcCur *ptDatasrcCur);
int fixed_define_func(T_DatasrcFieldCur *ptDatasrcFieldCur, void *value);
void fixed_define_post_func(void *data, int flag);

void *fixed_source_pre_func(T_DatasrcCur *ptDatasrcCur);
int fixed_source_func(T_DatasrcFieldCur *ptDatasrcFieldCur, void *value);
void fixed_source_post_func(void *data, int flag);

/*json format*/
void *json_define_pre_func(T_DatasrcCur *ptDatasrcCur);
int json_define_func(T_DatasrcFieldCur *ptDatasrcFieldCur, void *value);
void json_define_post_func(void *data, int flag);

void *json_source_pre_func(T_DatasrcCur *ptDatasrcCur);
int json_source_func(T_DatasrcFieldCur *ptDatasrcFieldCur, void *value);
void json_source_post_func(void *data, int flag);

/*avro format*/
void *avro_define_pre_func(T_DatasrcCur *ptDatasrcCur);
int avro_define_func(T_DatasrcFieldCur *ptDatasrcFieldCur, void *value);
void avro_define_post_func(void *data, int flag);

void *avro_source_pre_func(T_DatasrcCur *ptDatasrcCur);
int avro_source_func(T_DatasrcFieldCur *ptDatasrcFieldCur, void *value);
void avro_source_post_func(void *data, int flag);


typedef struct _kr_iface_format_t
{
    char format_name[10];
    char file_code[10];

    /*functions for this format*/
    DatasrcFieldPreFunc datasrc_field_pre_func;
    DatasrcFieldFunc datasrc_field_func;
    DatasrcFieldPostFunc datasrc_field_post_func;
}T_KRIfaceFormat;

int kr_traversal_fields(T_DatasrcCur *ptDatasrcCur, 
        DatasrcFieldPreFunc pre_func,
        DatasrcFieldFunc func,
        DatasrcFieldPostFunc post_func);

#endif /* __KR_IFACE_H__ */
