#include "kr_channel.h"


static void *channel_routine(void *arg)
{
    T_KRChannel *kr_channel = (T_KRChannel *)arg;

    int ret = 0, finished = 1;
    T_KRMessage *apply = NULL, *reply = NULL;

    while(1) {

        /*allocate new message if last finished*/
        if (finished) {
            apply = kr_message_alloc();
            reply = kr_message_alloc();
            finished = 0;
        }

        /*get message first*/
        ret = kr_channel_recv(kr_channel, apply);
        if (ret < 0) {
            KR_LOG(KR_LOGERROR, "run feed function failed!");
            finished = 1;
            continue;
        } else if (ret > 0) {
            KR_LOG(KR_LOGDEBUG, "channel receive unfinished!");
            continue;
        }

        /*feed it*/
        ret = kr_channel->feed_func(kr_channel, apply, reply, kr_channel->data);
        if (ret != 0) {
            KR_LOG(KR_LOGERROR, "run feed function failed!");
            finished = 1;
            continue;
        } 

        finished = 1;
    }


    return NULL;
}


T_KRChannel *kr_channel_open(T_KRChannelConfig *channel_config, KRChannelFeedFunc feed_func, void *data)
{
    /*calloc channel*/
    T_KRChannel *channel = kr_calloc(sizeof(*channel));
    if (channel == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc channel failed!");
        return NULL;
    }
    channel->channel_config = channel_config;
    channel->feed_func = feed_func;
    channel->data = data;

    /*open transport of this channel*/
    T_KRTransportConfig *transport_config = channel_config->transport_config;
    channel->transport = transport_config->func->open(transport_config);
    if (channel->transport == NULL) {
        KR_LOG(KR_LOGERROR, "open channel transport failed!");
        kr_free(channel);
        return NULL;
    }
    
    /* Create detached channel thread */
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (pthread_create(&channel->thread, &attr, 
                channel_routine, (void *)channel) != 0) {
        KR_LOG(KR_LOGERROR, "create channel thread failed!");
        kr_free(channel);
        return NULL;

    }
    pthread_attr_destroy(&attr);

    return channel;
}


void kr_channel_close(T_KRChannel *channel)
{
    if (channel) {
        /*close transport of this channel*/
        T_KRTransportConfig *transport_config = channel->channel_config->transport_config;
        transport_config->func->close(channel->transport);

        /*cancel detached channel thread*/
        if (channel->thread) pthread_cancel(channel->thread);

        /*free channel*/
        kr_free(channel);
    }

}


int kr_channel_recv(T_KRChannel *channel, T_KRMessage *message)
{
    int ret = 0;

    /*recv buffer*/
    T_KRTransportConfig *transport_config = channel->channel_config->transport_config;
    ret = transport_config->func->recv(channel->transport, channel->buffer_recv);
    if (ret < 0) {
        KR_LOG(KR_LOGERROR, "transport recv error!");
        /*handle error*/
        transport_config->func->error(channel->transport);
        return ret;
    } else if (ret == 0) {
        KR_LOG(KR_LOGERROR, "transport recv timeout!");
        /*handle timeout*/
        transport_config->func->timeout(channel->transport);
        return ret;
    }


    /*parse buffer to message*/
    T_KRProtocolConfig *protocol_config = channel->channel_config->protocol_config;
    ret = protocol_config->parse_func(channel->protocol, channel->buffer_recv, message);
    if (ret > 0) {
        /*message unfinished*/

        return ret;
    } else if (ret < 0) {
        /*parse failed*/

        return ret;
    }

    return 0;
}


int kr_channel_send(T_KRChannel *channel, T_KRMessage *message)
{
    int ret = 0;

    /*dump message to buffer*/
    T_KRProtocolConfig *protocol_config = channel->channel_config->protocol_config;
    ret = protocol_config->dump_func(channel->protocol, message, channel->buffer_send);
    if (ret != 0) {
        /*dump failed*/

        return ret;
    } 

    /*send buffer*/
    T_KRTransportConfig *transport_config = channel->channel_config->transport_config;
    ret = transport_config->func->send(channel->transport, channel->buffer_send);
    if (ret < 0) {
        KR_LOG(KR_LOGERROR, "transport send error!");
        /*handle error*/
        transport_config->func->error(channel->transport);
        return ret;
    } else if (ret == 0) {
        KR_LOG(KR_LOGERROR, "transport send timeout!");
        /*handle timeout*/
        transport_config->func->timeout(channel->transport);
        return ret;
    }

    return 0;
}
