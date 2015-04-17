#include "kr_engine_context.h"

/* initialize rule detecting context */
T_KRContext *kr_context_init(T_KRContextEnv *ptEnv)
{
    T_KRContext *ptContext = kr_calloc(sizeof(T_KRContext));
    if (ptContext == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptContext failed!");
        return NULL;
    }

    ptContext->ptEnv = ptEnv;

    /* reconnect database in thread */
    T_DbsEnv *ptDbsEnv = ((T_KRContextEnv *)ptEnv)->ptDbsEnv;
    ptContext->ptDbsEnv = dbsConnect(ptDbsEnv->dsn, ptDbsEnv->user, ptDbsEnv->pass);
    if (ptContext->ptDbsEnv == NULL) {
        KR_LOG(KR_LOGERROR, "dbsConnect [%s] [%s] [%s] failed!", \
                ptDbsEnv->dsn, ptDbsEnv->user, ptDbsEnv->pass);
        kr_free(ptContext);
        return NULL;
    }

    /* construct data */
    ptContext->ptData = kr_data_construct(ptEnv->ptParam, ptEnv->dataModule, 
            ptDbsEnv, ptEnv->ptHDICache, kr_data_get_type, kr_data_get_value);
    if (ptContext->ptData == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_construct failed!");
        dbsDisconnect(ptContext->ptDbsEnv);
        kr_free(ptContext);
        return NULL;
    }
    
    /*TODO: construct flow */
    
    return ptContext;
}


int kr_context_set(T_KRContext *ptContext, void *ptArg)
{
    T_KRContextEnv *ptEnv = ptContext->ptEnv;

    /* check dynamic memory, reload if needed */
    if (kr_data_check(ptContext->ptData) != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_check failed");
        return -1;
    }

    /* set argument */
    ptContext->ptArg = ptArg;

    return 0;
}

void kr_context_clean(T_KRContext *ptContext)
{
    /*initialize dynamic data memory*/
    kr_data_init(ptContext->ptData);

    /*initialize dynamic flow memory*/
    //kr_flow_init(ptContext->ptFlow);

    /*initialize others*/
    ptContext->ptArg = NULL;
    ptContext->ptCurrRec = NULL;
}

void kr_context_fini(T_KRContext *ptContext)
{
    if (ptContext) {
        //kr_flow_destruct(ptContext->ptFlow);
        kr_data_destruct(ptContext->ptData);
        dbsDisconnect(ptContext->ptDbsEnv);
        kr_free(ptContext);
    }
}

static void _set_cjson_field(E_KRType type, char *name, U_KRValue *value, cJSON *json) 
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

static void _set_cjson_record(T_KRRecord *krrecord, cJSON *json)
{
    T_KRTable *ptTable = (T_KRTable *) krrecord->ptTable;
    for (int i=0; i<ptTable->iFieldCnt; i++) {
        E_KRType type = kr_field_get_type(krrecord, i);
        char *name = kr_field_get_name(krrecord, i);
        void *value = kr_field_get_value(krrecord, i);
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

static void _set_cjson_related(void *key, T_KRRecord *krrecord, cJSON *array)
{
    cJSON *record = cJSON_CreateObject();
    _set_cjson_record(krrecord, record);
    cJSON_AddItemToArray(array, record);
}

static void _add_sdi(void *key, T_KRSDI *krsdi, cJSON *sdis)
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

static void _add_ddi(void *key, T_KRDDI *krddi, cJSON *ddis)
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

static void _add_hdi(void *key, T_KRHDI *krhdi, cJSON *hdis)
{
    cJSON *hdi = cJSON_CreateObject();
    cJSON_AddNumberToObject(hdi, "id", krhdi->lHDIId);
    cJSON_AddNumberToObject(hdi, "valueind", krhdi->eValueInd);
    _set_cjson_field(krhdi->eValueType, "value", &krhdi->uValue, hdi);
    cJSON_AddItemToArray(hdis, hdi);
}

static void _add_rule(T_KRRule *krrule, cJSON *rules)
{
    cJSON *rule = cJSON_CreateObject();
    cJSON_AddNumberToObject(rule, "id", krrule->lRuleId);
    cJSON_AddStringToObject(rule, "name", krrule->ptParamRuleDef->caRuleName);
    U_KRValue *value = kr_calc_value(krrule->ptRuleCalc);
    cJSON_AddNumberToObject(rule, "result", value->b);
    cJSON_AddItemToArray(rules, rule);
}

static void _add_sdis_to_data(T_KRHashTable *ptSdiTable, cJSON *datas)
{
    cJSON *sdis = cJSON_CreateArray();
    kr_hashtable_foreach(ptSdiTable, (KRHFunc )_add_sdi, sdis);
    cJSON_AddItemToObject(datas, "sdis", sdis);
}

static void _add_ddis_to_data(T_KRHashTable *ptDdiTable, cJSON *datas)
{
    cJSON *ddis = cJSON_CreateArray();
    kr_hashtable_foreach(ptDdiTable, (KRHFunc )_add_ddi, ddis);
    cJSON_AddItemToObject(datas, "ddis", ddis);
}

static void _add_hdis_to_data(T_KRHashTable *ptHdiTable, cJSON *datas)
{
    cJSON *hdis = cJSON_CreateArray();
    kr_hashtable_foreach(ptHdiTable, (KRHFunc )_add_hdi, hdis);
    cJSON_AddItemToObject(datas, "hdis", hdis);
}

cJSON *kr_context_dump_json(T_KRContext *ptContext)
{
    cJSON *alert = cJSON_CreateObject();

    /*add group info*/
    T_KRGroup *krgroup = ptContext->ptFlow->ptRoutedGroup;
    cJSON_AddNumberToObject(alert, "groupid", krgroup->lGroupId);

    /*add current record info*/
    cJSON *currec = cJSON_CreateObject();
    _set_cjson_record(ptContext->ptCurrRec, currec);
    cJSON_AddItemToObject(alert, "currec", currec);

    /*add rules info*/
    cJSON *rules = cJSON_CreateArray();
    kr_list_foreach(krgroup->ptRuleList->ptRuleList, 
            (KRForEachFunc )_add_rule, rules);
    cJSON_AddItemToObject(alert, "rules", rules);

    /*add dataitems info*/
    cJSON *datas = cJSON_CreateObject();
    T_KRHashTable *ptSdiTable = ptContext->ptData->ptSdiTable->ptSDITable;
    _add_sdis_to_data(ptSdiTable, datas);
    T_KRHashTable *ptDdiTable = ptContext->ptData->ptDdiTable->ptDDITable;
    _add_ddis_to_data(ptDdiTable, datas);
    T_KRHashTable *ptHdiTable = ptContext->ptData->ptHdiTable->ptHDITable;
    _add_hdis_to_data(ptHdiTable, datas);
    cJSON_AddItemToObject(alert, "datas", datas);

    return alert;
}
