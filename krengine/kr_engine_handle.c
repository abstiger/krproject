#include "kr_engine.h"
#include "kr_engine_context.h"

#include "krparam/kr_param_api.h"
#include "krdb/kr_db_api.h"
#include "krdata/kr_data_api.h"
#include "krflow/kr_flow_api.h"

static void kr_engine_info(T_KRContext *krctx, T_KREngineArg *krarg);
static void kr_engine_info_log(T_KRContext *krctx, T_KREngineArg *krarg);
static void kr_engine_set_logpath(T_KRContext *krctx, T_KREngineArg *krarg);
static void kr_engine_set_logname(T_KRContext *krctx, T_KREngineArg *krarg);
static void kr_engine_set_loglevel(T_KRContext *krctx, T_KREngineArg *krarg);

static void kr_engine_info_krdb(T_KRContext *krctx, T_KREngineArg *krarg);
static void kr_engine_info_table(T_KRContext *krctx, T_KREngineArg *krarg);
static void kr_engine_info_index(T_KRContext *krctx, T_KREngineArg *krarg);
static void kr_engine_list_index_key(T_KRContext *krctx, T_KREngineArg *krarg);

static void kr_engine_reload_param(T_KRContext *krctx, T_KREngineArg *krarg);
static void kr_engine_info_param(T_KRContext *krctx, T_KREngineArg *krarg);
static void kr_engine_info_group(T_KRContext *krctx, T_KREngineArg *krarg);
static void kr_engine_info_rule(T_KRContext *krctx, T_KREngineArg *krarg);
static void kr_engine_info_set(T_KRContext *krctx, T_KREngineArg *krarg);
static void kr_engine_info_sdi(T_KRContext *krctx, T_KREngineArg *krarg);
static void kr_engine_info_ddi(T_KRContext *krctx, T_KREngineArg *krarg);
static void kr_engine_info_hdi(T_KRContext *krctx, T_KREngineArg *krarg);

static void kr_engine_insert_event(T_KRContext *krctx, T_KREngineArg *krarg);
static void kr_engine_detect_event(T_KRContext *krctx, T_KREngineArg *krarg);


int kr_engine_register_default(T_KREngine *krengine)
{
    kr_engine_register(krengine, "detect_event", (KRHandleFunc )kr_engine_detect_event);
    kr_engine_register(krengine, "insert_event", (KRHandleFunc )kr_engine_insert_event);

    kr_engine_register(krengine, "info_engine", (KRHandleFunc )kr_engine_info);
    kr_engine_register(krengine, "info_log", (KRHandleFunc )kr_engine_info_log);

    kr_engine_register(krengine, "set_logpath", (KRHandleFunc )kr_engine_set_logpath);
    kr_engine_register(krengine, "set_logname", (KRHandleFunc )kr_engine_set_logname);
    kr_engine_register(krengine, "set_loglevel", (KRHandleFunc )kr_engine_set_loglevel);

    kr_engine_register(krengine, "info_krdb", (KRHandleFunc )kr_engine_info_krdb);
    kr_engine_register(krengine, "info_table", (KRHandleFunc )kr_engine_info_table);
    kr_engine_register(krengine, "info_index", (KRHandleFunc )kr_engine_info_index);
    kr_engine_register(krengine, "list_index_key", (KRHandleFunc )kr_engine_list_index_key);
    kr_engine_register(krengine, "reload_param", (KRHandleFunc )kr_engine_reload_param);
    kr_engine_register(krengine, "info_param", (KRHandleFunc )kr_engine_info_param);
    kr_engine_register(krengine, "info_group", (KRHandleFunc )kr_engine_info_group);
    kr_engine_register(krengine, "info_rule", (KRHandleFunc )kr_engine_info_rule);
    kr_engine_register(krengine, "info_set", (KRHandleFunc )kr_engine_info_set);
    kr_engine_register(krengine, "info_sdi", (KRHandleFunc )kr_engine_info_sdi);
    kr_engine_register(krengine, "info_ddi", (KRHandleFunc )kr_engine_info_ddi);
    kr_engine_register(krengine, "info_hdi", (KRHandleFunc )kr_engine_info_hdi);
    return 0;
};


/*handles*/
static void kr_engine_info(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "version", VERSION);
    cJSON_AddStringToObject(json, "package", PACKAGE);
    cJSON_AddStringToObject(json, "package_version", PACKAGE_VERSION);
    cJSON_AddStringToObject(json, "package_bugreport", PACKAGE_BUGREPORT);
    cJSON_AddStringToObject(json, "package_url", PACKAGE_URL);
    
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}

static void kr_engine_info_log(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    cJSON *json = kr_log_dump_json();
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}

static void kr_engine_set_logpath(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    cJSON *apply_json = cJSON_Parse(apply->msgbuf);
    if (apply_json == NULL) {
        KR_LOG(KR_LOGERROR, "parse apply json failed!");
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }

    char *logpath = cJSON_GetString(apply_json, "log_path");
    if (logpath == NULL) {
        KR_LOG(KR_LOGERROR, "log_path not set!");
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }

    if (access(logpath, R_OK) != 0) {
        KR_LOG(KR_LOGERROR, "invalid logpath [%s]!", logpath);
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    } 

    kr_log_set_path(logpath);

    cJSON *json = kr_log_dump_json();
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}

static void kr_engine_set_logname(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    cJSON *apply_json = cJSON_Parse(apply->msgbuf);
    if (apply_json == NULL) {
        KR_LOG(KR_LOGERROR, "parse apply json failed!");
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }
    char *logname = cJSON_GetString(apply_json, "log_name");
    if (logname == NULL) {
        KR_LOG(KR_LOGERROR, "log_name not set!");
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }

    kr_log_set_name(logname);

    cJSON *json = kr_log_dump_json();
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}

static void kr_engine_set_loglevel(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    cJSON *apply_json = cJSON_Parse(apply->msgbuf);
    if (apply_json == NULL) {
        KR_LOG(KR_LOGERROR, "parse apply json failed!");
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }

    int loglevel = (int )cJSON_GetNumber(apply_json, "log_level");
    if (loglevel < KR_LOGFATAL || loglevel > KR_LOGDEBUG) {
        KR_LOG(KR_LOGERROR, "invalid loglevel [%d]!", loglevel);
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }

    kr_log_set_level(loglevel);

    cJSON *json = kr_log_dump_json();
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}


static void kr_engine_info_krdb(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    cJSON *json = kr_db_info(krctx->ptEnv->ptDB);
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}

static void kr_engine_info_table(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    T_KRTable *ptTable = kr_table_get(krctx->ptEnv->ptDB, apply->datasrc);
    if (ptTable == NULL) {
        KR_LOG(KR_LOGERROR, "invalid datasrc [%d]!", apply->datasrc);
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }

    cJSON *json = kr_db_table_info(ptTable);
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}

static void kr_engine_info_index(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    cJSON *apply_json = cJSON_Parse(apply->msgbuf);
    if (apply_json == NULL) {
        KR_LOG(KR_LOGERROR, "parse apply json failed!");
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }
    int index_id = (int )cJSON_GetNumber(apply_json, "index_id");

    T_KRTable *ptTable = kr_table_get(krctx->ptEnv->ptDB, apply->datasrc);
    if (ptTable == NULL) {
        KR_LOG(KR_LOGERROR, "invalid datasrc [%d]!", apply->datasrc);
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }
    T_KRIndex *ptIndex = kr_index_get(krctx->ptEnv->ptDB, index_id);
    if (ptIndex == NULL) {
        KR_LOG(KR_LOGERROR, "invalid index_id [%d]!", index_id);
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }
    
    cJSON *json = kr_db_index_info(ptIndex);
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}

static void kr_engine_list_index_key(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    //TODO:
    
    reply->msgbuf = kr_strdup("list index key!");
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
}


static void kr_engine_reload_param(T_KRContext *krctx, T_KREngineArg *krarg)
{
    KR_LOG(KR_LOGDEBUG, "invoke kr_engine_reload_param ...");
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    if (kr_param_load(krctx->ptEnv->ptParam) < 0) {
        KR_LOG(KR_LOGERROR, "kr_param_load failed!");
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }
    
    cJSON *json = cJSON_CreateObject();
    cJSON *shm = kr_param_info(krctx->ptEnv->ptParam);
    cJSON_AddItemToObject(json, "shm", shm);
    cJSON *dym = kr_data_info(krctx->ptData);
    cJSON_AddItemToObject(json, "dym", dym);
    
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}

static void kr_engine_info_param(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    cJSON *json = cJSON_CreateObject();
    cJSON *shm = kr_param_info(krctx->ptEnv->ptParam);
    cJSON_AddItemToObject(json, "shm", shm);
    cJSON *dym = kr_data_info(krctx->ptData);
    cJSON_AddItemToObject(json, "dym", dym);
    
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}

static void kr_engine_info_group(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    cJSON *apply_json = cJSON_Parse(apply->msgbuf);
    if (apply_json == NULL) {
        KR_LOG(KR_LOGERROR, "parse apply json failed!");
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }
    int id = (int )cJSON_GetNumber(apply_json, "group_id");
    T_KRGroup *krgroup = kr_group_lookup(krctx->ptFlow->ptGroupList, id);
    if (krgroup == NULL) {
        KR_LOG(KR_LOGERROR, "Group %d not found!", id);
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }
    cJSON *json = kr_group_info(krgroup);
    
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}

static void kr_engine_info_rule(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    cJSON *apply_json = cJSON_Parse(apply->msgbuf);
    if (apply_json == NULL) {
        KR_LOG(KR_LOGERROR, "parse apply json failed!");
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }
    int group_id = (int )cJSON_GetNumber(apply_json, "group_id");
    T_KRGroup *krgroup = kr_group_lookup(krctx->ptFlow->ptGroupList, group_id);
    if (krgroup == NULL) {
        KR_LOG(KR_LOGERROR, "Group %d not found!", group_id);
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }
    int rule_id = (int )cJSON_GetNumber(apply_json, "rule_id");
    T_KRRule *krrule = kr_rule_lookup(krgroup->ptRuleList, rule_id);
    if (krrule == NULL) {
        KR_LOG(KR_LOGERROR, "Rule %d not found!", rule_id);
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }

    cJSON *json = kr_rule_info(krrule);
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}

static void kr_engine_info_set(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    cJSON *apply_json = cJSON_Parse(apply->msgbuf);
    if (apply_json == NULL) {
        KR_LOG(KR_LOGERROR, "parse apply json failed!");
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }
    int id = (int )cJSON_GetNumber(apply_json, "set_id");
    T_KRSet *krset = kr_set_lookup(krctx->ptData->ptSetTable, id);
    if (krset == NULL) {
        KR_LOG(KR_LOGERROR, "Set %d not found!", id);
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }

    cJSON *json = kr_set_info(krset);
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}

static void kr_engine_info_sdi(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    cJSON *apply_json = cJSON_Parse(apply->msgbuf);
    if (apply_json == NULL) {
        KR_LOG(KR_LOGERROR, "parse apply json failed!");
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }
    int id = (int )cJSON_GetNumber(apply_json, "sdi_id");
    T_KRSDI *krsdi = kr_sdi_lookup(krctx->ptData->ptSdiTable, id);
    if (krsdi == NULL) {
        KR_LOG(KR_LOGERROR, "SDI %d not found!", id);
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }

    cJSON *json = kr_sdi_info(krsdi);
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}

static void kr_engine_info_ddi(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    cJSON *apply_json = cJSON_Parse(apply->msgbuf);
    if (apply_json == NULL) {
        KR_LOG(KR_LOGERROR, "parse apply json failed!");
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }
    int id = (int )cJSON_GetNumber(apply_json, "ddi_id");
    T_KRDDI *krddi = kr_ddi_lookup(krctx->ptData->ptDdiTable, id);
    if (krddi == NULL) {
        KR_LOG(KR_LOGERROR, "DDI %d not found!", id);
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }

    cJSON *json = kr_ddi_info(krddi);
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}

static void kr_engine_info_hdi(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    cJSON *apply_json = cJSON_Parse(apply->msgbuf);
    if (apply_json == NULL) {
        KR_LOG(KR_LOGERROR, "parse apply json failed!");
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }
    int id = (int )cJSON_GetNumber(apply_json, "hdi_id");
    T_KRHDI *krhdi = kr_hdi_lookup(krctx->ptData->ptHdiTable, id);
    if (krhdi == NULL) {
        KR_LOG(KR_LOGERROR, "DDI %d not found!", id);
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }

    cJSON *json = kr_hdi_info(krhdi);
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}


static void kr_engine_insert_event(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    /* insert event into krdb */
    /*FIXME:convert message to record first
    krctx->ptCurrRec = kr_db_insert(
            krctx->ptEnv->ptDB, apply->datasrc, apply->msgbuf);
    if (krctx->ptCurrRec == NULL) {
        KR_LOG(KR_LOGERROR, "kr_db_insert [%d] [%s] failed!", 
                apply->datasrc, apply->msgbuf);
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }
    */

    reply->msgtype = KR_MSGTYPE_SUCCESS;
}

static void kr_engine_detect_event(T_KRContext *krctx, T_KREngineArg *krarg)
{
    T_KRMessage *apply = krarg->apply;
    T_KRMessage *reply = krarg->reply;

    /* insert event into krdb */
    /*FIXME:convert message to record first
    krctx->ptCurrRec = kr_db_insert(
            krctx->ptEnv->ptDB, apply->datasrc, apply->msgbuf);
    if (krctx->ptCurrRec == NULL) {
        KR_LOG(KR_LOGERROR, "kr_db_insert [%d] [%s] failed!", 
                apply->datasrc, apply->msgbuf);
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }
    */

    /* group route and rule detect */
    if (kr_flow_detect(krctx->ptFlow, krctx->ptCurrRec) != 0) {
        KR_LOG(KR_LOGERROR, "kr_engine_detect failed!");
        reply->msgtype = KR_MSGTYPE_ERROR;
        return;
    }

    cJSON *json = kr_context_dump_json(krctx);
    reply->msgbuf = cJSON_PrintUnformatted(json);
    reply->msglen = strlen(reply->msgbuf)+1;
    reply->msgtype = KR_MSGTYPE_SUCCESS;
    cJSON_Delete(json);
}

