#include "kr_param_class_input.h"
#include "krutils/kr_utils.h"

static cJSON *kr_param_input_field_serialize(T_KRParamInputField *ptParamInputField)
{
    cJSON *field = cJSON_CreateObject();
    cJSON_AddNumberToObject(field, "id", ptParamInputField->lFieldId);
    cJSON_AddStringToObject(field, "name", ptParamInputField->caFieldName);
    cJSON_AddStringToObject(field, "desc", ptParamInputField->caFieldDesc);
    cJSON_AddStringToObject(field, "type", ptParamInputField->caFieldType);
    cJSON_AddNumberToObject(field, "length", ptParamInputField->lFieldLength);
    return field;
}

char *kr_param_input_serialize(T_KRParamInput *ptParamInput)
{
    cJSON *input = cJSON_CreateObject();
    cJSON_AddNumberToObject(input, "id", ptParamInput->lInputId);
    cJSON_AddStringToObject(input, "name", ptParamInput->caInputName);
    cJSON_AddStringToObject(input, "desc", ptParamInput->caInputDesc);
    cJSON_AddStringToObject(input, "keep_mode", ptParamInput->caKeepMode);
    cJSON_AddNumberToObject(input, "keep_value", ptParamInput->lKeepValue);
    cJSON *fields = cJSON_CreateArray();
    for (int i=0; i<ptParamInput->lFieldCnt; ++i) {
        cJSON *field = kr_param_input_field_serialize(&ptParamInput->ptFieldDef[i]);
        cJSON_AddItemToArray(fields, field);
    }
    cJSON_AddItemToObject(input, "fields", fields);
    
    char *str = cJSON_Print(input);
    cJSON_Delete(input);
    return str;
}

void kr_param_input_serialize_free(char *psParamInputString)
{
    if (psParamInputString) {
        kr_free(psParamInputString);
    }
}


static void kr_param_input_field_deserialize(cJSON *field, T_KRParamInputField *ptParamInputField)
{
    ptParamInputField->lFieldId = cJSON_GetNumber(field, "id");
    strcpy(ptParamInputField->caFieldName, cJSON_GetString(field, "name"));
    strcpy(ptParamInputField->caFieldDesc, cJSON_GetString(field, "desc"));
    strcpy(ptParamInputField->caFieldType, cJSON_GetString(field, "type"));
    ptParamInputField->lFieldLength = cJSON_GetNumber(field, "length");
}


T_KRParamInput *kr_param_input_deserialize(char *psParamInputString)
{
    T_KRParamInput *ptParamInput = kr_calloc(sizeof(*ptParamInput));
    
    cJSON *input=cJSON_Parse(psParamInputString);
    ptParamInput->lInputId = cJSON_GetNumber(input, "id");
    strcpy(ptParamInput->caInputName, cJSON_GetString(input, "name"));
    strcpy(ptParamInput->caInputDesc, cJSON_GetString(input, "desc"));
    strcpy(ptParamInput->caKeepMode, cJSON_GetString(input, "keep_mode"));
    ptParamInput->lKeepValue = cJSON_GetNumber(input, "keep_value");
    cJSON *fields = cJSON_GetObjectItem(input, "fields");
    ptParamInput->lFieldCnt = cJSON_GetArraySize(fields);
    ptParamInput->ptFieldDef = kr_calloc(sizeof(T_KRParamInputField)*ptParamInput->lFieldCnt);
    for (int i=0; i<ptParamInput->lFieldCnt; ++i) {
        cJSON *field = cJSON_GetArrayItem(fields, i);
        kr_param_input_field_deserialize(field, &ptParamInput->ptFieldDef[i]);
    }
    
    return ptParamInput;
}


void kr_param_input_deserialize_free(T_KRParamInput *ptParamInput)
{
    if (ptParamInput) {
        if (ptParamInput->ptFieldDef) {
            kr_free(ptParamInput->ptFieldDef);
        }
        kr_free(ptParamInput);
    }
}

