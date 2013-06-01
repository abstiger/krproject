#ifndef __KR_SERVER_H__
#define __KR_SERVER_H__

#include "krutils/kr_utils.h"
#include "krutils/kr_threadpool.h"
#include "krutils/kr_cache.h"
#include "krshm/kr_shm.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"
#include "krrule/kr_rule.h"
#include "krengine/kr_engine.h"
#include "kr_message.h"

typedef struct _kr_server_t
{
    /* General */
    char *configfile;             /* krserver's configure file */
    char *serverid;               /* server identifier */
    char *krdbmodule;             /* krdb's module file */
    char *datamodule;             /* data's module file */
    char *rulemodule;             /* rule's module file */
    int  daemonize;               /* True if running as a daemon */
    char *pidfile;                /* PID file path */
    int  maxevents;               /* the max event number supported */
    int  hdicachesize;            /* cache size of historical dynamic item */
    
    int shmkey;                   /* share memory key */
    int detectmode;               /* detect mode */
    int dumpinfo;                 /* dump info while server down:0-no,1-yes */
    int loglevel;                 /* log level of this server */
    char *logpath;                /* Path of log file */
    
    int threadcnt;                /* Thread pool size, 0: no threads*/
    int hwm;                      /* Thread pool high water mark*/
    int shutdown;                 /* Shutdown flag */
    char *dbname;                 /* data source name */
    char *dbuser;                 /* database user */
    char *dbpass;                 /* database password */

    /* Networking */
    int tcpport;                  /* TCP listening port */
    char *tcpbindaddr;            /* Bind address or NULL */
    int  clustermode;             /* Server in cluder:1 or not:0 */
    int  weights;                 /* weights for this server in cluster mode */
    int  replica;                 /* a replication server:1 or not:0*/
    int coordport;                /* Coordinator port */
    char *coordip;                /* Coordinator ip */
    int ipfd;                     /* TCP socket file descriptor */
    int sofd;                     /* Unix socket file descriptor */
    int cofd;                     /* Coordinator connecting socket */
    int retrytimes;               /* retry times for connecting cluster */
    int retryinterval;            /* retry interval for connecting cluster */
    char neterr[KR_NET_ERR_LEN];  /* Error buffer for kr_net.c */
        
    /* Global variables*/
    T_KREngine       *krengine;   /* The Engine */
    T_KREventLoop    *krel;       /* the main event loop */
}T_KRServer;

#endif  /*__KR_SERVER_H__*/
