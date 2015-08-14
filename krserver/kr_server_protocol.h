#ifndef __KR_SERVER_PROTOCOL_H__
#define __KR_SERVER_PROTOCOL_H__

#include "krutils/kr_utils.h"
#include "krengine/kr_engine.h"

/* The message format for krproject communication:
 * |msgid(16b)|msgsrc(4b)|msgfunc(20b)|msglen(4b)|message(msglen bytes)
 */
#define KR_MSGID_LEN 16
#define KR_MSGSRC_LEN 4
#define KR_MSGFUNC_LEN 20
#define KR_MSGLEN_LEN 4
#define KR_MSGHEADER_LEN 44

#define KR_BUFFER_MAX_LEN 16*1024

typedef struct _kr_buffer_t
{
    size_t        size;
    char          data[KR_BUFFER_MAX_LEN];
}T_KRBuffer;


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


/* basic functions */
int kr_request_dump(T_KRRequest *req, T_KRBuffer *krbuf);
int kr_request_parse(T_KRBuffer *krbuf, T_KRRequest *req);

int kr_response_dump(T_KRResponse *resp, T_KRBuffer *krbuf);
int kr_response_parse(T_KRBuffer *krbuf, T_KRResponse *resp);

#endif  /*__KR_SERVER_PROTOCOL_H__*/

