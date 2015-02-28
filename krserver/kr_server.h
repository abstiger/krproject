#ifndef __KR_SERVER_H__
#define __KR_SERVER_H__

#include "krutils/kr_utils.h"
#include "krutils/kr_event.h"
#include "krengine/kr_engine.h"
#include "krengine/kr_message.h"
#include "kr_server_config.h"


typedef struct _kr_client_t T_KRClient;
typedef struct _kr_server_t T_KRServer;


struct _kr_client_t
{
    int               fd;

    T_KRBuffer       *inbuf;
    T_KRMessage      *inmsg;
    
    T_KRServer       *krserver;
};


struct _kr_server_t
{
    char             *config_file;        /* configure file */
    T_KRServerConfig *config;             /* krserver configuration */

    T_KREventLoop    *krel;               /* the main event loop */
    T_KREngine       *krengine;           /* the engine layer */

    int               ipfd;               /* TCP socket file descriptor */
    int               cofd;               /* Coordinator connecting socket */
    char              neterr[KR_NET_ERR_LEN];

    T_KRList         *clients;
};

#endif  /*__KR_SERVER_H__*/
