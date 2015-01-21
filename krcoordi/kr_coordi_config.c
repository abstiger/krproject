#include "kr_coordi.h"

int kr_coordi_config_parse(char *configfile, T_KRCoordi *krcoordi)
{
    FILE *f=fopen(configfile, "rb");
    if (f == NULL) {
        fprintf(stderr, "open configure file %s failed!\n", configfile);
        return -1;
    }
    fseek(f,0,SEEK_END); long len=ftell(f); fseek(f,0,SEEK_SET);
    char *data=(char *)kr_calloc(len+1); int n=fread(data,1,len,f); fclose(f);

    cJSON *krjson = cJSON_Parse(data); kr_free(data);
    if (krjson == NULL) {
        fprintf(stderr, "parse configfile %s failed[%s]!\n", 
                configfile, cJSON_GetErrorPtr());
        return -1;
    }

    /*coordi*/
    cJSON *coordi = cJSON_GetObjectItem(krjson, "coordi");
    if (coordi == NULL) {
        fprintf(stderr, "configfile %s miss coordi section!\n", configfile);
        cJSON_Delete(krjson); 
        return -1;
    }
    krcoordi->coordiid = kr_string_dupenv(cJSON_GetString(coordi, "COORDIID"));
    krcoordi->daemonize = (int )cJSON_GetNumber(coordi, "DAEMONIZE");
    krcoordi->pidfile = kr_string_dupenv(cJSON_GetString(coordi, "PIDFILE"));
    krcoordi->maxevents = (int )cJSON_GetNumber(coordi, "MAXEVENTS");
    krcoordi->tcpbindaddr = kr_string_dupenv(cJSON_GetString(coordi, "TCPBINDADDR"));
    krcoordi->tcpport = (int )cJSON_GetNumber(coordi, "TCPPORT");

    krcoordi->logpath = kr_string_dupenv(cJSON_GetString(coordi, "LOGPATH"));
    krcoordi->logname = kr_string_dupenv(cJSON_GetString(coordi, "LOGNAME"));
    krcoordi->loglevel = (int )cJSON_GetNumber(coordi, "LOGLEVEL");

    cJSON_Delete(krjson);
    return 0;
}

void kr_coordi_config_dump(T_KRCoordi *krcoordi, FILE *fp)
{
    fprintf(fp, "Dumping Coordi Configure...");
    fprintf(fp, "configfile[%s]\n coordiid[%s]\n daemonize[%d]\n ", \
        krcoordi->configfile, krcoordi->coordiid, krcoordi->daemonize);
    fprintf(fp, "logpath[%s], logname[%s], loglevel[%d]\n ", \
        krcoordi->logpath , krcoordi->logname, krcoordi->loglevel);
    fprintf(fp, "maxevents[%d]\n tcpport[%d]\n tcpbindaddr[%s]\n ", \
        krcoordi->maxevents, krcoordi->tcpport, krcoordi->tcpbindaddr);    
}


void kr_coordi_config_reset(T_KRCoordi *krcoordi)
{
    if (krcoordi->configfile) kr_free(krcoordi->configfile);
    if (krcoordi->coordiid) kr_free(krcoordi->coordiid);
    if (krcoordi->logpath) kr_free(krcoordi->logpath);
    if (krcoordi->logname) kr_free(krcoordi->logname);
    if (krcoordi->tcpbindaddr) kr_free(krcoordi->tcpbindaddr);
}
