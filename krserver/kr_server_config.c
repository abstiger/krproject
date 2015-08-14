#include "kr_server_config.h"
#include "krutils/kr_utils.h"

#define _dupenv(x) kr_string_dupenv(x)

T_KRServerConfig *kr_server_config_parse(char *config_file)
{
    FILE *f=fopen(config_file, "rb");
    if (f == NULL) {
        fprintf(stderr, "open configure file %s failed!\n", config_file);
        goto fail;
    }
    fseek(f,0,SEEK_END); long len=ftell(f); fseek(f,0,SEEK_SET);
    char *data=(char *)kr_calloc(len+1); int n=fread(data,1,len,f); fclose(f);

    cJSON *krjson = cJSON_Parse(data); kr_free(data);
    if (krjson == NULL) {
        fprintf(stderr, "parse config_file %s failed[%s]!\n",
                config_file, cJSON_GetErrorPtr());
        goto fail;
    }


    /*server config section*/
    cJSON *server = cJSON_GetObjectItem(krjson, "server");
    if (server == NULL) {
        fprintf(stderr, "config_file %s miss server section!\n", config_file);
        goto fail;
    }
    T_KRServerConfig *krserver = kr_calloc(sizeof(*krserver));
    krserver->serverid = _dupenv(cJSON_GetString(server, "serverid"));
    krserver->daemonize = (int )cJSON_GetNumber(server, "daemonize");
    krserver->pidfile = _dupenv(cJSON_GetString(server, "pidfile"));
    krserver->tcpport = (int )cJSON_GetNumber(server, "tcpport");
    krserver->tcpbindaddr = _dupenv(cJSON_GetString(server, "tcpbindaddr"));

    /*engine config section*/
    cJSON *engine = cJSON_GetObjectItem(server, "engine");
    if (engine == NULL) {
        fprintf(stderr, "config_file %s miss engine section!\n", config_file);
        goto fail;
    }
    T_KREngineConfig *krengine = kr_calloc(sizeof(*krengine));
    krengine->logpath = _dupenv(cJSON_GetString(engine, "logpath"));
    krengine->logname = _dupenv(cJSON_GetString(engine, "logname"));
    krengine->loglevel = (int )cJSON_GetNumber(engine, "loglevel");

    krengine->param_url = _dupenv(cJSON_GetString(engine, "param_url"));

    krengine->input_module = _dupenv(cJSON_GetString(engine, "input_module"));
    krengine->output_module = _dupenv(cJSON_GetString(engine, "output_module"));

    krengine->hdi_cache_size = (int )cJSON_GetNumber(engine, "hdi_cache_size");
    krengine->thread_pool_size = (int )cJSON_GetNumber(engine, "thread_pool_size");
    krengine->high_water_mark = (int )cJSON_GetNumber(engine, "high_water_mark");
    krserver->engine = krengine;

    /*cluster config section*/
    cJSON *cluster = cJSON_GetObjectItem(server, "cluster");
    if (cluster == NULL) {
        fprintf(stderr, "config_file %s miss cluster section!\n", config_file);
        goto fail;
    }
    T_KRClusterConfig *krcluster = kr_calloc(sizeof(*krcluster));
    krcluster->clustermode = (int )cJSON_GetNumber(cluster, "clustermode");
    krcluster->weights = (int )cJSON_GetNumber(cluster, "weights");
    krcluster->replica = (int )cJSON_GetNumber(cluster, "replica");
    krcluster->coordip = _dupenv(cJSON_GetString(cluster, "coordip"));
    krcluster->coordport = (int )cJSON_GetNumber(cluster, "coordport");
    krcluster->retrytimes = (int )cJSON_GetNumber(cluster, "retrytimes");
    krcluster->retryinterval = (int )cJSON_GetNumber(cluster, "retryinterval");
    krserver->cluster = krcluster;
    
    cJSON_Delete(krjson);
    return krserver;

fail:
    if (krjson) cJSON_Delete(krjson); 
    kr_server_config_free(krserver);
    return NULL;
}


void kr_server_config_free(T_KRServerConfig *server)
{
    if (server == NULL) return;

    /*server config section*/
    if (server->serverid) kr_free(server->serverid);
    if (server->pidfile) kr_free(server->pidfile);
    if (server->tcpbindaddr) kr_free(server->tcpbindaddr);
    
    /*engine config section*/
    if (server->engine) {
        T_KREngineConfig *engine = server->engine;
        if (engine->logpath) kr_free(engine->logpath);
        if (engine->logname) kr_free(engine->logname);
        if (engine->param_url) kr_free(engine->param_url);
        if (engine->input_module) kr_free(engine->input_module);
        if (engine->output_module) kr_free(engine->output_module);
    }

    /*cluster config section*/
    if (server->cluster) {
        T_KRClusterConfig *cluster = server->cluster;
        if (cluster->coordip) kr_free(cluster->coordip);
    }
}

