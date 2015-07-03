#include "kr_param_class_ddi.h"
#include "krutils/kr_utils.h"


char *kr_param_ddi_serialize(T_KRParamDdi *ptParamDdi)
{
    cJSON *ddi = cJSON_CreateObject();
    cJSON_AddNumberToObject(ddi, "id", ptParamDdi->lDdiId);
    cJSON_AddStringToObject(ddi, "name", ptParamDdi->caDdiName);
    cJSON_AddStringToObject(ddi, "desc", ptParamDdi->caDdiDesc);
    cJSON_AddStringToObject(ddi, "type", ptParamDdi->caDdiType);
    cJSON_AddStringToObject(ddi, "value_type", ptParamDdi->caDdiValueType);
    cJSON_AddStringToObject(ddi, "aggr_func", ptParamDdi->caDdiAggrFunc);
    cJSON_AddNumberToObject(ddi, "datasrc", ptParamDdi->lStatisticsDatasrc);
    cJSON_AddNumberToObject(ddi, "index", ptParamDdi->lStatisticsIndex);
    cJSON_AddNumberToObject(ddi, "field", ptParamDdi->lStatisticsField);
    cJSON_AddStringToObject(ddi, "statictics_type", ptParamDdi->caStatisticsType);
    cJSON_AddNumberToObject(ddi, "statictics_value", ptParamDdi->lStatisticsValue);
    cJSON_AddNumberToObject(ddi, "statictics_count", ptParamDdi->lStatisticsCount);
    cJSON_AddStringToObject(ddi, "statictics_method", ptParamDdi->caStatisticsMethod);
    cJSON_AddStringToObject(ddi, "filter_format", ptParamDdi->caDdiFilterFormat);
    cJSON_AddStringToObject(ddi, "filter_string", ptParamDdi->caDdiFilterString);
    
    char *str = cJSON_Print(ddi);
    cJSON_Delete(ddi);
    return str;
}

void kr_param_ddi_serialize_free(char *ptParamDdiString)
{
    if (ptParamDdiString) {
        kr_free(ptParamDdiString);
    }
}



T_KRParamDdi *kr_param_ddi_deserialize(char *ptParamDdiString)
{
    T_KRParamDdi *ptParamDdi = kr_calloc(sizeof(*ptParamDdi));
    
    cJSON *ddi=cJSON_Parse(ptParamDdiString);
    ptParamDdi->lDdiId = cJSON_GetNumber(ddi, "id");
    strcpy(ptParamDdi->caDdiName, cJSON_GetString(ddi, "name"));
    strcpy(ptParamDdi->caDdiDesc, cJSON_GetString(ddi, "desc"));
    strcpy(ptParamDdi->caDdiType, cJSON_GetString(ddi, "type"));
    strcpy(ptParamDdi->caDdiValueType, cJSON_GetString(ddi, "value_type"));
    strcpy(ptParamDdi->caDdiAggrFunc, cJSON_GetString(ddi, "aggr_func"));
    ptParamDdi->lStatisticsDatasrc = cJSON_GetNumber(ddi, "datasrc");
    ptParamDdi->lStatisticsIndex = cJSON_GetNumber(ddi, "index");
    strcpy(ptParamDdi->caStatisticsType, cJSON_GetString(ddi, "statictics_type"));
    ptParamDdi->lStatisticsValue = cJSON_GetNumber(ddi, "statictics_value");
    ptParamDdi->lStatisticsCount = cJSON_GetNumber(ddi, "statictics_count");
    strcpy(ptParamDdi->caStatisticsMethod, cJSON_GetString(ddi, "statictics_method"));
    strcpy(ptParamDdi->caDdiFilterFormat, cJSON_GetString(ddi, "filter_format"));
    strcpy(ptParamDdi->caDdiFilterString, cJSON_GetString(ddi, "filter_string"));
    
    return ptParamDdi;
}


void kr_param_ddi_deserialize_free(T_KRParamDdi *ptParamDdi)
{
    if (ptParamDdi) {
        kr_free(ptParamDdi);
    }
}

