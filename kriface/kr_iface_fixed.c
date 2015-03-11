#include "kr_iface.h"

typedef struct _kr_iface_fixed_data_t
{
    int datasrc_id;
    char datasrc_name[100];
    char filename[1024];
    FILE *fp;
}T_KRIfaceFixedData;

T_KRIfaceFixedData *fixed_data = NULL;

/*fixed format*/
void *fixed_define_pre_func(T_DatasrcCur *ptDatasrcCur)
{
    T_KRIfaceFixedData *fixed_data = kr_calloc(sizeof(*fixed_data));

    /* Open Define File */
    fixed_data->datasrc_id = ptDatasrcCur->lOutDatasrcId;
    strcpy(fixed_data->datasrc_name, ptDatasrcCur->caOutDatasrcName);
    snprintf(fixed_data->filename, sizeof(fixed_data->filename), \
                "%s.h", ptDatasrcCur->caOutDatasrcName);
    fixed_data->fp = fopen(fixed_data->filename, "w");
    if (fixed_data->fp == NULL) {
        fprintf(stdout, "open output file: %s failed\n", fixed_data->filename);
        kr_free(fixed_data);
        return NULL;
    }
    /* Generate Define file head*/
    FILE *fp = fixed_data->fp;
    fprintf(fp, "#ifndef __KR_INTERFACE_%d_H__ \n", fixed_data->datasrc_id);
    fprintf(fp, "#define __KR_INTERFACE_%d_H__ \n", fixed_data->datasrc_id);
    fprintf(fp, "#include <stdint.h> \n");
    fprintf(fp, "\n\n");
    fprintf(fp, "typedef struct _kr_interface_%d_t \n", fixed_data->datasrc_id);
    fprintf(fp, "{\n");

    return fixed_data;
}

void fixed_define_post_func(void *data, int flag)
{
    T_KRIfaceFixedData *fixed_data = (T_KRIfaceFixedData *)data;

    if (flag != 0) goto clean;

    /* Generate Define file foot*/
    FILE *fp = fixed_data->fp;
    fprintf(fp, "}T_KRInterface%d;\n", fixed_data->datasrc_id);
    fprintf(fp, "\n\n");
    fprintf(fp, "#endif \n");

clean:
    fclose(fixed_data->fp);
    kr_free(fixed_data);
}

int fixed_define_func(T_DatasrcFieldCur *ptDatasrcFieldCur, void *data)
{
    T_KRIfaceFixedData *fixed_data = (T_KRIfaceFixedData *)data;
    FILE *fp = fixed_data->fp;

    char *name = ptDatasrcFieldCur->caOutFieldName;
    int length = ptDatasrcFieldCur->lOutFieldLength;
    switch(ptDatasrcFieldCur->caOutFieldType[0])
    {
        case KR_TYPE_INT:
            fprintf(fp, "    int %s; \n", name);
            break;
        case KR_TYPE_LONG:
            fprintf(fp, "    long %s; \n", name);
            break;
        case KR_TYPE_DOUBLE:
            fprintf(fp, "    double %s; \n", name);
            break;
        case KR_TYPE_STRING:
            fprintf(fp, "    char %s[%d]; \n", name, length);
            break;
        default:
            break;
    }

    return 0;
}


static inline void *pre_func(T_DatasrcCur *ptDatasrcCur)
{
    FILE *fp = fixed_data->fp;
    fprintf(fp, "typedef struct _kr_interface_%d_t \n", fixed_data->datasrc_id);
    fprintf(fp, "{\n");

    return fixed_data;
}

static inline void post_func(void *data, int flag)
{
    FILE *fp = fixed_data->fp;
    fprintf(fp, "}T_KRInterface%d;\n", fixed_data->datasrc_id);
    fprintf(fp, "\n\n");
}

/*fixed format source*/
void *fixed_source_pre_func(T_DatasrcCur *ptDatasrcCur)
{
    fixed_data = kr_calloc(sizeof(*fixed_data));

    /* Open Source File */
    fixed_data->datasrc_id = ptDatasrcCur->lOutDatasrcId;
    strcpy(fixed_data->datasrc_name, ptDatasrcCur->caOutDatasrcName);
    snprintf(fixed_data->filename, sizeof(fixed_data->filename), \
                "%s.c", ptDatasrcCur->caOutDatasrcName);
    fixed_data->fp = fopen(fixed_data->filename, "w");
    if (fixed_data->fp == NULL) {
        fprintf(stdout, "open output file: %s failed\n", fixed_data->filename);
        kr_free(fixed_data);
        return NULL;
    }

    /*generate pre&post function*/
    FILE *fp = fixed_data->fp;
    fprintf(fp, "#include <stdio.h> \n");
    fprintf(fp, "#include <stdlib.h> \n");
    fprintf(fp, "#include <stdint.h> \n");
    fprintf(fp, "#include <string.h> \n");
    fprintf(fp, "#include <time.h> \n");
    fprintf(fp, "\n");
    /*generate fixed struct definition */
    kr_traversal_fields(ptDatasrcCur, pre_func, fixed_define_func, post_func);
    /* map_pre_func */
    fprintf(fp, "void *map_pre_func_%d(void *msg)\n", fixed_data->datasrc_id);
    fprintf(fp, "{ \n");
    fprintf(fp, "    return msg;\n");
    fprintf(fp, "} \n");
    fprintf(fp, "\n\n");
    /* map_func_post */
    fprintf(fp, "void map_post_func_%d(void *data)\n", fixed_data->datasrc_id);
    fprintf(fp, "{ \n");
    fprintf(fp, "    return;\n");
    fprintf(fp, "} \n");
    fprintf(fp, "\n\n");
    /* map_func */
    fprintf(fp, "void map_func_%d(void *fldval, int fldno, int fldlen, void *data)\n", fixed_data->datasrc_id);
    fprintf(fp, "{ \n");
    fprintf(fp, "    T_KRInterface%d *root = (T_KRInterface%d *)data; \n", \
            fixed_data->datasrc_id, fixed_data->datasrc_id);
    fprintf(fp, "    memset(fldval, 0x00, fldlen); \n");
    fprintf(fp, "    switch(fldno) {\n");

    return fixed_data;
}

void fixed_source_post_func(void *data, int flag)
{
    T_KRIfaceFixedData *fixed_data = (T_KRIfaceFixedData *)data;

    /* map_func footer */
    FILE *fp = fixed_data->fp;
    fprintf(fp, "    default: \n");
    fprintf(fp, "        break; \n");
    fprintf(fp, "    } \n");
    fprintf(fp, "} \n");

clean:
    fclose(fixed_data->fp);
    kr_free(fixed_data);
}

int fixed_source_func(T_DatasrcFieldCur *ptDatasrcFieldCur, void *data)
{
    T_KRIfaceFixedData *fixed_data = (T_KRIfaceFixedData *)data;
    FILE *fp = fixed_data->fp;
    char *name = ptDatasrcFieldCur->caOutFieldName;
    printf("processing field %s \n", name);

    fprintf(fp, "    case %ld: \n", ptDatasrcFieldCur->lOutFieldId);
    fprintf(fp, "    { \n");
    switch(ptDatasrcFieldCur->caOutFieldType[0])
    {
        case KR_TYPE_INT:
            fprintf(fp, "        *(int *)fldval = (int )root->%s;\n", name);
            break;
        case KR_TYPE_LONG:
            fprintf(fp, "        *(long *)fldval = (long )root->%s;\n", name);
            break;
        case KR_TYPE_DOUBLE:
            fprintf(fp, "        *(double *)fldval = (double )root->%s;\n", name);
            break;
        case KR_TYPE_STRING:
            fprintf(fp, "        memcpy(fldval, root->%s, fldlen); \n", name);
            break;
        default:
            break;
    }
    fprintf(fp, "        break; \n");
    fprintf(fp, "    } \n");
    return 0;
}

