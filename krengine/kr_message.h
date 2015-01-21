#ifndef __KR_MESSAGE_H__
#define __KR_MESSAGE_H__

#include <stddef.h>


/*reply msgtype*/
#define    KR_MSGTYPE_ERROR            -1
#define    KR_MSGTYPE_SUCCESS           0


typedef enum {
    KR_MSGFMT_FIXED = 'F',
    KR_MSGFMT_JSON  = 'J',
    KR_MSGFMT_AVRO  = 'A'
}E_KRMsgFmt;

typedef struct _kr_message_t
{
    int           fd;
    char         *method;
    char         *msgid;
    E_KRMsgFmt    msgfmt;
    int           msgtype;
    int           datasrc;
    size_t        msglen;
    char         *msgbuf;
}T_KRMessage;


T_KRMessage *kr_message_alloc(void);
void kr_message_free(T_KRMessage *krmsg);


#endif /* __KR_MESSAGE_H__ */
