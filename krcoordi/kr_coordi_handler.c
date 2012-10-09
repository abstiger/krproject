#include "kr_coordi.h"
#include <fcntl.h>
#include <errno.h>

extern T_KRCoordi krcoordi;

int kr_coordi_handle_svron(T_KREventLoop *el, int fd, T_KRMessage *ptMessage)
{
    KR_LOG(KR_LOGDEBUG, "Server [%s] Online [%d]!", ptMessage->serverid, fd);
    
    char ip[32];
    int port;
    kr_net_peer_to_string(fd, ip, &port);
    
    T_KRMsgSvrOn *ptMsgSvrOn = (T_KRMsgSvrOn *)ptMessage->msgbuf;

    /* Check this server whether exists */
    T_KRActualNode *node = \
        kr_conhash_lookup(krcoordi.servers, ptMessage->serverid);
    if (node != NULL) {
        KR_LOG(KR_LOGDEBUG, "Server %s already exists!", ptMessage->serverid);
        return 0;
    }
    
    /* allocate connector for this server */
    T_KRCoordiConnector *ptServer = kr_calloc(sizeof(T_KRCoordiConnector));
    if (ptServer == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptServer error!");
        return -1;
    }
    ptServer->fd = fd;
    ptServer->role = KR_COORDI_SERVER;
    strcpy(ptServer->id, ptMessage->serverid);
    strcpy(ptServer->ip, ip);
    ptServer->aplnum = 0;
    ptServer->rplnum = 0;
    ptServer->errnum = 0;
    ptServer->weights = ptMsgSvrOn->weights;
    ptServer->replica = ptMsgSvrOn->replica;
    
    /* Manage this server with consistent hashing */
    kr_conhash_add(krcoordi.servers, ptServer->id, ptServer->weights, ptServer);
    
    /*add this server fd--id mapping to fdtable*/
    kr_hashtable_replace(krcoordi.fdtable, &ptServer->fd, ptServer);
    
    return 0;
}


int kr_coordi_handle_svroff(T_KREventLoop *el, int fd, T_KRMessage *ptMessage)
{
    KR_LOG(KR_LOGDEBUG, "Server [%s] Offline [%d]!", ptMessage->serverid, fd);
    
    /* Check this server whether exists */
    T_KRActualNode *node = \
        kr_conhash_lookup(krcoordi.servers, ptMessage->serverid);
    if (node == NULL) {
        KR_LOG(KR_LOGDEBUG, "Server %s not exists!", ptMessage->serverid);
        return 0;
    }
    
    /* remove from fdtable */
    kr_hashtable_remove(krcoordi.fdtable, &fd);
    
    /* remove from conhash */
    T_KRCoordiConnector *ptServer = (T_KRCoordiConnector *)node->priv;
    kr_conhash_remove(krcoordi.servers, ptServer->id);

    /* Free this server */
    kr_event_file_delete(el, fd, KR_EVENT_READABLE);
    close(fd);
    
    kr_free(ptServer);
    
    return 0;
}


int kr_coordi_handle_clion(T_KREventLoop *el, int fd, T_KRMessage *ptMessage)
{
    KR_LOG(KR_LOGDEBUG, "Client [%s] Online [%d]!", ptMessage->clientid, fd);

    char ip[32];
    int port;
    kr_net_peer_to_string(fd, ip, &port);
    
    /* Check this client whether exists */
    T_KRListNode *node = kr_list_search(krcoordi.clients, ptMessage->clientid);
    if (node != NULL) {
        KR_LOG(KR_LOGDEBUG, "Client %s already exists!", ptMessage->clientid);
        return 0;
    }

    /* allocate connector for this server */
    T_KRCoordiConnector *ptClient = kr_calloc(sizeof(T_KRCoordiConnector));
    if (ptClient == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptClient error!");
        return -1;
    }
    ptClient->fd = fd;
    ptClient->role = KR_COORDI_CLIENT;
    strcpy(ptClient->id, ptMessage->clientid);
    strcpy(ptClient->ip, ip);
    ptClient->aplnum = 0;
    ptClient->rplnum = 0;
    ptClient->errnum = 0;
    
    /* Manage this client with list */
    kr_list_add_tail(krcoordi.clients, ptClient);
    
    /*add this client fd--id mapping to fdtable*/
    kr_hashtable_replace(krcoordi.fdtable, &ptClient->fd, ptClient);
    
    return 0;
}


int kr_coordi_handle_clioff(T_KREventLoop *el, int fd, T_KRMessage *ptMessage)
{
    KR_LOG(KR_LOGDEBUG, "Client [%s] Offline [%d]!", ptMessage->clientid, fd);

    /* Check this client whether exists */
    T_KRListNode *node = kr_list_search(krcoordi.clients, ptMessage->clientid);
    if (node == NULL) {
        KR_LOG(KR_LOGDEBUG, "Client %s not exists!", ptMessage->clientid);
        return 0;
    }
   
    /*remove from list*/
    T_KRCoordiConnector *ptClient = (T_KRCoordiConnector *)node->value;
    kr_list_remove(krcoordi.clients, ptClient->id);

    /* remove from fdtable */
    kr_hashtable_remove(krcoordi.fdtable, &fd);
    
    /* Free this client */
    kr_event_file_delete(el, fd, KR_EVENT_READABLE);
    close(fd);
    
    kr_free(ptClient);
    
    return 0;
}


static void 
kr_coordi_send_to_server(T_KRActualNode *ptActualNode, T_KRMessage *ptMessage)
{
    T_KRCoordiConnector *ptServer = ptActualNode->priv;

    /* sent to the specified server or a replica server */
    if (ptServer->replica || !strcmp(ptServer->id, ptMessage->serverid)) {
        int nLen = kr_message_write(ptServer->neterr, ptServer->fd, ptMessage);
	    if (nLen <= 0) {/* write message failure */	    
            KR_LOG(KR_LOGERROR, "write message error[%s]!", ptServer->neterr);
            ++ptServer->errnum;
        }
    }
}


int kr_coordi_handle_apply(T_KREventLoop *el, int fd, T_KRMessage *ptMessage)
{
    KR_LOG(KR_LOGDEBUG, "Client [%s] Send Apply To Server [%s]!", \
        ptMessage->clientid, ptMessage->serverid);
    
    /* Check this client whether allowed */
    T_KRListNode *node = kr_list_search(krcoordi.clients, ptMessage->clientid);
    if (node == NULL) {
        KR_LOG(KR_LOGERROR, "Client %s not allowed!", ptMessage->clientid);
        return -1;
    }
    T_KRCoordiConnector *ptClient = (T_KRCoordiConnector *)node->value;
    ++ptClient->aplnum;

    T_KRActualNode *server = NULL;
    /*if apply message specified the destination server*/
    if (ptMessage->serverid[0] != '\0') {
        /* Check this server whether exists */
        server = kr_conhash_lookup(krcoordi.servers, ptMessage->serverid);
        if (server == NULL) {
            KR_LOG(KR_LOGDEBUG, "Specified Server [%s] doesn't exists!", \
                ptMessage->serverid);
        }
    }
    
    /*choose server according to the key*/
    if (server == NULL) {
        server = kr_conhash_locate(krcoordi.servers, ptMessage->objectkey);
        if (server == NULL) {
            KR_LOG(KR_LOGERROR, "kr_conhash_locate %s failed!", \
               ptMessage->objectkey);
            return -1;   
        }
    }
    
    /* padding serverid field */
    T_KRCoordiConnector *ptServer = (T_KRCoordiConnector *)server->priv;
    strcpy(ptMessage->serverid, ptServer->id);
    ++ptServer->aplnum;
    
    KR_LOG(KR_LOGDEBUG, "Located Server [%s] with Consistent hashing [%s]!", \
                ptMessage->serverid, ptMessage->objectkey);
    
    /* send to the one who's the specified or need replication */
    kr_conhash_foreach_node(krcoordi.servers, \
        (KRForEachFunc )kr_coordi_send_to_server, ptMessage);

    return 0;
}


int kr_coordi_handle_reply(T_KREventLoop *el, int fd, T_KRMessage *ptMessage)
{
    KR_LOG(KR_LOGDEBUG, "Client [%s] Got Reply From Server [%s]!", \
        ptMessage->clientid, ptMessage->serverid);
        
    /* Check this server whether allowed */
    T_KRActualNode *server = NULL;
    server = kr_conhash_lookup(krcoordi.servers, ptMessage->serverid);
    if (server == NULL) {
        KR_LOG(KR_LOGERROR, "Server %s not allowed!", ptMessage->serverid);
        return -1;
    }
    T_KRCoordiConnector *ptServer = (T_KRCoordiConnector *)server->priv;
    ++ptServer->rplnum;
        
    /* Check this client whether exists */
    T_KRListNode *node = kr_list_search(krcoordi.clients, ptMessage->clientid);
    if (node == NULL) {
        KR_LOG(KR_LOGERROR, "Client %s not exists!", ptMessage->clientid);
        return -1;
    }
    T_KRCoordiConnector *ptClient = (T_KRCoordiConnector *)node->value;
    
    /*send reply to client*/
    int writeLen = kr_message_write(krcoordi.neterr, ptClient->fd, ptMessage);
	if (writeLen <= 0) {/* write message failure */	    
        KR_LOG(KR_LOGERROR, "write message error[%s]!", krcoordi.neterr);
        strcpy(ptClient->neterr, krcoordi.neterr);
        ++ptClient->errnum;
        return -1;
    }
    
    ++ptClient->rplnum;
    return 0;
}


static int 
kr_coordi_handle_message(T_KREventLoop *el, int fd, T_KRMessage *ptMessage)
{
    int ret = -1;

printf("kr_coordi_handle_message:msgtype[%d], serverid[%s], clientid[%s], msglen[%d], msgbuf:[%s]\n", \
        ptMessage->msgtype, ptMessage->serverid, ptMessage->clientid, \
        ptMessage->msglen, (char *)ptMessage->msgbuf);
    
    switch(ptMessage->msgtype)
	{
	    case KR_MSGTYPE_SVRON:
	        ret = kr_coordi_handle_svron(el, fd, ptMessage);
	        break;
	    case KR_MSGTYPE_SVROFF:
	        ret = kr_coordi_handle_svroff(el, fd, ptMessage);
	        break;
	    case KR_MSGTYPE_CLION:
	        ret = kr_coordi_handle_clion(el, fd, ptMessage);
	        break;
	    case KR_MSGTYPE_CLIOFF:
	        ret = kr_coordi_handle_clioff(el, fd, ptMessage);
	        break;
	    case KR_MSGTYPE_APPLY:
	        ret = kr_coordi_handle_apply(el, fd, ptMessage);
            break;
        case KR_MSGTYPE_REPLY:
            ret = kr_coordi_handle_reply(el, fd, ptMessage);
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported message type[%d]!", \
                ptMessage->msgtype);
            break;
	}
	
	return ret;
}


static void 
kr_coordi_read_handler(T_KREventLoop *el, int fd, void *priv, int mask) 
{
    int ret = 0;
    int readLen = 0;
    T_KRMessage stMessage = {0};

	/** read message */
	readLen = kr_message_read(krcoordi.neterr, fd, &stMessage);
	if (readLen <= 0) {
	    /* read message failure */
        KR_LOG(KR_LOGERROR, "read message error[%s]!", krcoordi.neterr);
        T_KRCoordiConnector *ptConn = NULL;
        
        /* find server or client with fd, then free it */
        ptConn = kr_hashtable_lookup(krcoordi.fdtable, &fd);
        if (ptConn == NULL) {
            kr_event_file_delete(el, fd, KR_EVENT_READABLE);
            close(fd);
            return;
        }
        
        /* handle it as connector offline */
        if (ptConn->role == KR_COORDI_SERVER) {
            stMessage.msgtype = KR_MSGTYPE_SVROFF;
            strcpy(stMessage.serverid, ptConn->id);
        } else {
            stMessage.msgtype = KR_MSGTYPE_CLIOFF;
            strcpy(stMessage.clientid, ptConn->id);
        }
    }
            
	/** handle message */
	ret = kr_coordi_handle_message(el, fd, &stMessage);
	if (ret != 0) {/* handle message failure */
        KR_LOG(KR_LOGERROR, "handle message [%d], [%s]error!", \
               stMessage.msgtype, stMessage.msgbuf);
        return;
    }
}


static void kr_coordi_common_accept_handler(int fd) 
{
    char ip[32];
    int port;
    
    kr_net_peer_to_string(fd, ip, &port);
    printf("Connection From: fd:%d ip:%s port:%d\n", fd, ip, port);
    
    if (fd != -1) {
        kr_net_nonblock(NULL,fd);
        kr_net_tcp_nodelay(NULL,fd);
        if (kr_event_file_create(krcoordi.el, fd, KR_EVENT_READABLE,
            kr_coordi_read_handler, NULL) == KR_NET_ERR)
        {
            KR_LOG(KR_LOGERROR, "kr_event_file_create error!");
            close(fd);
            return;
        }
    }
}


void 
kr_coordi_tcp_accept_handler(T_KREventLoop *el, int fd, void *priv, int mask) 
{
    int cport, cfd;
    char cip[128];

    cfd = kr_net_tcp_accept(krcoordi.neterr, fd, cip, &cport);
    if (cfd == KR_NET_ERR) {
        KR_LOG(KR_LOGERROR, "Accepting client connection: %s", krcoordi.neterr);
        return;
    }
    KR_LOG(KR_LOGDEBUG, "Accepted tcp/ip connection %s:%d", cip, cport);
    kr_coordi_common_accept_handler(cfd);
}


void 
kr_coordi_unix_accept_handler(T_KREventLoop *el, int fd, void *priv, int mask) 
{
    int cfd;

    cfd = kr_net_unix_accept(krcoordi.neterr, fd);
    if (cfd == KR_NET_ERR) {
        KR_LOG(KR_LOGERROR, "Accepting client connection: %s", krcoordi.neterr);
        return;
    }
    KR_LOG(KR_LOGDEBUG, "Accepted domain connection %s", krcoordi.unixdomain);
    kr_coordi_common_accept_handler(cfd);
}



