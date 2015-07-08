#include "kr_config.h"
#include "krutils/kr_utils.h"
#include "krutils/kr_threadpool.h"
#include "kr_engine.h"
#include "kr_engine_env.h"
#include "kr_engine_ctx.h"
#include "assert.h"

/* engine inner declaration */
struct _kr_engine_t
{
    char                *version;      /* version of engine */
    char                *info;         /* information of engine */

    T_KREngineEnv       *env;          /* environment of this engine */
    T_KREngineCtx       *ctx;          /* context of this engine */
    T_KRThreadPool      *tp;           /* thread pool */
};

static int kr_engine_handle(void *ctx, void *arg);


T_KREngine *kr_engine_startup(T_KREngineConfig *cfg, void *data)
{
    /* Set log file and level first*/
    if (cfg->logpath) kr_log_set_path(cfg->logpath);
    if (cfg->logname) kr_log_set_name(cfg->logname);
    if (cfg->loglevel) kr_log_set_level(cfg->loglevel);

    KR_LOG(KR_LOGDEBUG, "kr_engine_startup...");

    T_KREngine *engine = kr_calloc(sizeof(T_KREngine));
    if (engine == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc engine failed!");
        goto FAILED;
    }
    
    char caInfo[1024] = {0};
    engine->version = kr_strdup(VERSION);
    snprintf(caInfo, sizeof(caInfo), \
            "Welcome to %s !\n"
            "This is Version %s.\n"
            "If you find any bug or you'd like an enhancement,\n"
            "Please feel free to contact me: %s \n"
            "Check %s for details.\n",
            PACKAGE_NAME, PACKAGE_VERSION, PACKAGE_BUGREPORT, PACKAGE_URL);
    engine->info = kr_strdup(caInfo);

    /*create environment*/
    engine->env = kr_engine_env_create(cfg);
    if (engine->env == NULL) {
        KR_LOG(KR_LOGERROR, "kr_engine_env_create failed!");
        goto FAILED;
    }
    
    /* if no thread pool, initialize engine's context */
    if (cfg->thread_pool_size <= 0) {
        /* if no threadpool, initialize rule detecting context */
        engine->ctx = kr_engine_ctx_create(engine->env);
        if (engine->ctx == NULL) {
            KR_LOG(KR_LOGERROR, "kr_context_init failed!");
            goto FAILED;
        }
    } else { 
        /* else create and run thread pool */
        engine->tp = kr_threadpool_create(
                cfg->thread_pool_size, cfg->high_water_mark, engine->env,
                (KRThdInitFunc )kr_engine_ctx_create, 
                (KRThdFiniFunc )kr_engine_ctx_destroy);
        if (engine->tp == NULL) {
            KR_LOG(KR_LOGERROR, "kr_threadpool_create failed!");
            goto FAILED;
        }
        /* run thread pool */
        kr_threadpool_run(engine->tp);
    }

    return engine;

FAILED:
    kr_engine_shutdown(engine);
    return NULL;
}


void kr_engine_shutdown(T_KREngine *engine)
{
    KR_LOG(KR_LOGDEBUG, "kr_engine_shutdown...");
    
    if (engine == NULL) return;
    if (engine->ctx) {
        /* destroy context */
        kr_engine_ctx_destroy(engine->ctx);
    } else { 
        /* destroy threadpool */
        kr_threadpool_destroy(engine->tp);
    }

    /* destroy environment */
    kr_engine_env_destroy(engine->env);

    if (engine->version) kr_free(engine->version);
    if (engine->info) kr_free(engine->info);
    kr_free(engine);
}


int kr_engine_run(T_KREngine *engine, T_KREngineArg *arg)
{
    if (engine->ctx) {
        kr_engine_handle(engine->ctx, arg);
    } else {
        T_KRThreadPoolTask stTask = {kr_engine_handle, arg, sizeof(*arg)};
        if (kr_threadpool_add_task(engine->tp, &stTask) != 0) {
            KR_LOG(KR_LOGERROR, "kr_threadpool_add_task failed!");
            return -1;
        }
    }

    return 0;
}


static int kr_engine_handle(void *ctx, void *arg)
{
    T_KREngineCtx *krctx = (T_KREngineCtx *)ctx;
    T_KREngineArg *krarg = (T_KREngineArg *)arg;
    T_KRMessage *apply = (T_KRMessage *)krarg->apply;
    T_KRMessage *reply = (T_KRMessage *)krarg->reply;

    //check context, reload if parameter changed
    
    /*TODO: call interface module to process apply message */

    /* set context with arg */
    /*
    if (kr_context_set(krctx, krarg) != 0) {
        KR_LOG(KR_LOGERROR, "kr_context_set [%p] failed!", krarg);
        //FIXME:set error code
        //reply->msgtype = KR_MSGTYPE_ERROR;
        goto RESP;
    }
    */

    /* search handle for this apply message */
    /*
    KRFunc handle_func = kr_functable_search(\
            krctx->ptEnv->ptFuncTable, apply->method);
    if (handle_func == NULL) {
        KR_LOG(KR_LOGERROR, "unsupported method[%d]!", apply->method);
        //FIXME:set error code
        //reply->msgtype = KR_MSGTYPE_ERROR;
        goto RESP;
    }
    */

    /* excute handle function */
    /*
    KR_LOG(KR_LOGDEBUG, "invoking handle for method [%s]...", apply->method);
    handle_func(krctx, krarg);
    */

RESP:
    /*TODO: call interface module to genrate reply message */

    /* run user's callback function */
    if (krarg->cb_func) {
        krarg->cb_func(apply, reply, krarg->data);
    }

    /* clean context */
    //kr_context_clean(krctx);
    return 0;
}


int kr_engine_register(T_KREngine *engine, char *method, KRFunc func)
{
    kr_functable_register(engine->env->ptFuncTable, method, func);
    return 0;
}

