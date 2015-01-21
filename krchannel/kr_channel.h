#ifndef __KR_CHANNEL_H__
#define __KR_CHANNEL_H__

#include "krutils/kr_utils.h"
#include "krengine/kr_message.h"
#include "kr_transport.h"
#include "kr_protocol.h"

typedef int (*KRChannelFeedFunc)(void *krchannel, T_KRMessage *apply, T_KRMessage *reply, void *data);

typedef struct _kr_channel_config_t 
{
    char  *name;
    char  *desc;

    T_KRTransportConfig *transport_config;
    T_KRProtocolConfig  *protocol_config;
}T_KRChannelConfig;


typedef struct _kr_channel_t
{
    T_KRChannelConfig *channel_config;
    KRChannelFeedFunc  feed_func;
    void              *data;

    pthread_t          thread;

    /*recv and send buffer*/
    T_KRBuffer *buffer_recv;
    T_KRBuffer *buffer_send;

    /*transport layer*/
    T_KRTransport *transport;

    /*protocol layer*/
    T_KRProtocol *protocol;

}T_KRChannel;



/* functions of krchannel */
T_KRChannel *kr_channel_open(T_KRChannelConfig *channel_config, KRChannelFeedFunc feed_func, void *data);
void kr_channel_close(T_KRChannel *channel);
int kr_channel_recv(T_KRChannel *channel, T_KRMessage *message);
int kr_channel_send(T_KRChannel *channel, T_KRMessage *message);

#endif  /*__KR_CHANNEL_H__*/

