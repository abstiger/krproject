#ifndef __KR_SERVER_CONFIG_H__
#define __KR_SERVER_CONFIG_H__

#include "krengine/kr_engine.h"

typedef struct _kr_server_config_t
{
    /* server config section */
    char   *serverid;            /* server identifier */
    int     daemonize;           /* True if running as a daemon */
    char   *pidfile;             /* PID file path */
    int     tcpport;             /* TCP listening port */
    char   *tcpbindaddr;         /* Bind address or NULL */

    /* engine config section */
    T_KREngineConfig  *engine;

}T_KRServerConfig;


/* function declaration */
T_KRServerConfig *kr_server_config_parse(char *config_file);
void kr_server_config_free(T_KRServerConfig *server);

#endif  /*__KR_SERVER_CONFIG_H__*/
