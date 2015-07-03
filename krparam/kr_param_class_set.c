#include "kr_param_class_set.h"
#include "krutils/kr_utils.h"

char *kr_param_set_serialize(T_KRParamSet *ptParamSet)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "id", ptParamSet->lSetId);
    cJSON_AddStringToObject(json, "name", ptParamSet->caSetName);
    cJSON_AddStringToObject(json, "desc", ptParamSet->caSetDesc);
    cJSON_AddStringToObject(json, "usage", ptParamSet->caSetUsage);
    cJSON_AddStringToObject(json, "type", ptParamSet->caSetType);
    cJSON_AddStringToObject(json, "ele_type", ptParamSet->caElementType);
    cJSON_AddNumberToObject(json, "ele_length", ptParamSet->lElementLength);
    cJSON *eles = cJSON_CreateArray();
    for (int i=0; i<ptParamSet->lEleCount; ++i) {
        cJSON *ele = NULL;
        switch(ptParamSet->caElementType[0])
        {
            case KR_TYPE_INT:
                ele = cJSON_CreateNumber(ptParamSet->puEleValue[i].i);
                break;         
            case KR_TYPE_LONG:
                ele = cJSON_CreateNumber(ptParamSet->puEleValue[i].l);
                break; 
            case KR_TYPE_DOUBLE:
                ele = cJSON_CreateNumber(ptParamSet->puEleValue[i].d);
                break; 
            case KR_TYPE_STRING:
                ele = cJSON_CreateString(ptParamSet->puEleValue[i].s);
                break; 
            default:
                break;
        }
        cJSON_AddItemToArray(eles, ele);
    }
    cJSON_AddItemToObject(json, "eles", eles);
    
    char *str = cJSON_Print(json);
    cJSON_Delete(json);
    return str;
}

void kr_param_set_serialize_free(char *ptParamSetString)
{
    if (ptParamSetString) {
        kr_free(ptParamSetString);
    }
}


T_KRParamSet *kr_param_set_deserialize(char *ptParamSetString)
{
    T_KRParamSet *ptParamSet = kr_calloc(sizeof(*ptParamSet));
    
    cJSON *json=cJSON_Parse(ptParamSetString);
    ptParamSet->lSetId = cJSON_GetNumber(json, "id");
    strcpy(ptParamSet->caSetName, cJSON_GetString(json, "name"));
    strcpy(ptParamSet->caSetDesc, cJSON_GetString(json, "desc"));
    strcpy(ptParamSet->caSetUsage, cJSON_GetString(json, "usage"));
    strcpy(ptParamSet->caSetType, cJSON_GetString(json, "type"));
    strcpy(ptParamSet->caElementType, cJSON_GetString(json, "ele_type"));
    ptParamSet->lElementLength = cJSON_GetNumber(json, "ele_length");
    cJSON *eles = cJSON_GetObjectItem(json, "eles");
    ptParamSet->lEleCount = cJSON_GetArraySize(eles);
    ptParamSet->puEleValue = kr_calloc(sizeof(U_KRValue)*ptParamSet->lEleCount);
    for (int i=0; i<ptParamSet->lEleCount; ++i) {
        cJSON *ele = cJSON_GetArrayItem(eles, i);
        switch(ptParamSet->caElementType[0])
        {
            case KR_TYPE_INT:
                ptParamSet->puEleValue[i].i = (int )ele->valuedouble;
                break;         
            case KR_TYPE_LONG:
                ptParamSet->puEleValue[i].l = (long )ele->valuedouble;
                break; 
            case KR_TYPE_DOUBLE:
                ptParamSet->puEleValue[i].d = (double )ele->valuedouble;
                break; 
            case KR_TYPE_STRING:
                ptParamSet->puEleValue[i].s = kr_strdup(ele->valuestring);
                break; 
            default:
                break;
        }
    }
    
    return ptParamSet;
}


void kr_param_set_deserialize_free(T_KRParamSet *ptParamSet)
{
    if (ptParamSet) {
        if (ptParamSet->puEleValue) {
            if (ptParamSet->caElementType[0] == KR_TYPE_STRING) {
                for (int i=0; i<ptParamSet->lEleCount; ++i) {
                    kr_free(ptParamSet->puEleValue[i].s);
                }
            }
            kr_free(ptParamSet->puEleValue);
        }
        kr_free(ptParamSet);
    }
}

