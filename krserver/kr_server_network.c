#include "kr_server.h"
#include <errno.h>


static T_KRClient *kr_client_alloc(int fd, T_KRServer *krserver)
{
    T_KRClient *krclient = kr_calloc(sizeof(*krclient));
    if (krclient == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krclient failed!");
        return NULL;
    }
    krclient->fd = fd;
    krclient->krserver = krserver;

    return krclient;
}


static void kr_client_free(T_KRClient *krclient)
{
    T_KRServer *krserver = krclient->krserver;
    if (krclient) {
        kr_list_remove(krserver->clients, krclient);
        kr_event_file_delete(krserver->krel, krclient->fd, KR_EVENT_READABLE);
        close(krclient->fd);
        kr_free(krclient);
    }
}


static int kr_client_match(T_KRClient *node, T_KRClient *key)
{
    return node->fd == key->fd;
}


static void kr_client_reset(T_KRClient *krclient)
{

}



void kr_server_write_resp(T_KRMessage *inmsg, T_KRMessage *outmsg, void *data)
{
    T_KRClient *krclient = (T_KRClient *)data;

    /* dump out message */
    T_KRBuffer outbuf = {0};
    kr_message_dump(outmsg, &outbuf);

    /* write out buffer */
    kr_net_write(krclient->fd, outbuf.data, outbuf.size);

    /* free request&response message */
    kr_message_free(inmsg);
    kr_message_free(outmsg);
}

static int 
kr_server_process_input_message(T_KRMessage *inmsg, T_KRClient *krclient)
{
    /** store apply message */
    T_KRMessage *apply = inmsg;

    /** alloc reply message */
    T_KRMessage *reply = kr_message_alloc();

    /** invoke krengine */
    T_KREngineArg stArg = {apply, reply, kr_server_write_resp, krclient};
    return kr_engine_run(krclient->krserver->krengine, &stArg);
}

static void 
kr_server_process_input_buffer(T_KRBuffer *krbuf, T_KRClient *krclient)
{
    int ret = 0;
    while(krbuf->size) {
        /** alloc inmsg */
        if (krclient->inmsg == NULL) {
            KR_LOG(KR_LOGDEBUG, "alloc new inmsg! %d", krbuf->size);
            krclient->inmsg = kr_message_alloc();
        }

        /** parse request message */
        ret = kr_message_parse(&krclient->inbuf, krclient->inmsg);
        if (ret < 0) {
            KR_LOG(KR_LOGERROR, "kr_message_parse error:%d", ret);
            kr_client_free(krclient);
            return;
        } else if (ret > 0) {
            KR_LOG(KR_LOGDEBUG, "kr_message_parse need data!");
            return;
        } else if (ret == 0) {
            kr_server_process_input_message(krclient->inmsg, krclient);
            krclient->inmsg = NULL;
        }
    }
}

static void 
kr_server_on_read(T_KREventLoop *el, int fd, void *privdata, int mask) 
{
    T_KRClient *krclient = (T_KRClient *)privdata;
    T_KRServer *krserver = krclient->krserver;
    T_KRBuffer *krbuf = &krclient->inbuf;
    int ret, nread, readlen;

    readlen = KR_BUFFER_MAX_LEN-krbuf->size;
    nread = read(fd, krbuf->data+krbuf->size, readlen);
    if (nread == -1) {
        if (errno == EAGAIN) {
            nread = 0;
        } else {
            KR_LOG(KR_LOGERROR, "Read client error: %s", strerror(errno));
            kr_client_free(krclient);
            return;
        }
    } else if (nread == 0) {
        KR_LOG(KR_LOGDEBUG, "Client closed connection");
        kr_client_free(krclient);
        return;
    }

    /*read something, process with it */
    if (nread) {
        krbuf->size += nread;
        kr_server_process_input_buffer(krbuf, krclient);
    }

}


static void 
kr_server_on_connect(T_KREventLoop *krel, int fd, void *privdata, int mask) 
{
    T_KRServer *krserver = (T_KRServer *)privdata;
    char cip[128];
    int cport;
    int cfd;

    /*accept connection*/
    cfd = kr_net_tcp_accept(krserver->neterr, fd, cip, &cport);
    if (cfd == KR_NET_ERR) {
        KR_LOG(KR_LOGERROR, "Accepting client failed:%s", krserver->neterr);
        return;
    }
    KR_LOG(KR_LOGDEBUG, "Accepted %s:%d", cip, cport);

    /*create client of this connection*/
    if (cfd != -1) {
        T_KRClient *krclient = kr_client_alloc(cfd, krserver);
        if (krclient == NULL) {
            KR_LOG(KR_LOGERROR, "kr_client_alloc %d failed!", cfd);
            return;
        }

        kr_net_nonblock(NULL, cfd);
        kr_net_tcp_nodelay(NULL, cfd);
        if (kr_event_file_create(krserver->krel, cfd, KR_EVENT_READABLE,
            kr_server_on_read, krclient) == KR_NET_ERR)
        {
            KR_LOG(KR_LOGERROR, "kr_event_file_create error!");
            kr_client_free(krclient);
            return;
        }

        /*add this to server's client list*/
        kr_list_add_tail(krserver->clients, krclient);
    }
}


void kr_server_network_shutdown(T_KRServer *krserver)
{
    /* delete accept event and close listening ipfd */
    kr_event_file_delete(krserver->krel, krserver->ipfd, KR_EVENT_READABLE);
    close(krserver->ipfd);

    /* free all clients */
    kr_list_destroy(krserver->clients);
}


int kr_server_network_startup(T_KRServer *krserver)
{
    int ret=0;

    /* Becoming a server */
    if (krserver->config->tcpport != 0) {
        krserver->ipfd = kr_net_tcp_server(krserver->neterr, \
            krserver->config->tcpport, krserver->config->tcpbindaddr);
        if (krserver->ipfd == KR_NET_ERR) {
            KR_LOG(KR_LOGERROR, "kr_net_tcp_server[%s:%d] failed[%s]",
                    krserver->config->tcpbindaddr, 
                    krserver->config->tcpport, 
                    krserver->neterr);
            return -1;
        }
    }
    if (krserver->ipfd < 0) {
        KR_LOG(KR_LOGERROR, "tcpport [%d] uncorrect!", \
                krserver->config->tcpport);
        return -1;
    }
    
    /* Register tcp accept file event */
    ret = kr_event_file_create(krserver->krel, krserver->ipfd, 
            KR_EVENT_READABLE,  kr_server_on_connect, krserver);
    if (ret == KR_NET_ERR) {
        KR_LOG(KR_LOGERROR, "kr_event_file_create tcp error");
        return -1;
    }

    /*create client list*/
    krserver->clients = kr_list_new();
    kr_list_set_match(krserver->clients, kr_client_match);
    
    return 0;
}

