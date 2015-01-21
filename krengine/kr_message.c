#include "krutils/kr_utils.h"
#include "kr_message.h"


T_KRMessage *kr_message_alloc(void)
{
    /*need calloc here*/
    T_KRMessage *krmsg = kr_calloc(sizeof(*krmsg));
    //FIXME:add function to set fd
//    krmsg->fd = fd;
    return krmsg;
}


void kr_message_free(T_KRMessage *krmsg)
{
    if (krmsg) {
        if (krmsg->msgbuf) kr_free(krmsg->msgbuf);
        kr_free(krmsg); 
    }
}


