#ifndef __KR_ENGINE_H__
#define __KR_ENGINE_H__

#include <stddef.h>

/* forward declaration */
typedef struct _kr_engine_t T_KREngine;

/* The message format for krproject communication:
 * |msgid(16b)|method(20b)|datasrc(4b)|msglen(4b)|message(msglen bytes)
 */
#define KR_MSGID_LEN 16
#define KR_METHOD_LEN 20
#define KR_DATASRC_LEN 4
#define KR_MSGLEN_LEN 4

#define KR_MSGHEADER_LEN 44

/*reply msgtype*/
#define    KR_MSGTYPE_ERROR            -1
#define    KR_MSGTYPE_SUCCESS           0

#define KR_BUFFER_MAX_LEN 16*1024


typedef struct _kr_buffer_t
{
    size_t        size;
    char          data[KR_BUFFER_MAX_LEN];
}T_KRBuffer;


typedef struct _kr_message_t
{
    int           msgtype;
    char          msgid[KR_MSGID_LEN+1];
    char          method[KR_METHOD_LEN+1];
    int           datasrc;
    size_t        msglen;
    char         *msgbuf;
}T_KRMessage;


typedef void (*KRCBFunc)(void *apply, void *reply, void *data);
typedef void (*KRHandleFunc)(void *ctx, void *arg);

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
typedef struct _kr_engine_arg_t
{
    T_KRMessage      *apply;       /* request message */
    T_KRMessage      *reply;       /* response message */
    KRCBFunc         cb_func;      /* call back function */
    void             *data;        /* pointer to user's data */
}T_KREngineArg;


/* basic functions of krmessage */
T_KRMessage *kr_message_alloc(void);
void kr_message_free(T_KRMessage *krmsg);
int kr_message_parse(T_KRBuffer *krbuf, T_KRMessage *krmsg);
int kr_message_dump(T_KRMessage *krmsg, T_KRBuffer *krbuf);

/* basic functions of krengine */
T_KREngine *kr_engine_startup(T_KREngineConfig *arg, void *data);
void kr_engine_shutdown(T_KREngine *engine);
int kr_engine_run(T_KREngine *engine, T_KREngineArg *arg);
int kr_engine_register(T_KREngine *engine, char *method, KRHandleFunc func);

#endif  /*__KR_ENGINE_H__*/

