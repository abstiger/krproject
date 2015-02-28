#ifndef __KR_ENGINE_H__
#define __KR_ENGINE_H__

#include "kr_message.h"

typedef void (*KRCBFunc)(void *apply, void *reply, void *data);
typedef void (*KRHandleFunc)(void *ctx, void *arg);

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


typedef struct _kr_engine_arg_t
{
    T_KRMessage      *apply;       /* request message */
    T_KRMessage      *reply;       /* response message */
    KRCBFunc         cb_func;      /* call back function */
    void             *data;        /* pointer to user's data */
}T_KREngineArg;


/* forward declaration */
typedef struct _kr_engine_t T_KREngine;

/* basic functions of krengine */
T_KREngine *kr_engine_startup(T_KREngineConfig *arg, void *data);
void kr_engine_shutdown(T_KREngine *engine);
int kr_engine_run(T_KREngine *engine, T_KREngineArg *arg);
int kr_engine_register(T_KREngine *engine, char *method, KRHandleFunc func);

#endif  /*__KR_ENGINE_H__*/

