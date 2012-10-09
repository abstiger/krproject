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
    
    T_KRMessage stMessage = {0};
    stMessage.msgtype = KR_MSGTYPE_SVRON;
    strcpy(stMessage.serverid, krserver.serverid);
    stMessage.msglen = sizeof(T_KRMsgSvrOn);
    stMessage.msgbuf = &stMsgSvrOn;
    
    int writeLen = kr_message_write(krserver.neterr, krserver.cofd, &stMessage);
	if (writeLen <= 0) {/* write message failure */	    
        KR_LOG(KR_LOGERROR, "write message error[%s]!", krserver.neterr);
        return -1;
    }
    
    return 0;
}

int kr_server_handle_apply(T_KRMessage *ptMessage)
{
    int iResult = 0;
    E_KROprCode eOprCode = KR_OPRCODE_DETECT;
    
	/* judge whether this server detecting this message */
	if (strcmp(ptMessage->serverid, krserver.serverid) != 0) {
        KR_LOG(KR_LOGDEBUG, "message:[%s] server[%s] not match!", \
            ptMessage->serverid, krserver.serverid);
        eOprCode = KR_OPRCODE_INSERT;
    }

    /* run rule detecting */
    iResult = kr_engine_run(krserver.krengine, eOprCode, \
            ptMessage->datasrc, ptMessage->msgbuf);
    if (iResult != 0) {
        KR_LOG(KR_LOGERROR, "kr_engine_run:[%d] [%d] [%s] failed!", \
            eOprCode, ptMessage->datasrc, ptMessage->msgbuf);
        return -1; 
    }

KR_LOG(KR_LOGDEBUG, "kr_server_handle_apply [%d] Passed!", ptMessage->datasrc);
	
    return 0;
}


static int kr_server_handle_message(T_KRMessage *ptMessage)
{
    int ret = -1;
    
    switch(ptMessage->msgtype)
	{
	    case KR_MSGTYPE_APPLY:
	        ret = kr_server_handle_apply(ptMessage);
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported message type[%d]!", \
                ptMessage->msgtype);
            break;
	}
	
	return ret;
}

void kr_server_message_read_handler(T_KREventLoop *el, int fd, void *privdata, int mask) 
{
    int ret = 0;
    int readLen = 0;
    T_KRMessage stMessage = {0};

	/** read message */
	readLen = kr_message_read(krserver.neterr, fd, &stMessage);
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
    
printf("Read message:msgtype[%d], serverid[%s], clientid[%s], msglen[%d], msgbuf:[%s]\n", \
        stMessage.msgtype, stMessage.serverid, stMessage.clientid, stMessage.msglen, (char *)stMessage.msgbuf);
        
	/** handle message */
	ret = kr_server_handle_message(&stMessage);
	if (ret != 0) {/* handle message failure */
        KR_LOG(KR_LOGERROR, "handle message [%d], [%s]error!", \
               stMessage.msgtype, stMessage.msgbuf);
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
        KR_LOG(KR_LOGERROR, "Accepting client connection: %s", krserver.neterr);
        return;
    }
    KR_LOG(KR_LOGDEBUG, "Accepted %s:%d", cip, cport);
    kr_server_common_accept_handler(cfd);
}


void kr_server_unix_accept_handler(T_KREventLoop *el, int fd, void *privdata, int mask) 
{
    int cfd;

    cfd = kr_net_unix_accept(krserver.neterr, fd);
    if (cfd == KR_NET_ERR) {
        KR_LOG(KR_LOGERROR, "Accepting client connection: %s", krserver.neterr);
        return;
    }
    KR_LOG(KR_LOGDEBUG, "Accepted connection to %s", krserver.unixdomain);
    kr_server_common_accept_handler(cfd);
}



