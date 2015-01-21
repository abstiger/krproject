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

    /*channels config section*/
    cJSON *channels = cJSON_GetObjectItem(krjson, "channels");
    if (channels == NULL) {
        fprintf(stderr, "config_file %s miss channels section!\n", config_file);
        goto fail;
    }
    krserver->channel_count = cJSON_GetArraySize(channels);
    krserver->channels = kr_calloc(sizeof(T_KRChannelConfig)*krserver->channel_count);
    for (int i=0; i<krserver->channel_count; ++i) {
        cJSON *channel = cJSON_GetArrayItem(channels, i);
        T_KRChannelConfig *krchannel = krserver->channels[i];

        //FIXME:moved it to transport layer
        //krchannel->addr = _dupenv(cJSON_GetString(channel, "TCPBINDADDR"));
        //krchannel->port = (int )cJSON_GetNumber(channel, "TCPPORT");
    }

    /*engine config section*/
    cJSON *engine = cJSON_GetObjectItem(krjson, "engine");
    if (engine == NULL) {
        fprintf(stderr, "config_file %s miss engine section!\n", config_file);
        goto fail;
    }
    T_KREngineConfig *krengine = kr_calloc(sizeof(*krengine));
    krengine->dbname = _dupenv(cJSON_GetString(engine, "dbname"));
    krengine->dbuser = _dupenv(cJSON_GetString(engine, "dbuser"));
    krengine->dbpass = _dupenv(cJSON_GetString(engine, "dbpass"));

    krengine->logpath = _dupenv(cJSON_GetString(engine, "logpath"));
    krengine->logname = _dupenv(cJSON_GetString(engine, "logname"));
    krengine->loglevel = (int )cJSON_GetNumber(engine, "loglevel");

    krengine->krdb_module = _dupenv(cJSON_GetString(engine, "krdb_module"));
    krengine->data_module = _dupenv(cJSON_GetString(engine, "data_module"));
    krengine->rule_module = _dupenv(cJSON_GetString(engine, "rule_module"));

    krengine->thread_pool_size = (int )cJSON_GetNumber(engine, "thread_pool_size");
    krengine->high_water_mark = (int )cJSON_GetNumber(engine, "high_water_mark");
    krengine->hdi_cache_size = (int )cJSON_GetNumber(engine, "hdi_cache_size");
    krserver->engine = krengine;

    /*cluster config section*/
    cJSON *cluster = cJSON_GetObjectItem(krjson, "cluster");
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

    /*channel config section*/
    if (server->channel_count > 0) {
        for (int i=0; i<server->channel_count; ++i) {
            T_KRChannelConfig *channel = server->channels[i];

            //FIXME:
            //if (channel->addr) kr_free(channel->addr);
        }

    }
    
    /*engine config section*/
    if (server->engine) {
        T_KREngineConfig *engine = server->engine;
        if (engine->logpath) kr_free(engine->logpath);
        if (engine->logname) kr_free(engine->logname);
        if (engine->dbname) kr_free(engine->dbname);
        if (engine->dbuser) kr_free(engine->dbuser);
        if (engine->dbpass) kr_free(engine->dbpass);
        if (engine->krdb_module) kr_free(engine->krdb_module);
        if (engine->data_module) kr_free(engine->data_module);
        if (engine->rule_module) kr_free(engine->rule_module);
    }

    /*cluster config section*/
    if (server->cluster) {
        T_KRClusterConfig *cluster = server->cluster;
        if (cluster->coordip) kr_free(cluster->coordip);
    }
}

