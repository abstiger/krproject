#ifndef __KR_SHELL_PROTOCOL_H__
#define __KR_SHELL_PROTOCOL_H__

#include <stdio.h>
#include <stdint.h>
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


static inline uint32_t kr_get_u32(const uint8_t *buf)
{
    return (((uint32_t) buf [0]) << 24) |
        (((uint32_t) buf [1]) << 16) |
        (((uint32_t) buf [2]) << 8) |
        ((uint32_t) buf [3]);
}


static inline void kr_put_u32(uint8_t *buf, uint32_t val)
{
    buf [0] = (uint8_t) (((val) >> 24) & 0xff);
    buf [1] = (uint8_t) (((val) >> 16) & 0xff);
    buf [2] = (uint8_t) (((val) >> 8) & 0xff);
    buf [3] = (uint8_t) (val & 0xff);
}


#endif  /*__KR_SHELL_PROTOCOL_H__*/

