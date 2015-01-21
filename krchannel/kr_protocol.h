#ifndef __KR_PROTOCOL_H__
#define __KR_PROTOCOL_H__

#include "krutils/kr_utils.h"
#include "krengine/kr_message.h"


typedef struct _kr_buffer_t
{
    size_t        len;
    char         *buf;
}T_KRBuffer;

typedef enum {
    KR_MSGSTATE_INIT = 0,
    KR_MSGSTATE_HEAD = 1,
    KR_MSGSTATE_BODY = 2,
    KR_MSGSTATE_STOP = 3
}E_KRMsgState;


/*forward declaration*/
typedef struct _kr_protocol_config_t T_KRProtocolConfig;
typedef struct _kr_protocol_t T_KRProtocol;


/*function declaration*/
typedef int (*KRProtocolParseFunc)(T_KRProtocol *protocol, T_KRBuffer *buffer, T_KRMessage *message);
typedef int (*KRProtocolDumpFunc)(T_KRProtocol *protocol, T_KRMessage *message, T_KRBuffer *buffer);

struct _kr_protocol_config_t
{
    char    *protocol;

    /*protocol function*/
    KRProtocolParseFunc parse_func;
    KRProtocolDumpFunc dump_func;

};


struct _kr_protocol_t
{
    T_KRProtocolConfig *kr_protocol_config;


    E_KRMsgState        state;
    E_KRMsgFmt          fmt;
};


#endif /* __KR_PROTOCOL_H__ */
