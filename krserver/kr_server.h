#ifndef __KR_SERVER_H__
#define __KR_SERVER_H__

#include "krutils/kr_utils.h"
#include "krengine/kr_engine.h"
#include "krchannel/kr_channel.h"
#include "kr_server_config.h"

typedef struct _kr_server_t
{
    int               status;                 /* status of this server */

    char             *config_file;            /* configure file */
    T_KRServerConfig *config;                 /* krserver configuration */
    T_KREngine       *krengine;               /* the Engine */
    int               channel_count;          /* channel count */
    T_KRChannel     **krchannels;             /* the channels */
}T_KRServer;

#endif  /*__KR_SERVER_H__*/
