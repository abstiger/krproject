#include "kr_server.h"

int kr_server_config_parse(char *configfile, T_KRServer *server)
{
    char buf[1024] = {0};
    if (configfile == NULL) {
        fprintf(stderr, "Start your krserver with specified configure file!\n");
        return -1;
    } else if (kr_config_setfile(configfile) != 0) {
        fprintf(stderr, "kr_config_setfile [%s] failed!\n", configfile);
        return -1;
    }
    
    memset(buf, 0x00, sizeof(buf));
    if (kr_config_getstring("SYSTEM", "SERVERID", buf) != 0) {
        printf("kr_config_getstring PIDFILE failure!");
        return -1;
    }
    if (strlen(buf) != 0) server->serverid = kr_strdup(buf);
printf("server->serverid=[%s]\n", server->serverid);

    memset(buf, 0x00, sizeof(buf));
    if (kr_config_getstring("SYSTEM", "KRDBMODULE", buf) != 0) {
        printf("kr_config_getstring KRDBMODULE failure!");
        return -1;
    }
    if (strlen(buf) != 0) server->krdbmodule = kr_strdup(buf);
printf("server->krdbmodule=[%s]\n", server->krdbmodule);

    memset(buf, 0x00, sizeof(buf));
    if (kr_config_getstring("SYSTEM", "DATAMODULE", buf) != 0) {
        printf("kr_config_getstring DATAMODULE failure!");
        return -1;
    }
    if (strlen(buf) != 0) server->datamodule = kr_strdup(buf);
printf("server->datamodule=[%s]\n", server->datamodule);

    memset(buf, 0x00, sizeof(buf));
    if (kr_config_getstring("SYSTEM", "RULEMODULE", buf) != 0) {
        printf("kr_config_getstring RULEMODULE failure!");
        return -1;
    }
    if (strlen(buf) != 0) server->rulemodule = kr_strdup(buf);
printf("server->rulemodule=[%s]\n", server->rulemodule);

    if (kr_config_getint("SYSTEM", "DAEMONIZE", &server->daemonize) != 0) {
        printf("kr_config_getint DAEMONIZE failure!");
        return -1;
    }
    
    memset(buf, 0x00, sizeof(buf));
    if (kr_config_getstring("SYSTEM", "PIDFILE", buf) != 0) {
        printf("kr_config_getstring PIDFILE failure!");
        return -1;
    }
    if (strlen(buf) != 0) server->pidfile = kr_strdup(buf);
    
    if (kr_config_getint("SYSTEM", "MAXEVENTS", &server->maxevents) != 0) {
        printf("kr_config_getint MAXEVENTS failure!");
        return -1;
    }
    
    if (kr_config_getint("SYSTEM", "DETECTMODE", &server->detectmode) != 0) {
        printf("kr_config_getint DETECTMODE failure!");
        return -1;
    }
    
    memset(buf, 0x00, sizeof(buf));
    if (kr_config_getstring("SYSTEM", "LOGPATH", buf) != 0) {
        printf("kr_config_getstring LOGPATH failure!");
        return -1;
    }
    if (strlen(buf) != 0) server->logpath = kr_strdup(buf);

    if (kr_config_getint("SYSTEM", "LOGLEVEL", &server->loglevel) != 0) {
        printf("kr_config_getint LOGLEVEL failure!");
        return -1;
    }

    if (kr_config_getint("SYSTEM", "DUMPINFO", &server->dumpinfo) != 0) {
        printf("kr_config_getint DUMPINFO failure!");
        return -1;
    }
    
    if (kr_config_getint("SYSTEM", "THREADCNT", &server->threadcnt) != 0) {
        printf("kr_config_getint THREADCNT failure!");
        return -1;
    }
printf("server->threadcnt=[%d]\n", server->threadcnt);

    if (kr_config_getint("SYSTEM", "HIGHWATERMARK", &server->hwm) != 0) {
        printf("kr_config_getint HIGHWATERMARK failure!");
        return -1;
    }
printf("server->hwm=[%d]\n", server->hwm);

    if (kr_config_getint("SYSTEM", "HDICACHESIZE", &server->hdicachesize) != 0) {
        printf("kr_config_getint HDICACHESIZE failure!");
        return -1;
    }
printf("server->hdicachesize=[%d]\n", server->hdicachesize);

    memset(buf, 0x00, sizeof(buf));
    if (kr_config_getstring("SYSTEM", "DBNAME", buf) != 0) {
        printf("kr_config_getstring DBNAME failure!");
        return -1;
    }
    if (strlen(buf) != 0) server->dbname = kr_strdup(buf);
printf("server->dbname=[%s]\n", server->dbname);

    memset(buf, 0x00, sizeof(buf));
    if (kr_config_getstring("SYSTEM", "DBUSER", buf) != 0) {
        printf("kr_config_getstring DBUSER failure!");
        return -1;
    }
    if (strlen(buf) != 0) server->dbuser = kr_strdup(buf);
printf("server->dbuser=[%s]\n", server->dbuser);

    memset(buf, 0x00, sizeof(buf));
    if (kr_config_getstring("SYSTEM", "DBPASS", buf) != 0) {
        printf("kr_config_getstring DBPASS failure!");
        return -1;
    }
    if (strlen(buf) != 0) server->dbpass = kr_strdup(buf);
printf("server->dbpass=[%s]\n", server->dbpass);
    
    if (kr_config_getint("NETWORK", "TCPPORT", &server->tcpport) != 0) {
        printf("kr_config_getint TCPPORT failure!");
        return -1;
    }
    
    memset(buf, 0x00, sizeof(buf));
    if (kr_config_getstring("NETWORK", "TCPBINDADDR", buf) != 0) {
        printf("kr_config_getstring TCPBINDADDR failure!");
        return -1;
    }
    if (strlen(buf) != 0) server->tcpbindaddr = kr_strdup(buf);
printf("server->tcpbindaddr=[%s]\n", server->tcpbindaddr);
    
    if (kr_config_getint("NETWORK", "CLUSTERMODE", &server->clustermode) != 0) {
        printf("kr_config_getint CLUSTERMODE failure!");
        return -1;
    }
    
    if (kr_config_getint("NETWORK", "WEIGHTS", &server->weights) != 0) {
        printf("kr_config_getint WEIGHTS failure!");
        return -1;
    }

    if (kr_config_getint("NETWORK", "REPLICA", &server->replica) != 0) {
        printf("kr_config_getint REPLICA failure!");
        return -1;
    }
        
    if (kr_config_getint("NETWORK", "COORDPORT", &server->coordport) != 0) {
        printf("kr_config_getint COORDPORT failure!");
        return -1;
    }

    memset(buf, 0x00, sizeof(buf));
    if (kr_config_getstring("NETWORK", "COORDIP", buf) != 0) {
        printf("kr_config_getstring COORDIP failure!");
        return -1;
    }
    if (strlen(buf) != 0) server->coordip = kr_strdup(buf);
    
    if (kr_config_getint("NETWORK", "RETRYTIMES", &server->retrytimes) != 0) {
        printf("kr_config_getint RETRYTIMES failure!");
        return -1;
    }
    
    if (kr_config_getint("NETWORK", "RETRYINTERVAL", &server->retryinterval) != 0) {
        printf("kr_config_getint RETRYINTERVAL failure!");
        return -1;
    }
    
    return 0;
}


void kr_server_config_dump(T_KRServer *server, FILE *fp)
{
    fprintf(fp, "Dumping Server Configure...\n ");
    fprintf(fp, "configfile[%s]\n serverid[%s]\n daemonize[%d]\n pidfile[%s]\n ", \
        server->configfile, server->serverid, server->daemonize, server->pidfile);
    fprintf(fp, "detectmode[%d]\n loglevel[%d]\n logpath[%s]", \
        server->detectmode, server->loglevel, server->logpath);
    fprintf(fp, "krdbmodule[%s]\n datamodule[%s]\n rulemodule[%s]\n", \
        server->krdbmodule, server->datamodule, server->rulemodule);
    fprintf(fp, "threadcnt[%d]\n hwm[%d]\n maxevents[%d]\n shutdown[%d]\n ", \
        server->threadcnt, server->hwm, server->maxevents, server->shutdown);
    fprintf(fp, "tcpport[%d]\n tcpbindaddr[%s]\n ", \
        server->tcpport, server->tcpbindaddr);    
    fprintf(fp, "clustermode[%d]\n weights[%d]\n coordport[%d]\n coordip[%s]", \
        server->clustermode, server->weights, server->coordport, server->coordip);
}


void kr_server_config_free(T_KRServer *server)
{
    if (server->configfile) kr_free(server->configfile);
    if (server->serverid) kr_free(server->serverid);
    if (server->krdbmodule) kr_free(server->krdbmodule);
    if (server->datamodule) kr_free(server->datamodule);
    if (server->rulemodule) kr_free(server->rulemodule);
    if (server->pidfile) kr_free(server->pidfile);
    if (server->logpath) kr_free(server->logpath);
    if (server->dbname) kr_free(server->dbname);
    if (server->dbuser) kr_free(server->dbuser);
    if (server->dbpass) kr_free(server->dbpass);
    if (server->tcpbindaddr) kr_free(server->tcpbindaddr);
    if (server->coordip) kr_free(server->coordip);
}
