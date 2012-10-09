#include "kr_coordi.h"

int kr_coordi_config_parse(char *configfile, T_KRCoordi *coordi)
{
    char buf[1024] = {0};
    if (configfile == NULL) {
        fprintf(stderr, "Start your krcoordi with specified configure file!\n");
        return -1;
    } else if (kr_config_setfile(configfile) != 0) {
        fprintf(stderr, "kr_config_setfile [%s] failed!\n", configfile);
        return -1;
    }
    
    memset(buf, 0x00, sizeof(buf));
    if (kr_config_getstring("SYSTEM", "COORDIID", buf) != 0) {
        printf("kr_config_getstring COORDIID failure!");
        return -1;
    }
    if (strlen(buf) != 0) coordi->coordiid = kr_strdup(buf);
printf("coordi->coordiid=[%s]\n", coordi->coordiid);
    
    if (kr_config_getint("SYSTEM", "DAEMONIZE", &coordi->daemonize) != 0) {
        printf("kr_config_getint DAEMONIZE failure!");
        return -1;
    }

    memset(buf, 0x00, sizeof(buf));
    if (kr_config_getstring("SYSTEM", "LOGPATH", buf) != 0) {
        printf("kr_config_getstring LOGPATH failure!");
        return -1;
    }
    if (strlen(buf) != 0) coordi->logpath = kr_strdup(buf);
    
    if (kr_config_getint("SYSTEM", "LOGLEVEL", &coordi->loglevel) != 0) {
        printf("kr_config_getint LOGLEVEL failure!");
        return -1;
    }
    
    if (kr_config_getint("SYSTEM", "MAXEVENTS", &coordi->maxevents) != 0) {
        printf("kr_config_getint MAXEVENTS failure!");
        return -1;
    }
    
    if (kr_config_getint("NETWORK", "TCPPORT", &coordi->tcpport) != 0) {
        printf("kr_config_getint TCPPORT failure!");
        return -1;
    }
    
    memset(buf, 0x00, sizeof(buf));
    if (kr_config_getstring("NETWORK", "TCPBINDADDR", buf) != 0) {
        printf("kr_config_getstring TCPBINDADDR failure!");
        return -1;
    }
    if (strlen(buf) != 0) coordi->tcpbindaddr = kr_strdup(buf);
printf("coordi->tcpbindaddr=[%s]\n", coordi->tcpbindaddr);
    
    memset(buf, 0x00, sizeof(buf));
    if (kr_config_getstring("NETWORK", "UNIXDOMAIN", buf) != 0) {
        printf("kr_config_getstring UNIXDOMAIN failure!");
        return -1;
    }
    if (strlen(buf) != 0) coordi->unixdomain = kr_strdup(buf);
    
    if (kr_config_getint("NETWORK", "UNIXDOMAINPERM", &coordi->unixdomainperm) != 0) {
        printf("kr_config_getint UNIXDOMAINPERM failure!");
        return -1;
    }

    return 0;
}


void kr_coordi_config_dump(T_KRCoordi *coordi, FILE *fp)
{
    fprintf(fp, "Dumping Coordi Configure...");
    fprintf(fp, "configfile[%s]\n coordiid[%s]\n daemonize[%d]\n ", \
        coordi->configfile, coordi->coordiid, coordi->daemonize);
    fprintf(fp, "loglevel[%d]\n logpath[%s]", \
        coordi->loglevel, coordi->logpath);
    fprintf(fp, "maxevents[%d]\n shutdown[%d]\n ", \
        coordi->maxevents, coordi->shutdown);    
    fprintf(fp, "tcpport[%d]\n tcpbindaddr[%s]\n unixdomain[%s]\n unixdomainperm[%d]", \
        coordi->tcpport, coordi->tcpbindaddr, coordi->unixdomain, coordi->unixdomainperm);    
}


void kr_coordi_config_reset(T_KRCoordi *coordi)
{
    if (coordi->configfile) kr_free(coordi->configfile);
    if (coordi->coordiid) kr_free(coordi->coordiid);
    if (coordi->logpath) kr_free(coordi->logpath);
    if (coordi->tcpbindaddr) kr_free(coordi->tcpbindaddr);
    if (coordi->unixdomain) kr_free(coordi->unixdomain);
}
