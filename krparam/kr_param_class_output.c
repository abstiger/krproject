#include "kr_param_class_output.h"
#include "krutils/kr_utils.h"

static cJSON *kr_param_output_field_serialize(T_KRParamOutputField *ptParamOutputField)
{
    cJSON *field = cJSON_CreateObject();
    cJSON_AddNumberToObject(field, "id", ptParamOutputField->lFieldId);
    cJSON_AddStringToObject(field, "name", ptParamOutputField->caFieldName);
    cJSON_AddStringToObject(field, "desc", ptParamOutputField->caFieldDesc);
    cJSON_AddStringToObject(field, "type", ptParamOutputField->caFieldType);
    cJSON_AddNumberToObject(field, "length", ptParamOutputField->lFieldLength);
    return field;
}

char *kr_param_output_serialize(T_KRParamOutput *ptParamOutput)
{
    cJSON *output = cJSON_CreateObject();
    cJSON_AddNumberToObject(output, "id", ptParamOutput->lOutputId);
    cJSON_AddStringToObject(output, "name", ptParamOutput->caOutputName);
    cJSON_AddStringToObject(output, "desc", ptParamOutput->caOutputDesc);
    cJSON *fields = cJSON_CreateArray();
    for (int i=0; i<ptParamOutput->lFieldCnt; ++i) {
        cJSON *field = kr_param_output_field_serialize(&ptParamOutput->ptFieldDef[i]);
        cJSON_AddItemToArray(fields, field);
    }
    cJSON_AddItemToObject(output, "fields", fields);
    
    char *str = cJSON_Print(output);
    cJSON_Delete(output);
    return str;
}

void kr_param_output_serialize_free(char *ptParamOutputString)
{
    if (ptParamOutputString) {
        kr_free(ptParamOutputString);
    }
}


static void kr_param_output_field_deserialize(cJSON *field, T_KRParamOutputField *ptParamOutputField)
{
    ptParamOutputField->lFieldId = cJSON_GetNumber(field, "id");
    strcpy(ptParamOutputField->caFieldName, cJSON_GetString(field, "name"));
    strcpy(ptParamOutputField->caFieldDesc, cJSON_GetString(field, "desc"));
    strcpy(ptParamOutputField->caFieldType, cJSON_GetString(field, "type"));
    ptParamOutputField->lFieldLength = cJSON_GetNumber(field, "length");
}


T_KRParamOutput *kr_param_output_deserialize(char *ptParamOutputString)
{
    T_KRParamOutput *ptParamOutput = kr_calloc(sizeof(*ptParamOutput));
    
    cJSON *output=cJSON_Parse(ptParamOutputString);
    ptParamOutput->lOutputId = cJSON_GetNumber(output, "id");
    strcpy(ptParamOutput->caOutputName, cJSON_GetString(output, "name"));
    strcpy(ptParamOutput->caOutputDesc, cJSON_GetString(output, "desc"));
    cJSON *fields = cJSON_GetObjectItem(output, "fields");
    ptParamOutput->lFieldCnt = cJSON_GetArraySize(fields);
    ptParamOutput->ptFieldDef = kr_calloc(sizeof(T_KRParamOutputField)*ptParamOutput->lFieldCnt);
    for (int i=0; i<ptParamOutput->lFieldCnt; ++i) {
        cJSON *field = cJSON_GetArrayItem(fields, i);
        kr_param_output_field_deserialize(field, &ptParamOutput->ptFieldDef[i]);
    }
    
    return ptParamOutput;
}


void kr_param_output_deserialize_free(T_KRParamOutput *ptParamOutput)
{
    if (ptParamOutput) {
        if (ptParamOutput->ptFieldDef) {
            kr_free(ptParamOutput->ptFieldDef);
        }
        kr_free(ptParamOutput);
    }
}

