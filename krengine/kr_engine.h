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
        char *dbname, char *dbuser, char *dbpass,
        char *logpath, char *logname, int loglevel,
        int shmkey, char *krdbname, char *dbmodulename,
        int hdicachesize, int threadcnt);
int kr_engine_run(T_KREngine *krengine, E_KROprCode oprcode, int datasrc, char *msgbuf);
void kr_engine_shutdown(T_KREngine *krengine);
char *kr_engine_info(T_KREngine *krengine);

#endif  /*__KR_ENGINE_H__*/

