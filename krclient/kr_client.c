#include "krutils/kr_utils.h"
#include "krutils/kr_json.h"
#include "krserver/kr_server_message.h"
#include <time.h>
#include <errno.h>
#include <signal.h>

#define TCP_IP "127.0.0.1"
#define TCP_PORT 7251
#define CLIENT_ID "krclient1"
#define SERVER_ID "krserver1"

char caNetErr[KR_NET_ERR_LEN] = {0};

int kr_client_on(int fd)
{
    T_KRMessage stCliOn = {0};
    stCliOn.msgtype = KR_MSGTYPE_CLION;
    strcpy(stCliOn.msgid, CLIENT_ID);
    strcpy(stCliOn.clientid, CLIENT_ID);

    if (kr_message_write(caNetErr, fd, &stCliOn) <= 0) {
        fprintf(stderr, "kr_message_write clion error[%s]!\n", caNetErr);
        return -1;
    }

    return 0;
}

int kr_client_apply_struct(T_KRMessage *krmsg, int fd, int i)
{
    char buf[1024] = {0};
    typedef struct _kr_tradflow_t{
        char     caOutCustNo[20+1];
        char     caOutTransDate[8+1];
        char     caOutTransTime[6+1];
        char     caOutFlowNo[15+1];
        char     caOutTransType[2+1];
        double   dOutTransAmt;
        char     caOutTransLoc[100+1];
    } T_KRTradFlow;

    srandom((unsigned int)(time(NULL)+i));

    T_KRTradFlow *msgbody = kr_calloc(sizeof(T_KRTradFlow));
    memset(buf, 0x00, sizeof(buf));
    sprintf(buf, "%s%04d", "6223000000000000", (int )(random()%10));
    strcpy(krmsg->objectkey, buf); //set krmsg objectkey
    memcpy(msgbody->caOutCustNo, buf, sizeof(msgbody->caOutCustNo));

    memset(buf, 0x00, sizeof(buf));
    sprintf(buf, "20130219");
    memcpy(msgbody->caOutTransDate, buf, sizeof(msgbody->caOutTransDate));

    memset(buf, 0x00, sizeof(buf));
    sprintf(buf, "0919%02d", i%60);
    memcpy(msgbody->caOutTransTime, buf, sizeof(msgbody->caOutTransTime));

    memset(buf, 0x00, sizeof(buf));
    sprintf(buf, "%015d", i);
    strcpy(krmsg->msgid, buf); //set krmsg msgid
    memcpy(msgbody->caOutFlowNo, buf, sizeof(msgbody->caOutFlowNo));

    memset(buf, 0x00, sizeof(buf));
    sprintf(buf, "%02d", i%2);
    memcpy(msgbody->caOutTransType, buf, sizeof(msgbody->caOutTransType));

    msgbody->dOutTransAmt = random()%10000;

    memset(buf, 0x00, sizeof(buf));
    sprintf(buf, "TRANS_LOCATION:%02d", i);
    memcpy(msgbody->caOutTransLoc, buf, sizeof(msgbody->caOutTransLoc));

    krmsg->msglen = sizeof(*msgbody);
    memcpy(krmsg->msgbuf, msgbody, krmsg->msglen);
    if (kr_message_write(caNetErr, fd, krmsg) != krmsg->msglen) {
        fprintf(stderr, "kr_message_write apply error[%s]!\n", caNetErr);
        kr_free(krmsg->msgbuf);
        return -1;
    }

    return 0;
}


int kr_client_apply_json(T_KRMessage *krmsg, int fd, int i)
{
    char buf[1024] = {0};

    srandom((unsigned int)(time(NULL)+i));
    /*Write Apply message */
    cJSON *msgbody =cJSON_CreateObject();
    memset(buf, 0x00, sizeof(buf));
    sprintf(buf, "%s%04d", "6223000000000000", (int )(random()%10));
    strcpy(krmsg->objectkey, buf); //set krmsg objectkey
    cJSON_AddStringToObject(msgbody, "custno", buf);
    memset(buf, 0x00, sizeof(buf));
    sprintf(buf, "20130219");
    cJSON_AddStringToObject(msgbody, "txndate", buf);
    memset(buf, 0x00, sizeof(buf));
    sprintf(buf, "0919%02d", i%60);
    cJSON_AddStringToObject(msgbody, "txntime", buf);
    memset(buf, 0x00, sizeof(buf));
    sprintf(buf, "%015d", i);
    strcpy(krmsg->msgid, buf); //set krmsg msgid
    cJSON_AddStringToObject(msgbody, "flowno", buf);
    memset(buf, 0x00, sizeof(buf));
    sprintf(buf, "%02d", i%2);
    cJSON_AddStringToObject(msgbody, "txntype", buf);
    cJSON_AddNumberToObject(msgbody, "txnamt", i*9.8);
    memset(buf, 0x00, sizeof(buf));
    sprintf(buf, "TRANS_LOCATION:%02d", i);
    cJSON_AddStringToObject(msgbody, "txnloc", buf);

    char *jsonmsg = cJSON_PrintUnformatted(msgbody);
    krmsg->msglen = strlen(jsonmsg);
    memcpy(krmsg->msgbuf, jsonmsg, krmsg->msglen);
    kr_free(jsonmsg);
    cJSON_Delete(msgbody);
    if (kr_message_write(caNetErr, fd, krmsg) != krmsg->msglen) {
        fprintf(stderr, "kr_message_write apply error[%s]!\n", caNetErr);
        kr_free(krmsg->msgbuf);
        return -1;
    }

    return 0;
}

int kr_client_apply_weibo(T_KRMessage *krmsg, int fd, int i)
{
    strcpy(krmsg->objectkey, "1761437952"); //set krmsg objectkey
    snprintf(krmsg->msgid, sizeof(krmsg->msgid), "%08d", i); //set krmsg msgid
    strcpy(krmsg->msgbuf, "{\"reposts_count\": 0, \"truncated\": false, \"text\": \"haha \", \"visible\": {\"type\": 0, \"list_id\": 0}, \"in_reply_to_status_id\": \"\", \"created_at_time\": 1369899542.0, \"id\": 3583680788661581, \"mid\": \"3583680788661581\", \"source\": \"source\", \"attitudes_count\": 0, \"favorited\": false, \"user\": {\"bi_followers_count\": 110, \"domain\": \"\", \"avatar_large\": \"http://tp1.sinaimg.cn/1761437952/180/5602071810/0\", \"block_word\": 0, \"star\": 0, \"id\": 1761437952, \"city\": \"1000\", \"verified\": false, \"follow_me\": true, \"verified_reason\": \"\", \"followers_count\": 542, \"mbtype\": 0, \"statuses_count\": 3266, \"friends_count\": 273, \"online_status\": 0, \"mbrank\": 0, \"idstr\": \"1761437952\", \"geo_enabled\": false, \"name\": \"\u624b\u5fc3\u91cc\u7684\u5e78\u798f\u611f\", \"remark\": \"\", \"favourites_count\": 45, \"screen_name\": \"\u624b\u5fc3\u91cc\u7684\u5e78\u798f\u611f\", \"url\": \"\", \"gender\": \"f\", \"created_at\": \"Sun Jun 13 11:39:02 +0800 2010\", \"verified_type\": -1, \"following\": true}, \"geo\": null, \"created_at\": \"Thu May 30 15:39:02 +0800 2013\", \"comments_count\": 0}");
    krmsg->msglen = strlen(krmsg->msgbuf);
    printf("krclient send:%s %d\n", krmsg->msgbuf, krmsg->msglen);
    if (kr_message_write(caNetErr, fd, krmsg) != krmsg->msglen) {
        fprintf(stderr, "kr_message_write apply error[%s]!\n", caNetErr);
        kr_free(krmsg->msgbuf);
        return -1;
    }

    return 0;
}

int kr_client_apply(int fd, char fmt)
{
    int ret = 0;
    int i = 0;
    char readbuf[1024] = {0};
    size_t readlen = 0;
    T_KRMessage stApply = {0};
    stApply.msgtype = KR_MSGTYPE_APPLY;
    strcpy(stApply.serverid, SERVER_ID);
    strcpy(stApply.clientid, CLIENT_ID);
    stApply.datasrc = 2;
    for (i=0; i<22; ++i) {
        if (fmt == 'J') {
            kr_client_apply_json(&stApply, fd, i);
        } else if (fmt == 'W') {
            kr_client_apply_weibo(&stApply, fd, i);
        } else {
            kr_client_apply_struct(&stApply, fd, i);
        }

printf("Send:fd:[%d], msgid[%s], msgtype[%d], serverid[%s], clientid[%s], "
                "datasrc[%d], objectkey[%s], msglen[%d]\n", \
            fd, stApply.msgid, stApply.msgtype, 
            stApply.serverid, stApply.clientid, 
            stApply.datasrc, stApply.objectkey, stApply.msglen);
/*        
        readlen = read(fd, &readbuf, sizeof(readbuf));
        if( readlen < 0) {
            fprintf(stderr, "read fd failed[%s]!\n", strerror(errno));
            return -1;
        }
        printf("read jsonstr:[%s]\n", readbuf);
*/

        if(i%1000 == 0) {
            time_t tTime = time(NULL);
            struct tm *ptmNow = localtime(&tTime);
            char caTimeString[80] = {0};
            strftime(caTimeString, sizeof(caTimeString), "%c", ptmNow);
            printf("client sending [%d] records at [%s]!\n", i, caTimeString);
        }
    }

    return 0;
}


int main(int argc, char *argv[])
{  
    int ch;
    char ip[32];
    int port;
    int tcpfd;
    char caNetErr[1024];
    char msgfmt;

    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    msgfmt = 'C';
    strcpy(ip, TCP_IP);
    port = TCP_PORT;
    while ((ch = getopt(argc, argv, "i:p:f:")) != -1)
    {
        switch (ch)
        {
            case 'i':
                strcpy(ip, optarg);
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'f':
                msgfmt = optarg[0];
                break;
            default:
                break;
        }
    }
    
    /*connect to coordi/server*/
    tcpfd = kr_net_tcp_connect(caNetErr, ip, port);
    if (tcpfd <= 0) {
        fprintf(stderr, "kr_net_tcp_connect [%s]:[%d] failed[%s]\n", 
                TCP_IP, TCP_PORT, caNetErr);
        return -1;
    }
    
    /*client on*/    
    if (kr_client_on(tcpfd) !=0) {
        return -1;
    }

    /*client apply*/    
    if (kr_client_apply(tcpfd, msgfmt) !=0) {
        return -1;
    }
    
    close(tcpfd);
    return 0;
}

