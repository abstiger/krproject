#include "kr_request.h"
#include "krutils/kr_alloc.h"

/* request message definetion */
struct _kr_request_t
{
    char          msgid[30+1];
    char          msgfunc[20+1];
    int           msgsrc;
    char          msgfmt[10+1];
    size_t        msglen;
    char         *msgbuf;
};


T_KRRequest *kr_request_alloc(void)
{
    /*need calloc here*/
    T_KRRequest *ptRequest = kr_calloc(sizeof(*ptRequest));
    return ptRequest;
}


void kr_request_free(T_KRRequest *ptRequest)
{
    if (ptRequest) {
        if (ptRequest->msgbuf) kr_free(ptRequest->msgbuf);
        kr_free(ptRequest); 
    }
}


