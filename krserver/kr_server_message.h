#ifndef __KR_MESSAGE_H__
#define __KR_MESSAGE_H__

/* The message format for krproject communication:
 * |msgtype(4bytes)|msgid(10bytes)|serverid(26bytes)|clientid(26bytes)|datasrcid(4bytes)|objectkey(64bytes)|msglen(4bytes)|message(msglen bytes)|
 */


#define KR_MSGTYPE_LEN 4
#define KR_MSGID_LEN 16
#define KR_SERVERID_LEN 26
#define KR_CLIENTID_LEN 26
#define KR_OBJECTKEY_LEN 48
#define KR_DATASRCID_LEN 4
#define KR_MSGLEN_LEN 4

#define KR_MSGHEADER_LEN 128
#define KR_MSGHEADER_FMT "%4d%16s%26s%26s%4d%48s%4d"
#define KR_MSGBODY_MAXLEN 4096

typedef enum {
    KR_LISTENMODE_DOMAIN = '0',
    KR_LISTENMODE_TCPIP  = '1'
}E_KRListenMode;


typedef enum {
    KR_MSGTYPE_SVRON  = 0,
    KR_MSGTYPE_SVROFF,
    KR_MSGTYPE_CLION,
    KR_MSGTYPE_CLIOFF,
    KR_MSGTYPE_APPLY,
    KR_MSGTYPE_REPLY,
    KR_MSGTYPE_HEART
}E_KRMsgType;


typedef struct _kr_msg_svron_t
{
    unsigned int           weights;
    int                    replica;
}T_KRMsgSvrOn;


typedef struct _kr_message_t
{
    int         fd;
    int         msgtype;
    char        msgid[KR_MSGID_LEN+1];
    char        serverid[KR_SERVERID_LEN+1];
    char        clientid[KR_CLIENTID_LEN+1];
    int         datasrc;
    char        objectkey[KR_OBJECTKEY_LEN+1];
    int         msglen;
    char        msgbuf[KR_MSGBODY_MAXLEN+1];
}T_KRMessage;


int kr_message_read(char *err, int fd, T_KRMessage *ptMessage);
int kr_message_write(char *err, int fd, T_KRMessage *ptMessage);

#endif /* __KR_MESSAGE_H__ */
