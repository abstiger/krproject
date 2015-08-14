#include "kr_shell_protocol.h"


int kr_request_dump(T_KRRequest *req, T_KRBuffer *krbuf)
{
    char *p = krbuf->data;

    memcpy(p, req->msgid, KR_MSGID_LEN);
    p = p + KR_MSGID_LEN;

    kr_put_u32(p, req->msgsrc);
    p = p + KR_MSGSRC_LEN;

    memcpy(p, req->msgfunc, KR_MSGFUNC_LEN);
    p = p + KR_MSGFUNC_LEN;
    
    kr_put_u32(p, req->msglen);
    p = p + KR_MSGLEN_LEN;

    memcpy(p, req->msgbuf, req->msglen);
    p = p + req->msglen;

    krbuf->size = KR_MSGHEADER_LEN + req->msglen;

    return krbuf->size;
}

int kr_request_parse(T_KRBuffer *krbuf, T_KRRequest *req)
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
    memcpy(req->msgid, p, KR_MSGID_LEN);
    p = p + KR_MSGID_LEN;

    req->msgsrc = kr_get_u32(p);
    p = p + KR_MSGSRC_LEN;

    memcpy(req->msgfunc, p, KR_MSGFUNC_LEN);
    p = p + KR_MSGFUNC_LEN;

    req->msglen = kr_get_u32(p);
    p = p + KR_MSGLEN_LEN;

    req->msgbuf = kr_calloc(req->msglen);
    memcpy(req->msgbuf, p, req->msglen);
    p = p + req->msglen;

    char buff[KR_BUFFER_MAX_LEN] = {0};
    memcpy(buff, p, krbuf->size-KR_MSGHEADER_LEN-req->msglen);
    memcpy(krbuf->data, buff, KR_BUFFER_MAX_LEN);

    krbuf->size = krbuf->size-KR_MSGHEADER_LEN-req->msglen;

    return 0;
}


int kr_response_dump(T_KRResponse *resp, T_KRBuffer *krbuf)
{
    char *p = krbuf->data;

    memcpy(p, resp->msgid, KR_MSGID_LEN);
    p = p + KR_MSGID_LEN;

    kr_put_u32(p, resp->msgsrc);
    p = p + KR_MSGSRC_LEN;

    memcpy(p, resp->msgfunc, KR_MSGFUNC_LEN);
    p = p + KR_MSGFUNC_LEN;
    
    kr_put_u32(p, resp->msglen);
    p = p + KR_MSGLEN_LEN;

    memcpy(p, resp->msgbuf, resp->msglen);
    p = p + resp->msglen;

    krbuf->size = KR_MSGHEADER_LEN + resp->msglen;

    return krbuf->size;
}

int kr_response_parse(T_KRBuffer *krbuf, T_KRResponse *resp)
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
    memcpy(resp->msgid, p, KR_MSGID_LEN);
    p = p + KR_MSGID_LEN;

    resp->msgsrc = kr_get_u32(p);
    p = p + KR_MSGSRC_LEN;

    memcpy(resp->msgfunc, p, KR_MSGFUNC_LEN);
    p = p + KR_MSGFUNC_LEN;

    resp->msglen = kr_get_u32(p);
    p = p + KR_MSGLEN_LEN;

    resp->msgbuf = kr_calloc(resp->msglen);
    memcpy(resp->msgbuf, p, resp->msglen);
    p = p + resp->msglen;

    char buff[KR_BUFFER_MAX_LEN] = {0};
    memcpy(buff, p, krbuf->size-KR_MSGHEADER_LEN-resp->msglen);
    memcpy(krbuf->data, buff, KR_BUFFER_MAX_LEN);

    krbuf->size = krbuf->size-KR_MSGHEADER_LEN-resp->msglen;

    return 0;
}
