#ifndef __KR_ENGINE_H__
#define __KR_ENGINE_H__

/* krengine's operation code */
typedef enum {
    KR_OPRCODE_INSERT = 0,
    KR_OPRCODE_DETECT = 1
}E_KROprCode;

/* forward declaration */
typedef struct _kr_engine_t T_KREngine;

T_KREngine *kr_engine_startup(
        char *dsn, char *user, char *pass,
        char *logpath, char *logname, int loglevel,
        int shmkey, char *krdbname, char *krdbmodule,
        char *datamodule, char *rulemodule,
        int hdicachesize, int threadcnt, int hwm,
        void *eventloop);
int kr_engine_run(T_KREngine *krengine, int fd, E_KROprCode oprcode, int datasrc, char *msgbuf);
void kr_engine_shutdown(T_KREngine *krengine);
char *kr_engine_info(T_KREngine *krengine);

#endif  /*__KR_ENGINE_H__*/

