#include "kr_data_api.h"

cJSON *kr_set_info(T_KRSet *ptSet) 
{ 
    cJSON *set = cJSON_CreateObject(); 
    cJSON *def = kr_param_set_info(ptSet->ptParamSetDef); 
    cJSON_AddItemToObject(set, "def", def); 
    return set; 
}

cJSON *kr_sdi_info(T_KRSDI *ptSDI) 
{ 
    cJSON *sdi = cJSON_CreateObject(); 
    cJSON *def = kr_param_sdi_info(ptSDI->ptParamSDIDef); 
    cJSON_AddItemToObject(sdi, "def", def); 
    return sdi; 
}

cJSON *kr_ddi_info(T_KRDDI *ptDDI)
{
    cJSON *ddi = cJSON_CreateObject();
    cJSON *def = kr_param_ddi_info(ptDDI->ptParamDDIDef);
    cJSON_AddItemToObject(ddi, "def", def);
    return ddi;
}

cJSON *kr_hdi_info(T_KRHDI *ptHDI) 
{ 
    cJSON *hdi = cJSON_CreateObject(); 
    cJSON *def = kr_param_hdi_info(ptHDI->ptParamHDIDef); 
    cJSON_AddItemToObject(hdi, "def", def); 
    return hdi; 
}

cJSON *kr_data_info(T_KRData *ptData)
{ 
    cJSON *data = cJSON_CreateObject();

    cJSON_AddNumberToObject(data, "set_construct_time",
            (long )ptData->ptSetTable->tConstructTime);
    cJSON_AddNumberToObject(data, "sdi_construct_time",
            (long )ptData->ptSdiTable->tConstructTime);
    cJSON_AddNumberToObject(data, "ddi_construct_time",
            (long )ptData->ptDdiTable->tConstructTime);
    cJSON_AddNumberToObject(data, "hdi_construct_time",
            (long )ptData->ptHdiTable->tConstructTime);

    return data;
}
