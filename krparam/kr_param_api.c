#include "kr_param_api.h"

cJSON *kr_param_set_info(T_KRParamSetDef *ptParamSetDef)
{
    cJSON *set = cJSON_CreateObject();
    cJSON_AddNumberToObject(set, "id", ptParamSetDef->lSetId);
    cJSON_AddStringToObject(set, "name", ptParamSetDef->caSetName);
    cJSON_AddStringToObject(set, "desc", ptParamSetDef->caSetDesc);
    cJSON_AddStringToObject(set, "usage", ptParamSetDef->caSetUsage);
    cJSON_AddStringToObject(set, "type", ptParamSetDef->caSetType);
    cJSON_AddStringToObject(set, "ele_type", ptParamSetDef->caElementType);
    cJSON_AddNumberToObject(set, "ele_length", ptParamSetDef->lElementLength);
    return set;
}

cJSON *kr_param_sdi_info(T_KRParamSDIDef *ptParamSDIDef)
{
    cJSON *sdi = cJSON_CreateObject();
    cJSON_AddNumberToObject(sdi, "id", ptParamSDIDef->lSdiId);
    cJSON_AddStringToObject(sdi, "name", ptParamSDIDef->caSdiName);
    cJSON_AddStringToObject(sdi, "desc", ptParamSDIDef->caSdiDesc);
    cJSON_AddStringToObject(sdi, "type", ptParamSDIDef->caSdiType);
    cJSON_AddStringToObject(sdi, "value_type", ptParamSDIDef->caSdiValueType);
    cJSON_AddStringToObject(sdi, "aggr_func", ptParamSDIDef->caSdiAggrFunc);
    cJSON_AddNumberToObject(sdi, "datasrc", ptParamSDIDef->lStatisticsDatasrc);
    cJSON_AddNumberToObject(sdi, "index", ptParamSDIDef->lStatisticsIndex);
    cJSON_AddNumberToObject(sdi, "field", ptParamSDIDef->lStatisticsField);
    cJSON_AddNumberToObject(sdi, "location", ptParamSDIDef->lStatisticsLocation);
    cJSON_AddStringToObject(sdi, "location_property", ptParamSDIDef->caLocationProperty);
    cJSON_AddStringToObject(sdi, "filter_format", ptParamSDIDef->caSdiFilterFormat);
    cJSON_AddStringToObject(sdi, "filter_string", ptParamSDIDef->caSdiFilterString);
    return sdi;
}

cJSON *kr_param_ddi_info(T_KRParamDDIDef *ptParamDDIDef)
{
    cJSON *ddi = cJSON_CreateObject();
    cJSON_AddNumberToObject(ddi, "id", ptParamDDIDef->lDdiId);
    cJSON_AddStringToObject(ddi, "name", ptParamDDIDef->caDdiName);
    cJSON_AddStringToObject(ddi, "desc", ptParamDDIDef->caDdiDesc);
    cJSON_AddStringToObject(ddi, "type", ptParamDDIDef->caDdiType);
    cJSON_AddStringToObject(ddi, "value_type", ptParamDDIDef->caDdiValueType);
    cJSON_AddStringToObject(ddi, "aggr_func", ptParamDDIDef->caDdiAggrFunc);
    cJSON_AddNumberToObject(ddi, "datasrc", ptParamDDIDef->lStatisticsDatasrc);
    cJSON_AddNumberToObject(ddi, "index", ptParamDDIDef->lStatisticsIndex);
    cJSON_AddNumberToObject(ddi, "field", ptParamDDIDef->lStatisticsField);
    cJSON_AddStringToObject(ddi, "statictics_type", ptParamDDIDef->caStatisticsType);
    cJSON_AddNumberToObject(ddi, "statictics_value", ptParamDDIDef->lStatisticsValue);
    cJSON_AddNumberToObject(ddi, "statictics_count", ptParamDDIDef->lStatisticsCount);
    cJSON_AddStringToObject(ddi, "statictics_method", ptParamDDIDef->caStatisticsMethod);
    cJSON_AddStringToObject(ddi, "filter_format", ptParamDDIDef->caDdiFilterFormat);
    cJSON_AddStringToObject(ddi, "filter_string", ptParamDDIDef->caDdiFilterString);
    return ddi;
}


cJSON *kr_param_hdi_info(T_KRParamHDIDef *ptParamHDIDef)
{
    cJSON *hdi = cJSON_CreateObject();
    cJSON_AddNumberToObject(hdi, "id", ptParamHDIDef->lHdiId);
    cJSON_AddStringToObject(hdi, "name", ptParamHDIDef->caHdiName);
    cJSON_AddStringToObject(hdi, "desc", ptParamHDIDef->caHdiDesc);
    cJSON_AddStringToObject(hdi, "type", ptParamHDIDef->caHdiType);
    cJSON_AddStringToObject(hdi, "value_type", ptParamHDIDef->caHdiValueType);
    cJSON_AddStringToObject(hdi, "aggr_func", ptParamHDIDef->caHdiAggrFunc);
    cJSON_AddNumberToObject(hdi, "datasrc", ptParamHDIDef->lStatisticsDatasrc);
    cJSON_AddNumberToObject(hdi, "index", ptParamHDIDef->lStatisticsIndex);
    cJSON_AddStringToObject(hdi, "statictics_type", ptParamHDIDef->caStatisticsType);
    cJSON_AddNumberToObject(hdi, "statictics_value", ptParamHDIDef->lStatisticsValue);
    cJSON_AddStringToObject(hdi, "statictics_method", ptParamHDIDef->caStatisticsMethod);
    return hdi;
}

cJSON *kr_param_rule_info(T_KRParamRuleDef *ptParamRuleDef)
{
    cJSON *rule = cJSON_CreateObject();
    cJSON_AddNumberToObject(rule, "id", ptParamRuleDef->lRuleId);
    cJSON_AddStringToObject(rule, "name", ptParamRuleDef->caRuleName);
    cJSON_AddStringToObject(rule, "desc", ptParamRuleDef->caRuleDesc);
    cJSON_AddNumberToObject(rule, "datasrc", ptParamRuleDef->lRuleDatasrc);
    cJSON_AddStringToObject(rule, "type", ptParamRuleDef->caRuleType);
    cJSON_AddStringToObject(rule, "func", ptParamRuleDef->caRuleFunc);
    cJSON_AddNumberToObject(rule, "weight", ptParamRuleDef->lRuleWeight);
    cJSON_AddStringToObject(rule, "calc_format", ptParamRuleDef->caRuleCalcFormat);
    cJSON_AddStringToObject(rule, "calc_string", ptParamRuleDef->caRuleCalcString);
    return rule;
}


cJSON *kr_param_group_info(T_KRParamGroupDef *ptParamGroupDef)
{
    cJSON *group = cJSON_CreateObject();
    cJSON_AddNumberToObject(group, "id", ptParamGroupDef->lGroupId);
    cJSON_AddStringToObject(group, "name", ptParamGroupDef->caGroupName);
    cJSON_AddStringToObject(group, "desc", ptParamGroupDef->caGroupDesc);
    cJSON_AddStringToObject(group, "func", ptParamGroupDef->caGroupFunc);
    cJSON_AddStringToObject(group, "calc_format", ptParamGroupDef->caGroupCalcFormat);
    cJSON_AddStringToObject(group, "calc_string", ptParamGroupDef->caGroupCalcString);
    return group;
}

cJSON *kr_param_info(T_KRParam *ptParam)
{
    cJSON *param = cJSON_CreateObject();

    short nCurrSecId = ptParam->nSecId;
    cJSON_AddNumberToObject(param, "current_secid", nCurrSecId);

    cJSON_AddNumberToObject(param, "set_load_time", 
            (long )ptParam->stParamSet[nCurrSecId].tLastLoadTime);
    cJSON_AddNumberToObject(param, "sdi_load_time", 
            (long )ptParam->stParamSDI[nCurrSecId].tLastLoadTime);
    cJSON_AddNumberToObject(param, "ddi_load_time", 
            (long )ptParam->stParamDDI[nCurrSecId].tLastLoadTime);
    cJSON_AddNumberToObject(param, "hdi_load_time", 
            (long )ptParam->stParamHDI[nCurrSecId].tLastLoadTime);
    cJSON_AddNumberToObject(param, "group_load_time", 
            (long )ptParam->stParamGroup[nCurrSecId].tLastLoadTime);

    return param;
}
