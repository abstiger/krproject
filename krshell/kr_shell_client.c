#include "kr_shell_client.h"
#include "krutils/kr_utils.h"
#include "krutils/kr_net.h"
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>


static T_KRMessage *kr_client_read_message(int fd)
{
    int readLen = 0;
    T_KRBuffer krbuf = {0};
    size_t msglen;

    /*alloc message*/
    T_KRMessage *krmsg = kr_message_alloc();
    if (krmsg == NULL) {
        fprintf(stderr, "kr_message_alloc failed!\n");
        return NULL;
    }

    /* read message head */
    readLen = kr_net_read(fd, krbuf.data, KR_MSGHEADER_LEN);
    if (readLen <= 0) {
        fprintf(stderr, "kr_net_read msghead failed!\n");
        kr_message_free(krmsg);
        return NULL;
    }
    krbuf.size += readLen;

    /* get message length */
    msglen = kr_get_u32(krbuf.data+KR_MSGHEADER_LEN-KR_MSGLEN_LEN);
    if (msglen < 0) {
        fprintf(stderr, "kr_get_u32 msglen failed!\n");
        kr_message_free(krmsg);
        return NULL;
    }

    /* read message body */
    readLen = kr_net_read(fd, krbuf.data+krbuf.size, msglen);
    if (readLen != msglen) {
        fprintf(stderr, "kr_net_read msgbody failed!\n");
        kr_message_free(krmsg);
        return NULL;
    }
    krbuf.size += readLen;

    /* parse message buffer */
    kr_message_parse(&krbuf, krmsg);

    return krmsg;
}


static int kr_client_write_message(int fd, T_KRMessage *krmsg)
{
    int writeLen = 0;
    T_KRBuffer krbuf = {0};

    /* dump message */
    if (kr_message_dump(krmsg, &krbuf) <= 0) {
        fprintf(stderr, "kr_message_dump failed!\n");
        return -1;
    }

    /* write message */
    writeLen = kr_net_write(fd, krbuf.data, krbuf.size);
    if (writeLen < 0) {
        fprintf(stderr, "kr_net_write failed! %d\n", writeLen);
        return -1;
    } else if (writeLen == 0) {
        return 0;
    }

    return writeLen;
}


void kr_client_disconnect(T_KRClient *krclient)
{
    if (krclient) {
        if (krclient->id) kr_free(krclient->id);
        if (krclient->ip) kr_free(krclient->ip);
        if (krclient->fd) {
            close(krclient->fd);
            shutdown(krclient->fd, SHUT_RDWR);
        }
        kr_free(krclient);
    }
}

int kr_client_set_timeout(T_KRClient *krclient, int secs)
{
    struct timeval timeout = {secs, 0};

    if (setsockopt(krclient->fd, SOL_SOCKET, SO_RCVTIMEO, 
                (char *)&timeout, sizeof(struct timeval)) != 0) {
        fprintf(stderr, "setsockopt SO_RCVTIMEO failed![%s]", strerror(errno));
        return -1;
    }

    if (setsockopt(krclient->fd, SOL_SOCKET, SO_SNDTIMEO, 
                (char *)&timeout, sizeof(struct timeval)) != 0) {
        fprintf(stderr, "setsockopt SO_SNDTIMEO failed![%s]", strerror(errno));
        return -1;
    }

    return 0;
}

T_KRClient *kr_client_connect(char *ip, int port)
{
    T_KRClient *krclient = malloc(sizeof(*krclient));
    if (krclient == NULL) {
        fprintf(stderr, "malloc krclient failed!\n");
        return NULL;
    }

    krclient->id = kr_strdup("krclient");//FIXME
    krclient->ip = kr_strdup(ip);
    krclient->port = port;
    krclient->fd = kr_net_tcp_connect(krclient->errmsg, 
            krclient->ip, krclient->port);
    if (krclient->fd <= 0) {
        fprintf(stderr, "connect server [%s:%d] failed[%s]\n",
                krclient->ip, krclient->port, krclient->errmsg);
        kr_client_disconnect(krclient);
        return NULL;
    }
    kr_net_nonblock(NULL, krclient->fd);
    kr_net_tcp_nodelay(NULL, krclient->fd);

    /*default timeout is 3 seconds*/
    if (kr_client_set_timeout(krclient, 3) != 0) {
        fprintf(stderr, "kr_client_set_timeout 3 failed!\n");
        kr_client_disconnect(krclient);
        return NULL;
    }

    return krclient;
}

int kr_client_reconnect(T_KRClient *krclient)
{
    /*close old*/
    close(krclient->fd);
    shutdown(krclient->fd, SHUT_RDWR);

    /*reconnect*/
    krclient->fd = kr_net_tcp_connect(krclient->errmsg, 
            krclient->ip, krclient->port);
    if (krclient->fd <= 0) {
        fprintf(stderr, "reconnect server [%s:%d] failed[%s]\n",
                krclient->ip, krclient->port, krclient->errmsg);
        return -1;
    }
    kr_net_nonblock(NULL, krclient->fd);
    kr_net_tcp_nodelay(NULL, krclient->fd);

    /*default timeout is 3 seconds*/
    if (kr_client_set_timeout(krclient, 3) != 0) {
        fprintf(stderr, "kr_client_set_timeout 3 failed!\n");
        return -1;
    }

    return 0;
}

T_KRMessage *kr_client_apply(T_KRClient *krclient, char *method, int datasrc, int msglen, void *msgbuf)
{
    /*if fd is not readable, reconnect it*/
    /*
    if (FD_ISSET(krclient->fd) == 0) {
        if (kr_client_reconnect(krclient) != 0) {
            fprintf(stderr, "kr_client_reconnect failed!\n");
            return NULL;
        }
    }
    */

    /*alloc request*/
    T_KRMessage *apply = kr_message_alloc();
    if (apply == NULL) {
        fprintf(stderr, "kr_message_alloc request failed!\n");
        return NULL;
    }

    /*set request*/
    char *msgid="0001";//FIXME
    strcpy(apply->msgid, msgid);
    strcpy(apply->method, method);
    apply->datasrc = datasrc;
    apply->msglen = msglen;
    apply->msgbuf = msgbuf;

    /*send request*/
    if (kr_client_write_message(krclient->fd, apply) <= 0) {
        fprintf(stderr, "kr_client_write_message request failed!\n");
        kr_message_free(apply);
        return NULL;
    }

    /*get response*/
    T_KRMessage *reply = kr_client_read_message(krclient->fd);
    if (reply == NULL) {
        fprintf(stderr, "kr_client_read_message response failed!\n");
        kr_message_free(apply);
        return NULL;
    }

    kr_message_free(apply);
    return reply;
}


T_KRMessage *kr_client_info(T_KRClient *krclient)
{
    return kr_client_apply(krclient, "info_engine", 0, 0, NULL);
}

T_KRMessage *kr_client_info_log(T_KRClient *krclient)
{
    return kr_client_apply(krclient, "info_log", 0, 0, NULL);
}

T_KRMessage *kr_client_set_logpath(T_KRClient *krclient, char *log_path)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "log_path", log_path);
    char *msgbuf = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);

    return kr_client_apply(krclient, "set_logpath", 0, strlen(msgbuf), msgbuf);
}

T_KRMessage *kr_client_set_logname(T_KRClient *krclient, char *log_name)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "log_name", log_name);
    char *msgbuf = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);

    return kr_client_apply(krclient, "set_logname", 0, strlen(msgbuf), msgbuf);
}

T_KRMessage *kr_client_set_loglevel(T_KRClient *krclient, int log_level)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "log_level", log_level);
    char *msgbuf = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);

    return kr_client_apply(krclient, "set_loglevel", 0, strlen(msgbuf), msgbuf);
}

T_KRMessage *kr_client_info_krdb(T_KRClient *krclient)
{
    return kr_client_apply(krclient, "info_krdb", 0, 0, NULL);
}

T_KRMessage *kr_client_info_table(T_KRClient *krclient, int table_id)
{
    return kr_client_apply(krclient, "info_table", table_id, 0, NULL);
}

T_KRMessage *kr_client_info_index(T_KRClient *krclient, int table_id, int index_id)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "index_id", index_id);
    char *msgbuf = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);

    return kr_client_apply(krclient, "info_index", table_id, strlen(msgbuf), msgbuf);
}

T_KRMessage *kr_client_list_index_key(T_KRClient *krclient)
{
    return kr_client_apply(krclient, "list_index_key", 0, 0, NULL);
}

T_KRMessage *kr_client_reload_param(T_KRClient *krclient)
{
    return kr_client_apply(krclient, "reload_param", 0, 0, NULL);
}

T_KRMessage *kr_client_info_param(T_KRClient *krclient)
{
    return kr_client_apply(krclient, "info_param", 0, 0, NULL);
}

T_KRMessage *kr_client_info_group(T_KRClient *krclient, int group_id)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "group_id", group_id);
    char *msgbuf = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);

    return kr_client_apply(krclient, "info_group", 0, strlen(msgbuf), msgbuf);
}

T_KRMessage *kr_client_info_group_rule(T_KRClient *krclient, int group_id, int rule_id)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "group_id", group_id);
    cJSON_AddNumberToObject(json, "rule_id", rule_id);
    char *msgbuf = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);

    return kr_client_apply(krclient, "info_rule", 0, strlen(msgbuf), msgbuf);
}

T_KRMessage *kr_client_info_set(T_KRClient *krclient, int set_id)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "set_id", set_id);
    char *msgbuf = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);

    return kr_client_apply(krclient, "info_set", 0, strlen(msgbuf), msgbuf);
}

T_KRMessage *kr_client_info_sdi(T_KRClient *krclient, int sdi_id)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "sdi_id", sdi_id);
    char *msgbuf = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);

    return kr_client_apply(krclient, "info_sdi", 0, strlen(msgbuf), msgbuf);
}

T_KRMessage *kr_client_info_ddi(T_KRClient *krclient, int ddi_id)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "ddi_id", ddi_id);
    char *msgbuf = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);

    return kr_client_apply(krclient, "info_ddi", 0, strlen(msgbuf), msgbuf);
}

T_KRMessage *kr_client_info_hdi(T_KRClient *krclient, int hdi_id)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "hdi_id", hdi_id);
    char *msgbuf = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);

    return kr_client_apply(krclient, "info_hdi", 0, strlen(msgbuf), msgbuf);
}

T_KRMessage *kr_client_insert_event(T_KRClient *krclient, int table_id, char *event)
{
    char *msgbuf = strdup(event);
    return kr_client_apply(krclient, "insert_event", table_id, strlen(msgbuf), msgbuf);
}

T_KRMessage *kr_client_detect_event(T_KRClient *krclient, int table_id, char *event)
{
    char *msgbuf = strdup(event);
    return kr_client_apply(krclient, "detect_event", table_id, strlen(msgbuf), msgbuf);
}

static void kr_client_tick_time(char *memo)
{
    time_t tTime = time(NULL);
    struct tm *ptmNow = localtime(&tTime);
    char caTimeString[80] = {0};
    strftime(caTimeString, sizeof(caTimeString), "%c", ptmNow);
    printf("%s: time tick:[%s]!\n", memo, caTimeString);
}

int kr_client_apply_file(T_KRClient *krclient, char *method, int table_id, char *applyfile)
{
    int iCnt = 0;
    char buff[1024] = {0};

    FILE *fp = fopen(applyfile, "r");
    if (fp == NULL) {
        fprintf(stderr, "fopen applyfile [%s] failed!\n", applyfile);
        return -1;
    }

    kr_client_tick_time("start");
    while(fgets(buff, sizeof(buff), fp) != NULL)
    {
        if (buff[0] == ' ') continue;

        /*apply this line*/
        char *msgbuf = strdup(buff);
        T_KRMessage *reply = kr_client_apply(krclient, \
                method, table_id, strlen(msgbuf), msgbuf);
        if (reply == NULL) {
            fprintf(stderr, "kr_client_apply [%zu] [%s] failed!\n", 
                    strlen(msgbuf), msgbuf);
            return -1;
        }
        /*print reply*/
        /*
        if (reply->msgtype == KR_MSGTYPE_SUCCESS) {
            fprintf(stdout, "SUCCESS: %s\n", reply->msgbuf);
        } else {
            fprintf(stdout, "ERROR!\n");
        }
        */
        kr_message_free(reply);

        if(iCnt%1000 == 0 && iCnt != 0) {
            char caNum[20] = {0};
            snprintf(caNum, sizeof(caNum), "Records:%010d", iCnt);
            kr_client_tick_time(caNum);
        }
        iCnt++;
    }
    kr_client_tick_time("stop");
    fclose(fp);

    return 0;
}

int kr_client_insert_file(T_KRClient *krclient, int table_id, char *applyfile)
{
    if (access(applyfile, R_OK) != 0) {
        fprintf(stderr, "file [%s] can not access!\n", applyfile);
        return -1;
    }

    return kr_client_apply_file(krclient, "insert_event", table_id, applyfile);
}

int kr_client_detect_file(T_KRClient *krclient, int table_id, char *applyfile)
{
    if (access(applyfile, R_OK) != 0) {
        fprintf(stderr, "file [%s] can not access!\n", applyfile);
        return -1;
    }

    return kr_client_apply_file(krclient, "detect_event", table_id, applyfile);
}

