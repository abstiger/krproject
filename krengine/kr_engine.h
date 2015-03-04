#ifndef __KR_ENGINE_H__
#define __KR_ENGINE_H__

#include <stdio.h>

/* forward declaration */
typedef struct _kr_engine_t T_KREngine;


/*reply msgtype*/
typedef enum _kr_msgtype_t
{
    KR_MSGTYPE_SUCCESS     = 0,
    KR_MSGTYPE_ERROR       = 1
}E_KRMsgType;

/* input and output message definetion */
typedef struct _kr_message_t
{
    E_KRMsgType   msgtype;
    char          msgid[30+1];
    char          method[20+1];
    int           datasrc;
    size_t        msglen;
    char         *msgbuf;
}T_KRMessage;

/* initial configure of krengine */
typedef struct _kr_engine_config_t 
{
    char          *logpath;          /* path of log file */
    char          *logname;          /* name of log file */
    int            loglevel;         /* log level of this server */
    
    char          *dbname;           /* database name, dsn */
    char          *dbuser;           /* database username */
    char          *dbpass;           /* database user password */

    char          *krdb_module;      /* name of krdb's module */
    char          *data_module;      /* name of data's module */
    char          *rule_module;      /* name of rule's module */

    int            hdi_cache_size;   /* hdi cache size */
    int            thread_pool_size; /* thread pool size */
    int            high_water_mark;  /* thread pool high water mark */
}T_KREngineConfig;


/* kr_engine_run's argument, packed with this struct */
typedef void (*KRCBFunc)(void *apply, void *reply, void *data);
typedef struct _kr_engine_arg_t
{
    T_KRMessage      *apply;       /* request message */
    T_KRMessage      *reply;       /* response message */
    KRCBFunc         cb_func;      /* call back function */
    void             *data;        /* pointer to user's data */
}T_KREngineArg;


/* basic functions of krengine */
typedef void (*KRHandleFunc)(void *ctx, void *arg);
T_KREngine *kr_engine_startup(T_KREngineConfig *arg, void *data);
int kr_engine_register(T_KREngine *engine, char *method, KRHandleFunc func);
int kr_engine_run(T_KREngine *engine, T_KREngineArg *arg);
void kr_engine_shutdown(T_KREngine *engine);

#endif  /*__KR_ENGINE_H__*/

