#ifndef __KR_ENGINE_H__
#define __KR_ENGINE_H__

#include <stdio.h>

/* forward declaration */
typedef struct _kr_engine_t T_KREngine;

/* request message definetion */
typedef struct _kr_request_t
{
    char          msgid[30+1];
    char          msgfunc[20+1];
    int           msgsrc;
    char          msgfmt[10+1];
    size_t        msglen;
    char         *msgbuf;
}T_KRRequest;

/* response message definetion */
typedef struct _kr_response_t
{
    char          msgid[30+1];
    char          msgfunc[20+1];
    int           msgsrc;
    char          msgfmt[10+1];
    size_t        msglen;
    char         *msgbuf;
}T_KRResponse;

/* initial configure of krengine */
typedef struct _kr_engine_config_t 
{
    char          *logpath;          /* path of log file */
    char          *logname;          /* name of log file */
    int            loglevel;         /* log level of this server */
    
    char          *param_url;        /* param persist url */

    char          *input_module;     /* path of input module file */
    char          *output_module;    /* path of output module file */

    int            hdi_cache_size;   /* hdi cache size */
    int            thread_pool_size; /* thread pool size */
    int            high_water_mark;  /* thread pool high water mark */
}T_KREngineConfig;


/* kr_engine_run's argument, packed with this struct */
typedef void (*KRCBFunc)(T_KRRequest *req, T_KRResponse *resp, void *data);
typedef struct _kr_engine_arg_t
{
    T_KRRequest      *apply;       /* request message */
    T_KRResponse     *reply;       /* response message */
    KRCBFunc         cb_func;      /* call back function */
    void             *data;        /* pointer to user's data */
}T_KREngineArg;


/* basic functions of krengine */
T_KRRequest *kr_request_alloc(void);
void kr_request_free(T_KRRequest *ptRequest);
T_KRResponse *kr_response_alloc(void);
void kr_response_free(T_KRResponse *ptResponse);

typedef void (*KRHandleFunc)(void *ctx, void *arg);
T_KREngine *kr_engine_startup(T_KREngineConfig *arg, void *data);
int kr_engine_register(T_KREngine *engine, char *method, KRHandleFunc func);
int kr_engine_run(T_KREngine *engine, T_KREngineArg *arg);
void kr_engine_shutdown(T_KREngine *engine);

#endif  /*__KR_ENGINE_H__*/

