#include "kr_param_class_sdi.h"
#include "krutils/kr_utils.h"


char *kr_param_sdi_serialize(T_KRParamSdi *ptParamSdi)
{
    cJSON *sdi = cJSON_CreateObject();
    cJSON_AddNumberToObject(sdi, "id", ptParamSdi->lSdiId);
    cJSON_AddStringToObject(sdi, "name", ptParamSdi->caSdiName);
    cJSON_AddStringToObject(sdi, "desc", ptParamSdi->caSdiDesc);
    cJSON_AddStringToObject(sdi, "type", ptParamSdi->caSdiType);
    cJSON_AddStringToObject(sdi, "value_type", ptParamSdi->caSdiValueType);
    cJSON_AddStringToObject(sdi, "aggr_func", ptParamSdi->caSdiAggrFunc);
    cJSON_AddNumberToObject(sdi, "datasrc", ptParamSdi->lStatisticsDatasrc);
    cJSON_AddNumberToObject(sdi, "index", ptParamSdi->lStatisticsIndex);
    cJSON_AddNumberToObject(sdi, "field", ptParamSdi->lStatisticsField);
    cJSON_AddNumberToObject(sdi, "location", ptParamSdi->lStatisticsLocation);
    cJSON_AddStringToObject(sdi, "location_property", ptParamSdi->caLocationProperty);
    cJSON_AddStringToObject(sdi, "filter_format", ptParamSdi->caSdiFilterFormat);
    cJSON_AddStringToObject(sdi, "filter_string", ptParamSdi->caSdiFilterString);
    
    char *str = cJSON_Print(sdi);
    cJSON_Delete(sdi);
    return str;
}

void kr_param_sdi_serialize_free(char *ptParamSdiString)
{
    if (ptParamSdiString) {
        kr_free(ptParamSdiString);
    }
}



T_KRParamSdi *kr_param_sdi_deserialize(char *ptParamSdiString)
{
    T_KRParamSdi *ptParamSdi = kr_calloc(sizeof(*ptParamSdi));
    
    cJSON *sdi=cJSON_Parse(ptParamSdiString);
    ptParamSdi->lSdiId = cJSON_GetNumber(sdi, "id");
    strcpy(ptParamSdi->caSdiName, cJSON_GetString(sdi, "name"));
    strcpy(ptParamSdi->caSdiDesc, cJSON_GetString(sdi, "desc"));
    strcpy(ptParamSdi->caSdiType, cJSON_GetString(sdi, "type"));
    strcpy(ptParamSdi->caSdiValueType, cJSON_GetString(sdi, "value_type"));
    strcpy(ptParamSdi->caSdiAggrFunc, cJSON_GetString(sdi, "aggr_func"));
    ptParamSdi->lStatisticsDatasrc = cJSON_GetNumber(sdi, "datasrc");
    ptParamSdi->lStatisticsIndex = cJSON_GetNumber(sdi, "index");
    ptParamSdi->lStatisticsField = cJSON_GetNumber(sdi, "field");
    ptParamSdi->lStatisticsLocation = cJSON_GetNumber(sdi, "location");
    strcpy(ptParamSdi->caLocationProperty, cJSON_GetString(sdi, "location_property"));
    strcpy(ptParamSdi->caSdiFilterFormat, cJSON_GetString(sdi, "filter_format"));
    strcpy(ptParamSdi->caSdiFilterString, cJSON_GetString(sdi, "filter_string"));
    
    return ptParamSdi;
}


void kr_param_sdi_deserialize_free(T_KRParamSdi *ptParamSdi)
{
    if (ptParamSdi) {
        kr_free(ptParamSdi);
    }
}

