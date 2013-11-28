#include "krdata/kr_data.h"
#include "krutils/kr_json.h"

void _set_cjson_field(E_KRType type, char *name, U_KRValue *value, cJSON *json) 
{ 
    switch(type) 
    { 
        case KR_TYPE_INT: 
            cJSON_AddNumberToObject(json, name, value->i);
            break;
        case KR_TYPE_LONG: 
            cJSON_AddNumberToObject(json, name, value->l);
            break;
        case KR_TYPE_DOUBLE: 
            cJSON_AddNumberToObject(json, name, value->d);
            break;
        case KR_TYPE_POINTER:
            cJSON_AddNumberToObject(json, name, (long )(value->p));
            break;
        case KR_TYPE_STRING: 
            cJSON_AddStringToObject(json, name, value->s?value->s:" ");
            break;
        default: 
            return; 
    } 
} 

void _set_cjson_record(T_KRRecord *krrecord, cJSON *json)
{
    T_KRTable *ptTable = (T_KRTable *) krrecord->ptTable;
    int i = 0;
    for (i = 0; i<ptTable->iFieldCnt; i++) {
        E_KRType type = kr_get_field_type(krrecord, i);
        char *name = kr_get_field_name(krrecord, i);
        void *value = kr_get_field_value(krrecord, i);
        U_KRValue uVal; 
        switch(type) 
        { 
            case KR_TYPE_INT: 
                uVal.i = *(int *)value;
                break;
            case KR_TYPE_LONG: 
                uVal.l = *(long *)value;
                break;
            case KR_TYPE_DOUBLE: 
                uVal.d = *(double *)value;
                break;
            case KR_TYPE_POINTER:
                uVal.p = value;
                break;
            case KR_TYPE_STRING: 
                uVal.s = value;
                break;
        } 
        _set_cjson_field(type, name, &uVal, json);
    }
}


void _set_cjson_related(void *key, T_KRRecord *krrecord, cJSON *array)
{
    cJSON *record = cJSON_CreateObject();
    _set_cjson_record(krrecord, record);
    cJSON_AddItemToArray(array, record);
}


void _add_sdi(void *key, T_KRSDI *krsdi, cJSON *sdis)
{
    cJSON *sdi = cJSON_CreateObject();
    cJSON_AddNumberToObject(sdi, "id", krsdi->lSDIId);
    cJSON_AddNumberToObject(sdi, "valueind", krsdi->eValueInd);
    _set_cjson_field(krsdi->eValueType, "value", &krsdi->uValue, sdi);
    /*add related*/
    cJSON *related = cJSON_CreateArray();
    kr_hashtable_foreach(krsdi->ptRelated, (KRHFunc )_set_cjson_related, related);
    cJSON_AddItemToObject(sdi, "related", related);
    cJSON_AddItemToArray(sdis, sdi);
}

void _add_ddi(void *key, T_KRDDI *krddi, cJSON *ddis)
{
    cJSON *ddi = cJSON_CreateObject();
    cJSON_AddNumberToObject(ddi, "id", krddi->lDDIId);
    cJSON_AddNumberToObject(ddi, "valueind", krddi->eValueInd);
    _set_cjson_field(krddi->eValueType, "value", &krddi->uValue, ddi);
    /*add related*/
    cJSON *related = cJSON_CreateArray();
    kr_hashtable_foreach(krddi->ptRelated, (KRHFunc )_set_cjson_related, related);
    cJSON_AddItemToObject(ddi, "related", related);
    cJSON_AddItemToArray(ddis, ddi);
}

void _add_hdi(void *key, T_KRHDI *krhdi, cJSON *hdis)
{
    cJSON *hdi = cJSON_CreateObject();
    cJSON_AddNumberToObject(hdi, "id", krhdi->lHDIId);
    cJSON_AddNumberToObject(hdi, "valueind", krhdi->eValueInd);
    _set_cjson_field(krhdi->eValueType, "value", &krhdi->uValue, hdi);
    cJSON_AddItemToArray(hdis, hdi);
}

void _add_rule(T_KRRule *krrule, cJSON *rules)
{
    cJSON *rule = cJSON_CreateObject();
    cJSON_AddNumberToObject(rule, "id", krrule->lRuleId);
    cJSON_AddStringToObject(rule, "name", krrule->ptShmRuleDef->caRuleName);
    cJSON_AddNumberToObject(rule, "result", krrule->ptRuleCalc->result_value.b);
    cJSON_AddItemToArray(rules, rule);
}

void _add_sdis_to_data(T_KRHashTable *sditable, cJSON *datas)
{
    cJSON *sdis = cJSON_CreateArray();
    kr_hashtable_foreach(sditable, (KRHFunc )_add_sdi, sdis);
    cJSON_AddItemToObject(datas, "sdis", sdis);
}

void _add_ddis_to_data(T_KRHashTable *dditable, cJSON *datas)
{
    cJSON *ddis = cJSON_CreateArray();
    kr_hashtable_foreach(dditable, (KRHFunc )_add_ddi, ddis);
    cJSON_AddItemToObject(datas, "ddis", ddis);
}

void _add_hdis_to_data(T_KRHashTable *hditable, cJSON *datas)
{
    cJSON *hdis = cJSON_CreateArray();
    kr_hashtable_foreach(hditable, (KRHFunc )_add_hdi, hdis);
    cJSON_AddItemToObject(datas, "hdis", hdis);
}


int kr_group_func(T_KRGroup *krgroup, T_KRContext *krcontext)
{
    /*add alert info*/
    cJSON *alert = cJSON_CreateObject();
    cJSON_AddNumberToObject(alert, "groupid", krgroup->lGroupId);

    /*add current record info*/
    cJSON *currec = cJSON_CreateObject();
    _set_cjson_record(krcontext->ptCurrRec, currec);
    cJSON_AddItemToObject(alert, "currec", currec);

    /*add rules info*/
    cJSON *rules = cJSON_CreateArray();
    kr_list_foreach(krgroup->ptRuleList->ptRuleList, (KRForEachFunc )_add_rule, rules);
    cJSON_AddItemToObject(alert, "rules", rules);

    /*add dataitems info*/
    cJSON *datas = cJSON_CreateObject();
    T_KRHashTable *sditable = krcontext->ptDym->sditable->ptSDITable;
    _add_sdis_to_data(sditable, datas);
    T_KRHashTable *dditable = krcontext->ptDym->dditable->ptDDITable;
    _add_ddis_to_data(dditable, datas);
    T_KRHashTable *hditable = krcontext->ptDym->hditable->ptHDITable;
    _add_hdis_to_data(hditable, datas);
    cJSON_AddItemToObject(alert, "datas", datas);

    krcontext->pcaResp = cJSON_PrintUnformatted(alert);
    cJSON_Delete(alert);

    /*Important: You need kr_free Response*/
    printf("response: %s\n", krcontext->pcaResp);
    KR_LOG(KR_LOGDEBUG, "response : %s ", krcontext->pcaResp);
    return 0;
}
