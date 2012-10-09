#ifndef __KR_COORDI_H__
#define __KR_COORDI_H__

#include "krutils/kr_utils.h"
#include "krutils/kr_skiplist.h"
#include "krutils/kr_conhash.h"
#include "krutils/kr_message.h"

typedef enum {
    KR_COORDI_SERVER = 'S',
    KR_COORDI_CLIENT = 'C'
}E_KRCoordiRole;


typedef struct _kr_coordi_connector_t
{
    int            fd;
    E_KRCoordiRole role;
    char           id[100];
    char           ip[32];
    unsigned int   aplnum;
    unsigned int   rplnum;
    unsigned int   errnum;
    
    unsigned int   weights;
    int            replica;
    char           neterr[KR_NET_ERR_LEN];  /* Error buffer for kr_net.c */
}T_KRCoordiConnector;

typedef struct _kr_coordi_t
{
    /* General */
    char *configfile;             /* krcoordi's configure file */
    char *coordiid;               /* coordi identifier */
    int daemonize;                /* True if running as a daemon */
    int loglevel;                 /* log level of this coordi */
    char *logpath;                /* Path of log file */
    int shutdown;                 /* Shutdown flag */
    int  maxevents;               /* the max event number supported */

    /* Networking */
    int tcpport;                  /* TCP listening port */
    char *tcpbindaddr;            /* Bind address or NULL */
    char *unixdomain;             /* UNIX domain path */
    mode_t unixdomainperm;        /* UNIX domain permission */
    int ipfd;                     /* TCP socket file descriptor */
    int sofd;                     /* Unix socket file descriptor */
    char neterr[KR_NET_ERR_LEN];  /* Error buffer for kr_net.c */
        
    /**/
    T_KREventLoop    *el;         /* Event loop for krcoordi */
    T_KRConHash      *servers;    /* Consistent hash for server nodes */
    T_KRList         *clients;    /* list for client nodes */
    T_KRHashTable    *fdtable;    /* use for searching server/client with fd */
}T_KRCoordi;

#endif  /*__KR_COORDI_H__*/
