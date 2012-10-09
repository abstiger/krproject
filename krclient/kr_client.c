#include "krutils/kr_utils.h"
#include "krutils/kr_message.h"
#include "krdb/kr_db_interface.h"
#include <errno.h>
#include <signal.h>

#define UNIX_DOMAIN "/tmp/krcoordi.domain"    
#define TCP_IP "127.0.0.1"
#define TCP_PORT 7250

char caNetErr[KR_NET_ERR_LEN] = {0};

int main(int argc, char *argv[])
{  
    int ret = 0;
    int unixfd = 0;
    int tcpfd = 0;
    int i = 0;
    int iResult = 0;
    char caTmpBuff[200] = {0};
    T_KRTradFlow_1 stTradFlow1 = {0};
	time_t	lTime;
	struct tm	*tTmLocal;
	char	sLogTime[128];
    T_KRMessage stMsg = {0};
    int iWriteLen = 0;
    stMsg.msgtype = KR_MSGTYPE_APPLY;
    strcpy(stMsg.serverid, "krserver5");
    strcpy(stMsg.clientid, "krclient1");
    stMsg.datasrc = 1;
    stMsg.msglen = sizeof(T_KRTradFlow_1);
    stMsg.msgbuf = kr_malloc(stMsg.msglen);
    
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    unixfd = kr_net_unix_connect(caNetErr, UNIX_DOMAIN);
    if (unixfd <= 0) {
        printf("kr_net_unix_connect [%s] failed [%s]\n", UNIX_DOMAIN, caNetErr);
        return -1;
    }
    
    tcpfd = kr_net_tcp_connect(caNetErr, TCP_IP, TCP_PORT);
    if (tcpfd <= 0) {
        printf("kr_net_tcp_connect [%s] [%d]failed [%s]\n", TCP_IP, TCP_PORT, caNetErr);
        return -1;
    }
    
    /*client on*/    
    T_KRMessage stMessage = {0};
    stMessage.msgtype = KR_MSGTYPE_CLION;
    strcpy(stMessage.serverid, "krserver1");
    strcpy(stMessage.clientid, "krclient1");
    
    if (kr_message_write(caNetErr, tcpfd, &stMessage) <= 0) {
        while (errno == EAGAIN) {
	        kr_message_write(caNetErr, tcpfd, &stMessage);
	    }
        printf("kr_message_write tcpfd failed[%s]!\n", caNetErr);
        return -1;
    }
    //sleep(2);
    
    //Step 2:FraudDetect
    for (i=0; i<22; i++)
    {
        memset(&stTradFlow1, 0x00, sizeof(T_KRTradFlow_1));
        
        memset(caTmpBuff, 0x00, sizeof(caTmpBuff));
        sprintf(caTmpBuff, "%s%04d", "6223000000000000", i%10);
        memcpy(stTradFlow1.caOutCustNo, caTmpBuff, sizeof(stTradFlow1.caOutCustNo));
                
        memset(caTmpBuff, 0x00, sizeof(caTmpBuff));
        sprintf(caTmpBuff, "201205%02d", i%7);
        memcpy(stTradFlow1.caOutTransDate, caTmpBuff, sizeof(stTradFlow1.caOutTransDate));
        
        memset(caTmpBuff, 0x00, sizeof(caTmpBuff));
        sprintf(caTmpBuff, "0919%02d", i%60);
        memcpy(stTradFlow1.caOutTransTime, caTmpBuff, sizeof(stTradFlow1.caOutTransTime));

        memset(caTmpBuff, 0x00, sizeof(caTmpBuff));
        sprintf(caTmpBuff, "%015d", i);
        memcpy(stTradFlow1.caOutFlowNo, caTmpBuff, sizeof(stTradFlow1.caOutFlowNo));
        
        memset(caTmpBuff, 0x00, sizeof(caTmpBuff));
        sprintf(caTmpBuff, "%02d", i%2);
        memcpy(stTradFlow1.caOutTransType, caTmpBuff, sizeof(stTradFlow1.caOutTransType));
        
        stTradFlow1.dOutTransAmt = i*9.9;
        
        memset(caTmpBuff, 0x00, sizeof(caTmpBuff));
        sprintf(caTmpBuff, "TRANS_LOCATION:%02d", i);
        memcpy(stTradFlow1.caOutTransLoc, caTmpBuff, sizeof(stTradFlow1.caOutTransLoc));
        
        
        memcpy(stMsg.msgbuf, &stTradFlow1, sizeof(T_KRTradFlow_1));
        
        strcpy(stMsg.objectkey, stTradFlow1.caOutCustNo);
printf("Write message:msgtype[%d], serverid[%s], clientid[%s], datasrc[%d], objectkey[%s], msglen[%d], msgbuf:[%s]\n", \
        stMsg.msgtype, stMsg.serverid, stMsg.clientid, stMsg.datasrc, stMsg.objectkey, stMsg.msglen, (char *)stMsg.msgbuf);
        
        if (1 == 0) {
            if (kr_message_write(caNetErr, unixfd, &stMsg) <= 0) {
                while (errno == EAGAIN) {
	                kr_message_write(caNetErr, unixfd, &stMsg);
	            }
                printf("kr_message_write unixfd failed[%s]!\n", caNetErr);
                break;
            }
        } else {
            if (kr_message_write(caNetErr, tcpfd, &stMsg) <= 0) {
                while (errno == EAGAIN) {
	                kr_message_write(caNetErr, unixfd, &stMsg);
	            }
                printf("kr_message_write tcpfd failed[%s]!\n", caNetErr);
                break;
            }
        }
printf("caNetErr=[%s], error[%s]!\n", caNetErr, strerror(errno));	    
	    
    }
    
    kr_free(stMsg.msgbuf);
    //sleep(10);
    close(unixfd);
    close(tcpfd);
    
    return 0;
}  

