#include "kr_iface.h"

typedef struct _kr_iface_json_data_t
{
    int datasrc_id;
    cJSON *schema;
    char filename[1024];
    FILE *fp;
}T_KRIfaceJsonData;

/*json format*/
void *json_input_define_pre_func(T_KRParamInput *ptParamInput)
{
    T_KRIfaceJsonData *json_data = kr_calloc(sizeof(*json_data));
    /* Create Schema json */
    json_data->schema = cJSON_CreateObject();

    /* Open Schema File */
    json_data->datasrc_id = ptParamInput->lInputId;
    snprintf(json_data->filename, sizeof(json_data->filename), \
                "input_%s.json", ptParamInput->caInputName);
    json_data->fp = fopen(json_data->filename, "w");
    if (json_data->fp == NULL) {
        fprintf(stdout, "open output file: %s failed\n", json_data->filename);
        cJSON_Delete(json_data->schema);
        kr_free(json_data);
        return NULL;
    }
    return json_data;
}

void json_input_define_post_func(void *data)
{
    T_KRIfaceJsonData *json_data = (T_KRIfaceJsonData *)data;

    /* Dump schema to file */
    if (json_data->schema) {
        char *schema_string = cJSON_Print(json_data->schema);
        fprintf(json_data->fp, "%s \n", schema_string);
        kr_free(schema_string);
    }

clean:
    /* Delete Schema*/
    cJSON_Delete(json_data->schema);
    fclose(json_data->fp);
    kr_free(json_data);
}

int json_input_define_func(T_KRParamInputField *ptParamInputField, void *data)
{
    T_KRIfaceJsonData *json_data = (T_KRIfaceJsonData *)data;
    cJSON *schema = (cJSON *)json_data->schema;

    char *name = ptParamInputField->caFieldName;
    switch(ptParamInputField->caFieldType[0])
    {
        case KR_TYPE_INT:
            cJSON_AddNumberToObject(schema, name, 0);
            break;
        case KR_TYPE_LONG:
            cJSON_AddNumberToObject(schema, name, 10);
            break;
        case KR_TYPE_DOUBLE:
            cJSON_AddNumberToObject(schema, name, 0.0);
            break;
        case KR_TYPE_STRING:
            cJSON_AddStringToObject(schema, name, " ");
            break;
        default:
            break;
    }

    return 0;
}


/*json format input*/
void *json_input_source_pre_func(T_KRParamInput *ptParamInput)
{
    T_KRIfaceJsonData *json_data = kr_calloc(sizeof(*json_data));

    /* Open Source File */
    json_data->datasrc_id = ptParamInput->lInputId;
    snprintf(json_data->filename, sizeof(json_data->filename), \
                "input_%s_json.c", ptParamInput->caInputName);
    json_data->fp = fopen(json_data->filename, "w");
    if (json_data->fp == NULL) {
        fprintf(stdout, "open output file: %s failed\n", json_data->filename);
        kr_free(json_data);
        return NULL;
    }

    /*generate pre&post function*/
    FILE *fp = json_data->fp;
    fprintf(fp, "#include <stdio.h> \n");
    fprintf(fp, "#include <stdlib.h> \n");
    fprintf(fp, "#include <string.h> \n");
    fprintf(fp, "#include <time.h> \n");
    fprintf(fp, "#include <jansson.h> \n");
    fprintf(fp, "\n\n");
    /* map_pre_func */
    fprintf(fp, "void *json_input_source_pre_func_%d(void *msg)\n", json_data->datasrc_id);
    fprintf(fp, "{ \n");
    fprintf(fp, "    json_t  *root; \n");
    fprintf(fp, "    json_error_t  json_error; \n");
    fprintf(fp, "    root = json_loads(msg, 0, &json_error); \n");
    fprintf(fp, "    return root;\n");
    fprintf(fp, "} \n");
    fprintf(fp, "\n\n");
    /* map_func_post */
    fprintf(fp, "void json_input_source_post_func_%d(void *data)\n", json_data->datasrc_id);
    fprintf(fp, "{ \n");
    fprintf(fp, "    json_t  *root = (json_t *)data; \n");
    fprintf(fp, "    json_decref(root); \n");
    fprintf(fp, "} \n");
    fprintf(fp, "\n\n");
    /* map_func header */
    fprintf(fp, "void json_input_source_func_%d(void *fldval, int fldno, int fldlen, void *data)\n", json_data->datasrc_id);
    fprintf(fp, "{ \n");
    fprintf(fp, "    json_t *root = (json_t *)data; \n");
    fprintf(fp, "    memset(fldval, 0x00, fldlen); \n");
    fprintf(fp, "    switch(fldno) {\n");

    return json_data;
}

void json_input_source_post_func(void *data)
{
    T_KRIfaceJsonData *json_data = (T_KRIfaceJsonData *)data;

    /* map_func footer */
    FILE *fp = json_data->fp;
    fprintf(fp, "    default: \n");
    fprintf(fp, "        break; \n");
    fprintf(fp, "    } \n");
    fprintf(fp, "} \n");

clean:
    fclose(json_data->fp);
    kr_free(json_data);
}

int json_input_source_func(T_KRParamInputField *ptParamInputField, void *data)
{
    T_KRIfaceJsonData *json_data = (T_KRIfaceJsonData *)data;
    FILE *fp = json_data->fp;
    char *name = ptParamInputField->caFieldName;
    printf("processing field %s \n", name);

    fprintf(fp, "    case %ld: \n", ptParamInputField->lFieldId);
    fprintf(fp, "    { \n");
    fprintf(fp, "        json_t *field = json_object_get(root, \"%s\"); \n", name);
    switch(ptParamInputField->caFieldType[0])
    {
        case KR_TYPE_INT:
            fprintf(fp, "        *(int *)fldval = (int )json_integer_value(field);\n");
            break;
        case KR_TYPE_LONG:
            fprintf(fp, "        *(long *)fldval = (long )json_integer_value(field);\n");
            break;
        case KR_TYPE_DOUBLE:
            fprintf(fp, "        *(double *)fldval = (double )json_real_value(field);\n");
            break;
        case KR_TYPE_STRING:
            fprintf(fp, "        memcpy(fldval, json_string_value(field), fldlen); \n");
            break;
        default:
            break;
    }
    fprintf(fp, "        break; \n");
    fprintf(fp, "    } \n");
    return 0;
}

/*json format*/
void *json_output_define_pre_func(T_KRParamOutput *ptParamOutput)
{
    T_KRIfaceJsonData *json_data = kr_calloc(sizeof(*json_data));
    /* Create Schema json */
    json_data->schema = cJSON_CreateObject();

    /* Open Schema File */
    json_data->datasrc_id = ptParamOutput->lOutputId;
    snprintf(json_data->filename, sizeof(json_data->filename), \
                "output_%s.json", ptParamOutput->caOutputName);
    json_data->fp = fopen(json_data->filename, "w");
    if (json_data->fp == NULL) {
        fprintf(stdout, "open output file: %s failed\n", json_data->filename);
        cJSON_Delete(json_data->schema);
        kr_free(json_data);
        return NULL;
    }
    return json_data;
}

void json_output_define_post_func(void *data)
{
    T_KRIfaceJsonData *json_data = (T_KRIfaceJsonData *)data;

    /* Dump schema to file */
    if (json_data->schema) {
        char *schema_string = cJSON_Print(json_data->schema);
        fprintf(json_data->fp, "%s \n", schema_string);
        kr_free(schema_string);
    }

clean:
    /* Delete Schema*/
    cJSON_Delete(json_data->schema);
    fclose(json_data->fp);
    kr_free(json_data);
}

int json_output_define_func(T_KRParamOutputField *ptParamOutputField, void *data)
{
    T_KRIfaceJsonData *json_data = (T_KRIfaceJsonData *)data;
    cJSON *schema = (cJSON *)json_data->schema;

    char *name = ptParamOutputField->caFieldName;
    switch(ptParamOutputField->caFieldType[0])
    {
        case KR_TYPE_INT:
            cJSON_AddNumberToObject(schema, name, 0);
            break;
        case KR_TYPE_LONG:
            cJSON_AddNumberToObject(schema, name, 10);
            break;
        case KR_TYPE_DOUBLE:
            cJSON_AddNumberToObject(schema, name, 0.0);
            break;
        case KR_TYPE_STRING:
            cJSON_AddStringToObject(schema, name, " ");
            break;
        default:
            break;
    }

    return 0;
}


/*json format output*/
void *json_output_source_pre_func(T_KRParamOutput *ptParamOutput)
{
    T_KRIfaceJsonData *json_data = kr_calloc(sizeof(*json_data));

    /* Open Source File */
    json_data->datasrc_id = ptParamOutput->lOutputId;
    snprintf(json_data->filename, sizeof(json_data->filename), \
                "output_%s_json.c", ptParamOutput->caOutputName);
    json_data->fp = fopen(json_data->filename, "w");
    if (json_data->fp == NULL) {
        fprintf(stdout, "open output file: %s failed\n", json_data->filename);
        kr_free(json_data);
        return NULL;
    }

    /*generate pre&post function*/
    FILE *fp = json_data->fp;
    fprintf(fp, "#include <stdio.h> \n");
    fprintf(fp, "#include <stdlib.h> \n");
    fprintf(fp, "#include <string.h> \n");
    fprintf(fp, "#include <time.h> \n");
    fprintf(fp, "#include <jansson.h> \n");
    fprintf(fp, "\n\n");
    /* map_pre_func */
    fprintf(fp, "void *json_output_source_pre_func_%d(void)\n", json_data->datasrc_id);
    fprintf(fp, "{ \n");
    fprintf(fp, "    json_t  *root = json_object(); \n");
    fprintf(fp, "    return root;\n");
    fprintf(fp, "} \n");
    fprintf(fp, "\n\n");
    /* map_func_post */
    fprintf(fp, "void json_output_source_post_func_%d(void *data, size_t *msglen, char **msgbuf)\n", json_data->datasrc_id);
    fprintf(fp, "{ \n");
    fprintf(fp, "    json_t  *root = (json_t *)data; \n");
    fprintf(fp, "    *msgbuf = json_dumps(root, 0); \n");
    fprintf(fp, "    *msglen = strlen(*msgbuf); \n");
    fprintf(fp, "    json_decref(root); \n");
    fprintf(fp, "} \n");
    fprintf(fp, "\n\n");
    /* map_func header */
    fprintf(fp, "void json_output_source_func_%d(void *data, int fldno, int fldlen, void *fldval)\n", json_data->datasrc_id);
    fprintf(fp, "{ \n");
    fprintf(fp, "    json_t *root = (json_t *)data; \n");
    fprintf(fp, "    switch(fldno) {\n");

    return json_data;
}

void json_output_source_post_func(void *data)
{
    T_KRIfaceJsonData *json_data = (T_KRIfaceJsonData *)data;

    /* map_func footer */
    FILE *fp = json_data->fp;
    fprintf(fp, "    default: \n");
    fprintf(fp, "        break; \n");
    fprintf(fp, "    } \n");
    fprintf(fp, "} \n");

clean:
    fclose(json_data->fp);
    kr_free(json_data);
}

int json_output_source_func(T_KRParamOutputField *ptParamOutputField, void *data)
{
    T_KRIfaceJsonData *json_data = (T_KRIfaceJsonData *)data;
    FILE *fp = json_data->fp;
    char *name = ptParamOutputField->caFieldName;
    
    fprintf(stderr,"processing field %s \n", name);

    fprintf(fp, "    case %ld: \n", ptParamOutputField->lFieldId);
    fprintf(fp, "    { \n");
    switch(ptParamOutputField->caFieldType[0])
    {
        case KR_TYPE_INT:
            fprintf(fp, "        json_t *field = json_integer(*(int *)fldval);\n");
            break;
        case KR_TYPE_LONG:
            fprintf(fp, "        json_t *field = json_integer(*(long *)fldval);\n");
            break;
        case KR_TYPE_DOUBLE:
            fprintf(fp, "        json_t *field = json_real(*(double *)fldval);\n");
            break;
        case KR_TYPE_STRING:
            fprintf(fp, "        json_t *field = json_stringn((char *)fldval, fldlen); \n");
            break;
        default:
            break;
    }
    fprintf(fp, "        json_object_set_new(root, \"%s\", field); \n", name);
    fprintf(fp, "        break; \n");
    fprintf(fp, "    } \n");
    return 0;
}

