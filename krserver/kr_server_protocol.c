#include "kr_server_protocol.h"
#include "krutils/kr_utils.h"

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


T_KRMessage *kr_message_alloc(void)
{
    /*need calloc here*/
    T_KRMessage *krmsg = kr_calloc(sizeof(*krmsg));
    return krmsg;
}


void kr_message_free(T_KRMessage *krmsg)
{
    if (krmsg) {
        if (krmsg->msgbuf) kr_free(krmsg->msgbuf);
        kr_free(krmsg); 
    }
}


int kr_message_parse(T_KRBuffer *krbuf, T_KRMessage *krmsg)
{
    /*check header length*/
    if (krbuf->size < KR_MSGHEADER_LEN) {
        return 1;
    }

    /*get message body length*/
    size_t msglen = kr_get_u32(krbuf->data+KR_MSGHEADER_LEN-KR_MSGLEN_LEN);
    if (msglen < 0) {
        return -1;
    }

    /*check body length*/
    if (krbuf->size < KR_MSGHEADER_LEN+msglen) {
        return 2;
    }

    char *p = krbuf->data;
    memcpy(krmsg->msgid, p, KR_MSGID_LEN);
    p = p + KR_MSGID_LEN;

    memcpy(krmsg->method, p, KR_METHOD_LEN);
    p = p + KR_METHOD_LEN;

    krmsg->datasrc = kr_get_u32(p);
    p = p + KR_DATASRC_LEN;

    krmsg->msglen = kr_get_u32(p);
    p = p + KR_MSGLEN_LEN;

    krmsg->msgbuf = kr_calloc(krmsg->msglen);
    memcpy(krmsg->msgbuf, p, krmsg->msglen);
    p = p + krmsg->msglen;

    char buff[KR_BUFFER_MAX_LEN] = {0};
    memcpy(buff, p, krbuf->size-KR_MSGHEADER_LEN-krmsg->msglen);
    memcpy(krbuf->data, buff, KR_BUFFER_MAX_LEN);

    krbuf->size = krbuf->size-KR_MSGHEADER_LEN-krmsg->msglen;

    return 0;
}


int kr_message_dump(T_KRMessage *krmsg, T_KRBuffer *krbuf)
{
    char *p = krbuf->data;

    memcpy(p, krmsg->msgid, KR_MSGID_LEN);
    p = p + KR_MSGID_LEN;

    memcpy(p, krmsg->method, KR_METHOD_LEN);
    p = p + KR_METHOD_LEN;
    
    kr_put_u32(p, krmsg->datasrc);
    p = p + KR_DATASRC_LEN;

    kr_put_u32(p, krmsg->msglen);
    p = p + KR_MSGLEN_LEN;

    memcpy(p, krmsg->msgbuf, krmsg->msglen);
    p = p + krmsg->msglen;

    krbuf->size = KR_MSGHEADER_LEN + krmsg->msglen;

    return krbuf->size;
}

