#ifndef __KR_SERVER_CONFIG_H__
#define __KR_SERVER_CONFIG_H__

#include "krengine/kr_engine.h"
#include "krchannel/kr_channel.h"

typedef struct _kr_cluster_config_t
{
    int  clustermode;             /* Server in cluder:1 or not:0 */
    int  weights;                 /* weights for this server in cluster mode */
    int  replica;                 /* a replication server:1 or not:0*/
    int  coordport;               /* Coordinator port */
    char *coordip;                /* Coordinator ip */
    int  retrytimes;              /* retry times for connecting cluster */
    int  retryinterval;           /* retry interval for connecting cluster */
}T_KRClusterConfig;


typedef struct _kr_server_config_t
{
    /* server config section */
    char              *serverid;                /* server identifier */
    int                daemonize;               /* True if running as a daemon */
    char              *pidfile;                 /* PID file path */

    /* channel config section */
    int                channel_count;
    T_KRChannelConfig **channels;
    
    /* engine config section */
    T_KREngineConfig  *engine;

    /* cluster config section*/
    T_KRClusterConfig *cluster;
}T_KRServerConfig;


/* function declaration */
T_KRServerConfig *kr_server_config_parse(char *config_file);
void kr_server_config_free(T_KRServerConfig *server);

#endif  /*__KR_SERVER_CONFIG_H__*/
