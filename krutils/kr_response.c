#include "kr_response.h"
#include "krutils/kr_alloc.h"

/* response message definetion */
struct _kr_response_t
{
    char          msgid[30+1];
    char          msgfunc[20+1];
    int           msgsrc;
    char          msgfmt[10+1];
    size_t        msglen;
    char         *msgbuf;
};


T_KRResponse *kr_response_alloc(void)
{
    /*need calloc here*/
    T_KRResponse *ptResponse = kr_calloc(sizeof(*ptResponse));
    return ptResponse;
}


void kr_response_free(T_KRResponse *ptResponse)
{
    if (ptResponse) {
        if (ptResponse->msgbuf) kr_free(ptResponse->msgbuf);
        kr_free(ptResponse); 
    }
}


