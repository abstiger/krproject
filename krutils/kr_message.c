#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "kr_alloc.h"
#include "kr_net.h"
#include "kr_message.h"

static void kr_message_set_error(char *err, const char *fmt, ...)
{
    va_list ap;

    if (!err) return;
    va_start(ap, fmt);
    vsnprintf(err, KR_NET_ERR_LEN, fmt, ap);
    va_end(ap);
}


int kr_message_read(char *err, int fd, T_KRMessage *ptMessage)
{
    int readLen = 0;
    char caMsgHeader[KR_MSGHEADER_LEN+1] = {0};

	/* read message header */
	readLen = kr_net_read(fd, caMsgHeader, KR_MSGHEADER_LEN);
	if( readLen < 0) {
        kr_message_set_error(err, "read msgheader error[%s]!", strerror(errno));
        return -1;
    } else if( readLen == 0) {
        kr_message_set_error(err, "Connection Closed!");
        return 0;
    }
    
    sscanf(caMsgHeader, KR_MSGHEADER_FMT, \
        &ptMessage->msgtype, ptMessage->serverid, ptMessage->clientid, \
        &ptMessage->datasrc, ptMessage->objectkey, &ptMessage->msglen);

    if (ptMessage->serverid[0] == '-') ptMessage->serverid[0] = '\0';
    if (ptMessage->clientid[0] == '-') ptMessage->clientid[0] = '\0';
    if (ptMessage->objectkey[0] == '-') ptMessage->objectkey[0] = '\0';
        
    /* read message body */
    if (readLen == KR_MSGHEADER_LEN && ptMessage->msglen > 0) {
        ptMessage->msgbuf = kr_calloc(ptMessage->msglen);
        readLen = kr_net_read(fd, ptMessage->msgbuf, ptMessage->msglen);
        if( readLen != ptMessage->msglen) {
            kr_message_set_error(err, "read body error[%s]!", strerror(errno));
            return readLen;
        }
    }

    return readLen;
}


int kr_message_write(char *err, int fd, T_KRMessage *ptMessage)
{
    int writeLen = 0;
    char caMsgHeader[KR_MSGHEADER_LEN+1] = {0};

    if (ptMessage->serverid[0] == '\0') ptMessage->serverid[0] = '-';
    if (ptMessage->clientid[0] == '\0') ptMessage->clientid[0] = '-';
    if (ptMessage->objectkey[0] == '\0') ptMessage->objectkey[0] = '-';
    
    snprintf(caMsgHeader, sizeof(caMsgHeader), KR_MSGHEADER_FMT, \
        ptMessage->msgtype, ptMessage->serverid, ptMessage->clientid, \
        ptMessage->datasrc, ptMessage->objectkey, ptMessage->msglen);
        
	/* write message header */
	writeLen = kr_net_write(fd, caMsgHeader, KR_MSGHEADER_LEN);
	if( writeLen < 0) {
	    kr_message_set_error(err, "write msgheader error[%s]!", strerror(errno));
        return -1;
    } else if( writeLen == 0) {
        kr_message_set_error(err, "Connection Closed!");
        return 0;
    }

    if (writeLen == KR_MSGHEADER_LEN && ptMessage->msglen > 0) {
        writeLen = kr_net_write(fd, ptMessage->msgbuf, ptMessage->msglen);
        if( writeLen != ptMessage->msglen) {
            kr_message_set_error(err, "write body error[%s]!", strerror(errno));
            return writeLen;
        }
    }

    return writeLen;
}


void kr_message_free(T_KRMessage *ptMessage)
{
    if (ptMessage) {
        if (ptMessage->msgbuf) 
            kr_free(ptMessage->msgbuf);
        kr_free(ptMessage);
    }
}