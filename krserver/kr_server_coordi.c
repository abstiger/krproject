#include "kr_server.h"

/*

static int kr_server_connect_to_cluster(void);


static int kr_server_write_svron(void)
{
    //Write Server On message 
    T_KRMessage *svron = kr_message_alloc();
    if (svron == NULL) {
        KR_LOG(KR_LOGERROR, "kr_message_alloc svron error!");
        return -1;
    }

    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "weights", krserver->weights);
    cJSON_AddNumberToObject(json, "replica", krserver->replica);
    //svron->msgtype = KR_MSGTYPE_SVRON;
    //strcpy(svron->serverid, krserver->serverid);
    svron->msgbuf = cJSON_PrintUnformatted(json);
    svron->msglen = strlen(svron->msgbuf);
    cJSON_Delete(json);

    if (kr_message_write(krserver->cofd, svron) <= 0) {
        KR_LOG(KR_LOGERROR, "write svron message error!");
        kr_message_free(svron);
        return -1;
    }

    kr_message_free(svron);
    return 0;
}


static int kr_server_try_connect_to_cluster(void)
{
    if (krserver->coordport != 0) {
        krserver->cofd = kr_net_tcp_connect(krserver->neterr, \
            krserver->coordip, krserver->coordport);
        if (krserver->cofd == KR_NET_ERR) {
            KR_LOG(KR_LOGERROR, "kr_net_tcp_connect[%s][%d] failed[%s]",
                krserver->coordip, krserver->coordport, krserver->neterr);
            return -1;
        }
    }
    
    if (krserver->cofd < 0) {
        KR_LOG(KR_LOGERROR, "Can't connect to coordinator in cluster!");
        return -1;
    }
    
    // Register read file event 
    if (kr_event_file_create(krserver->krel, krserver->cofd, \
                KR_EVENT_READABLE, kr_server_read_message, krserver) 
                == KR_NET_ERR) {
        KR_LOG(KR_LOGERROR, "kr_event_file_create failed[%s]", 
                krserver->neterr);
        return -1;
    }

    // write svron message to the cluster 
    if (kr_server_write_svron() != 0) {
        KR_LOG(KR_LOGERROR, "kr_server_write_svron failed!");
        return -1;
    }
    
    return 0;
}


static int kr_server_connect_to_cluster(void)
{
    for (int i=0; i<krserver->retrytimes ; ++i) {
        // try to connect to the cluster
        if (kr_server_try_connect_to_cluster() == 0) {
            return 0;
        }
        
        // sleep a while then go next try
        sleep(krserver->retryinterval);
    }
    
    return -1;
}

*/
