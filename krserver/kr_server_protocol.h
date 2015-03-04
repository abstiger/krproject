#ifndef __KR_SERVER_PROTOCOL_H__
#define __KR_SERVER_PROTOCOL_H__

#include <stddef.h>
#include "krengine/kr_engine.h"

/* The message format for krproject communication:
 * |msgid(16b)|method(20b)|datasrc(4b)|msglen(4b)|message(msglen bytes)
 */
#define KR_MSGID_LEN 16
#define KR_METHOD_LEN 20
#define KR_DATASRC_LEN 4
#define KR_MSGLEN_LEN 4
#define KR_MSGHEADER_LEN 44

#define KR_BUFFER_MAX_LEN 16*1024

typedef struct _kr_buffer_t
{
    size_t        size;
    char          data[KR_BUFFER_MAX_LEN];
}T_KRBuffer;

/* basic functions of krmessage */
T_KRMessage *kr_message_alloc(void);
void kr_message_free(T_KRMessage *krmsg);
int kr_message_parse(T_KRBuffer *krbuf, T_KRMessage *krmsg);
int kr_message_dump(T_KRMessage *krmsg, T_KRBuffer *krbuf);

#endif  /*__KR_SERVER_PROTOCOL_H__*/

