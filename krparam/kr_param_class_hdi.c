#include "kr_param_class_hdi.h"
#include "krutils/kr_utils.h"


char *kr_param_hdi_serialize(T_KRParamHdi *ptParamHdi)
{
    cJSON *hdi = cJSON_CreateObject();
    cJSON_AddNumberToObject(hdi, "id", ptParamHdi->lHdiId);
    cJSON_AddStringToObject(hdi, "name", ptParamHdi->caHdiName);
    cJSON_AddStringToObject(hdi, "desc", ptParamHdi->caHdiDesc);
    cJSON_AddStringToObject(hdi, "type", ptParamHdi->caHdiType);
    cJSON_AddStringToObject(hdi, "value_type", ptParamHdi->caHdiValueType);
    cJSON_AddStringToObject(hdi, "aggr_func", ptParamHdi->caHdiAggrFunc);
    cJSON_AddNumberToObject(hdi, "datasrc", ptParamHdi->lStatisticsDatasrc);
    cJSON_AddNumberToObject(hdi, "index", ptParamHdi->lStatisticsIndex);
    cJSON_AddStringToObject(hdi, "statictics_type", ptParamHdi->caStatisticsType);
    cJSON_AddNumberToObject(hdi, "statictics_value", ptParamHdi->lStatisticsValue);
    cJSON_AddStringToObject(hdi, "statictics_method", ptParamHdi->caStatisticsMethod);
    
    char *str = cJSON_Print(hdi);
    cJSON_Delete(hdi);
    return str;
}

void kr_param_hdi_serialize_free(char *ptParamHdiString)
{
    if (ptParamHdiString) {
        kr_free(ptParamHdiString);
    }
}



T_KRParamHdi *kr_param_hdi_deserialize(char *ptParamHdiString)
{
    T_KRParamHdi *ptParamHdi = kr_calloc(sizeof(*ptParamHdi));
    
    cJSON *hdi=cJSON_Parse(ptParamHdiString);
    ptParamHdi->lHdiId = cJSON_GetNumber(hdi, "id");
    strcpy(ptParamHdi->caHdiName, cJSON_GetString(hdi, "name"));
    strcpy(ptParamHdi->caHdiDesc, cJSON_GetString(hdi, "desc"));
    strcpy(ptParamHdi->caHdiType, cJSON_GetString(hdi, "type"));
    strcpy(ptParamHdi->caHdiValueType, cJSON_GetString(hdi, "value_type"));
    strcpy(ptParamHdi->caHdiAggrFunc, cJSON_GetString(hdi, "aggr_func"));
    ptParamHdi->lStatisticsDatasrc = cJSON_GetNumber(hdi, "datasrc");
    ptParamHdi->lStatisticsIndex = cJSON_GetNumber(hdi, "index");
    strcpy(ptParamHdi->caStatisticsType, cJSON_GetString(hdi, "statictics_type"));
    ptParamHdi->lStatisticsValue = cJSON_GetNumber(hdi, "statictics_value");
    strcpy(ptParamHdi->caStatisticsMethod, cJSON_GetString(hdi, "statictics_method"));
    
    return ptParamHdi;
}


void kr_param_hdi_deserialize_free(T_KRParamHdi *ptParamHdi)
{
    if (ptParamHdi) {
        kr_free(ptParamHdi);
    }
}

