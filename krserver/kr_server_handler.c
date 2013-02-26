#include "kr_server.h"
#include <fcntl.h>
#include <errno.h>

extern T_KRServer krserver;

int kr_server_handle_svron(void)
{
    /*Write Server On message */
    T_KRMsgSvrOn stMsgSvrOn = {0};
    stMsgSvrOn.weights = krserver.weights;
    stMsgSvrOn.replica = krserver.replica;
    
    T_KRMessage stMsg = {0};
    stMsg.msgtype = KR_MSGTYPE_SVRON;
    strcpy(stMsg.serverid, krserver.serverid);
    stMsg.msglen = sizeof(T_KRMsgSvrOn);
    stMsg.msgbuf = &stMsgSvrOn;
    
    int writeLen = kr_message_write(krserver.neterr, krserver.cofd, &stMsg);
    if (writeLen <= 0) {/* write message failure */        
        KR_LOG(KR_LOGERROR, "write message error[%s]!", krserver.neterr);
        return -1;
    }
    
    return 0;
}

int kr_server_handle_apply(T_KRMessage *krmsg)
{
    int ret = 0;
    E_KROprCode eOprCode = KR_OPRCODE_DETECT;
    
    /* judge whether this server detecting this message */
    if (strcmp(krmsg->serverid, krserver.serverid) != 0) {
        KR_LOG(KR_LOGDEBUG, "message:[%s] server[%s] not match!", \
            krmsg->serverid, krserver.serverid);
        eOprCode = KR_OPRCODE_INSERT;
    }

    /* run engine */
    ret = kr_engine_run(krserver.krengine, krmsg->fd, \
            eOprCode, krmsg->datasrc, krmsg->msgbuf);
    if (ret != 0) {
        KR_LOG(KR_LOGERROR, "kr_engine_run:[%d] [%d] [%s] failed!", \
            eOprCode, krmsg->datasrc, krmsg->msgid);
        return -1; 
    }

    return 0;
}


static int kr_server_handle_message(T_KRMessage *krmsg)
{
    int ret = 0;
    
    switch(krmsg->msgtype)
    {
        case KR_MSGTYPE_APPLY:
            KR_LOG(KR_LOGDEBUG, "Client[%s] Server[%s] msgid[%s]!", 
                    krmsg->clientid, krmsg->serverid, krmsg->msgid);
            ret = kr_server_handle_apply(krmsg);
            break;
        case KR_MSGTYPE_CLION:
            KR_LOG(KR_LOGDEBUG, "Client[%s] connected!", krmsg->clientid);
            break;
        case KR_MSGTYPE_CLIOFF:
            KR_LOG(KR_LOGDEBUG, "Client[%s] disconnected!", krmsg->clientid);
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported msgtype[%d]!", krmsg->msgtype);
            ret = -1;
            break;
    }
    
    return ret;
}

void kr_server_message_read_handler(T_KREventLoop *el, int fd, void *privdata, int mask) 
{
    int ret = 0;
    int readLen = 0;
    T_KRMessage stMsg = {0};

    /** read message */
    readLen = kr_message_read(krserver.neterr, fd, &stMsg);
    if (readLen <= 0) {/* read message failure */
        KR_LOG(KR_LOGERROR, "read message error[%s]!", krserver.neterr);
        kr_event_file_delete(el, fd, KR_EVENT_READABLE);
        close(fd);
        
        /* reconnect to the cluster */
        if (fd == krserver.cofd) {
            if (kr_server_connect_to_cluster() != 0) {
                KR_LOG(KR_LOGERROR, "kr_server_connect_to_cluster failed");
            }
        }
        return;
    }
    
    /** handle message */
    ret = kr_server_handle_message(&stMsg);
    if (ret != 0) {/* handle message failure */
        KR_LOG(KR_LOGERROR, "handle message [%d], [%s]error!", \
               stMsg.msgid, stMsg.msgbuf);
        return;
    }
}

static void kr_server_common_accept_handler(int fd) 
{
    char ip[32];
    int port;
    
    kr_net_peer_to_string(fd, ip, &port);
    printf("kr_net_peer_to_string: fd:%d ip:%s port:%d\n", fd, ip, port);
    
    if (fd != -1) {
        kr_net_nonblock(NULL,fd);
        kr_net_tcp_nodelay(NULL,fd);
        if (kr_event_file_create(krserver.krel, fd, KR_EVENT_READABLE,
            kr_server_message_read_handler, NULL) == KR_NET_ERR)
        {
            KR_LOG(KR_LOGERROR, "kr_event_file_create error!");
            close(fd);
            return;
        }
    }
    
    //server.stat_numconnections++;
}


void kr_server_tcp_accept_handler(T_KREventLoop *el, int fd, void *privdata, int mask) 
{
    int cport, cfd;
    char cip[128];

    cfd = kr_net_tcp_accept(krserver.neterr, fd, cip, &cport);
    if (cfd == KR_NET_ERR) {
        KR_LOG(KR_LOGERROR, "Accepting client failed:%s", krserver.neterr);
        return;
    }
    KR_LOG(KR_LOGDEBUG, "Accepted %s:%d", cip, cport);
    kr_server_common_accept_handler(cfd);
}

