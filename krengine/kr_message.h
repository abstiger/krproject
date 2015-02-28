#ifndef __KR_MESSAGE_H__
#define __KR_MESSAGE_H__

#include <stddef.h>

/* The message format for krproject communication:
 * |msgid(16bytes)|method(20bytes)|datasrc(4bytes)|msglen(4bytes)|message(msglen bytes)|
 *   */

#define KR_MSGID_LEN 16
#define KR_METHOD_LEN 20
#define KR_DATASRC_LEN 4
#define KR_MSGLEN_LEN 4
#define KR_MSGHEADER_LEN \
    (KR_MSGID_LEN+KR_METHOD_LEN+KR_DATASRC_LEN+KR_MSGLEN_LEN)


/*reply msgtype*/
#define    KR_MSGTYPE_ERROR            -1
#define    KR_MSGTYPE_SUCCESS           0


#define KR_BUFFER_MAX_LEN 16*1024


typedef enum {
    KR_MSGFMT_FIXED = 'F',
    KR_MSGFMT_JSON  = 'J',
    KR_MSGFMT_AVRO  = 'A'
}E_KRMsgFmt;


typedef struct _kr_buffer_t
{
    size_t        size;
    char          data[KR_BUFFER_MAX_LEN];
}T_KRBuffer;


typedef struct _kr_message_t
{
    char          msgid[KR_MSGID_LEN+1];
    char          method[KR_METHOD_LEN+1];
    E_KRMsgFmt    msgfmt;
    int           msgtype;
    int           datasrc;
    size_t        msglen;
    char         *msgbuf;
}T_KRMessage;


T_KRMessage *kr_message_alloc(void);
void kr_message_free(T_KRMessage *krmsg);

int kr_message_parse(T_KRBuffer *krbuf, T_KRMessage *krmsg);
int kr_message_dump(T_KRMessage *krmsg, T_KRBuffer *krbuf);

#endif /* __KR_MESSAGE_H__ */
