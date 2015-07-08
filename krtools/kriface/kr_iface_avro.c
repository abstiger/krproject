#include "kr_iface.h"

typedef struct _kr_iface_avro_data_t
{
    int datasrc_id;
    cJSON *schema;
    char filename[1024];
    FILE *fp;
}T_KRIfaceAvroData;
T_KRIfaceAvroData *avro_data = NULL;

/*avro format*/
void *avro_define_pre_func(T_KRParamInput *ptParamInput)
{
    T_KRIfaceAvroData *avro_data = kr_calloc(sizeof(*avro_data));
    /* Create Schema json */
    avro_data->schema = cJSON_CreateObject();
    cJSON_AddStringToObject(avro_data->schema, "type", "record");
    cJSON_AddStringToObject(avro_data->schema, "name", ptParamInput->caInputName);
    cJSON *fields = cJSON_CreateArray();
    cJSON_AddItemToObject(avro_data->schema, "fields", fields);

    /* Open Schema File */
    avro_data->datasrc_id = ptParamInput->lInputId;
    snprintf(avro_data->filename, sizeof(avro_data->filename), \
            "%s.avsc", ptParamInput->caInputName);
    avro_data->fp = fopen(avro_data->filename, "w");
    if (avro_data->fp == NULL) {
        fprintf(stdout, "open output file: %s failed\n", avro_data->filename);
        cJSON_Delete(avro_data->schema);
        kr_free(avro_data);
        return NULL;
    }
    return avro_data;
}

void avro_define_post_func(void *data, int flag)
{
    T_KRIfaceAvroData *avro_data = (T_KRIfaceAvroData *)data;

    if (flag != 0) goto clean;
        
    /* Dump schema to file */
    char *schema_string = cJSON_Print(avro_data->schema);
    fprintf(avro_data->fp, "%s \n", schema_string);
    kr_free(schema_string);

clean:
    /* Delete Schema*/
    cJSON_Delete(avro_data->schema);
    fclose(avro_data->fp);
    kr_free(avro_data);
}

int avro_define_func(T_KRParamInputField *ptParamInputField, void *data)
{
    T_KRIfaceAvroData *avro_data = (T_KRIfaceAvroData *)data;
    cJSON *fields = cJSON_GetObjectItem(avro_data->schema, "fields");

    cJSON *field = cJSON_CreateObject();
    cJSON_AddStringToObject(field, "name", ptParamInputField->caFieldName);
    switch(ptParamInputField->caFieldType[0])
    {
        case KR_TYPE_INT:
            cJSON_AddStringToObject(field, "type", "int");
            break;
        case KR_TYPE_LONG:
            cJSON_AddStringToObject(field, "type", "long");
            break;
        case KR_TYPE_DOUBLE:
            cJSON_AddStringToObject(field, "type", "double");
            break;
        case KR_TYPE_STRING:
            cJSON_AddStringToObject(field, "type", "string");
            break;
        default:
            break;
    }
    cJSON_AddNumberToObject(field, "length", ptParamInputField->lFieldLength);
    cJSON_AddItemToArray(fields, field);
}


static inline void *pre_func(T_KRParamInput *ptParamInput)
{
    /* Create Schema json */
    avro_data->schema = cJSON_CreateObject();
    cJSON_AddStringToObject(avro_data->schema, "type", "record");
    cJSON_AddStringToObject(avro_data->schema, "name", ptParamInput->caInputName);
    cJSON *fields = cJSON_CreateArray();
    cJSON_AddItemToObject(avro_data->schema, "fields", fields);

    return avro_data;
}

static inline void post_func(void *data, int flag)
{
    
}
/*avro format source*/
void *avro_source_pre_func(T_KRParamInput *ptParamInput)
{
    avro_data = kr_calloc(sizeof(*avro_data));

    /* Open Source File */
    avro_data->datasrc_id = ptParamInput->lInputId;
    snprintf(avro_data->filename, sizeof(avro_data->filename), \
            "%s_avro.c", ptParamInput->caInputName);
    avro_data->fp = fopen(avro_data->filename, "w");
    if (avro_data->fp == NULL) {
        fprintf(stdout, "open output file: %s failed\n", avro_data->filename);
        kr_free(avro_data);
        return NULL;
    }
    /* Create Schema json */
    kr_traversal_fields(ptParamInput, pre_func, avro_define_func, post_func);

    /*generate pre&post function*/
    FILE *fp = avro_data->fp;
    fprintf(fp, "#include <stdio.h> \n");
    fprintf(fp, "#include <stdlib.h> \n");
    fprintf(fp, "#include <string.h> \n");
    fprintf(fp, "#include <time.h> \n");
    fprintf(fp, "#include <avro.h> \n");
    fprintf(fp, "\n\n");
    fprintf(fp, "#define STR(...) #__VA_ARGS__\n");
    fprintf(fp, "\n\n");
    /* map_pre_func */
    char *schema_string = cJSON_Print(avro_data->schema);
    fprintf(fp, "void *avro_map_pre_func_%d(void *msg)\n", avro_data->datasrc_id);
    fprintf(fp, "{ \n");
    fprintf(fp, "    static const char SCHEMA_JSON[] = STR(%s); \n", schema_string);
    fprintf(fp, "    avro_schema_t  record_schema = NULL; \n");
    fprintf(fp, "    if (avro_schema_from_json_literal(SCHEMA_JSON, &record_schema)) { \n");
    fprintf(fp, "       fprintf(stderr, \"Error parsing schema:\\n  %%s\\n\", avro_strerror()); \n");
    fprintf(fp, "       return NULL; \n");
    fprintf(fp, "    } \n");
    fprintf(fp, "    \n");
    fprintf(fp, "    avro_value_iface_t  *record_class = avro_generic_class_from_schema(record_schema); \n");
    fprintf(fp, "    avro_reader_t  reader = avro_reader_memory(msg, strlen(msg)); \n");
    fprintf(fp, "    avro_value_t  *pval = malloc(sizeof(*pval)); \n");
    fprintf(fp, "    if (pval == NULL) { \n");
    fprintf(fp, "       fprintf(stderr, \"Error malloc pval:\\n\"); \n");
    fprintf(fp, "       return NULL; \n");
    fprintf(fp, "    } \n");
    fprintf(fp, "    if (avro_generic_value_new(record_class, pval)) { \n");
    fprintf(fp, "       fprintf(stderr, \"Error create record:\\n  %%s\\n\", avro_strerror()); \n");
    fprintf(fp, "       return NULL; \n");
    fprintf(fp, "    } \n");
    fprintf(fp, "    if (avro_value_read(reader, pval)) { \n");
    fprintf(fp, "       fprintf(stderr, \"Unable to read value:\\n  %%s\\n\", avro_strerror()); \n");
    fprintf(fp, "       return NULL; \n");
    fprintf(fp, "    } \n");
    fprintf(fp, "    avro_reader_free(reader); \n");
    fprintf(fp, "    return pval; \n");
    fprintf(fp, "} \n");
    fprintf(fp, "\n\n");
    /* map_func_post */
    fprintf(fp, "void avro_map_post_func_%d(void *data)\n", avro_data->datasrc_id);
    fprintf(fp, "{ \n");
    fprintf(fp, "    avro_value_t *pval = (avro_value_t *)data; \n");
    fprintf(fp, "    avro_value_decref(pval); \n");
    fprintf(fp, "    free(pval); \n");
    fprintf(fp, "} \n");
    fprintf(fp, "\n\n");
    /* map_func header */
    fprintf(fp, "void avro_map_func_%d(void *fldval, int fldno, int fldlen, void *data)\n", avro_data->datasrc_id);
    fprintf(fp, "{ \n");
    fprintf(fp, "    avro_value_t *pval = (avro_value_t *)data; \n");
    fprintf(fp, "    memset(fldval, 0x00, fldlen); \n");
    fprintf(fp, "    switch(fldno) {\n");

    return avro_data;
}

void avro_source_post_func(void *data, int flag)
{
    T_KRIfaceAvroData *avro_data = (T_KRIfaceAvroData *)data;

    /* map_func footer */
    FILE *fp = avro_data->fp;
    fprintf(fp, "    default: \n");
    fprintf(fp, "        break; \n");
    fprintf(fp, "    } \n");
    fprintf(fp, "} \n");

clean:
    fclose(avro_data->fp);
    kr_free(avro_data);
}

int avro_source_func(T_KRParamInputField *ptParamInputField, void *data)
{
    T_KRIfaceAvroData *avro_data = (T_KRIfaceAvroData *)data;
    FILE *fp = avro_data->fp;
    int id = ptParamInputField->lFieldId;
    char *name = ptParamInputField->caFieldName;
    printf("processing field %s \n", name);

    fprintf(fp, "    case %d: \n", id);
    fprintf(fp, "    { \n");
    fprintf(fp, "        avro_value_t field; \n");
    fprintf(fp, "        avro_value_get_by_name(pval, \"%s\", &field, NULL); \n", name);
    switch(ptParamInputField->caFieldType[0])
    {
        case KR_TYPE_INT:
            fprintf(fp, "        avro_value_get_int(&field, fldval);\n");
            break;
        case KR_TYPE_LONG:
            fprintf(fp, "        avro_value_get_long(&field, fldval);\n");
            break;
        case KR_TYPE_DOUBLE:
            fprintf(fp, "        avro_value_get_double(&field, fldval);\n");
            break;
        case KR_TYPE_STRING:
            fprintf(fp, "        avro_value_get_string(&field, fldval, (size_t *)&fldlen);\n");
            break;
        default:
            break;
    }
    fprintf(fp, "        break; \n");
    fprintf(fp, "    } \n");
    return 0;
}

