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
    T_KRContext         *ctx;          /* context of this engine */
    T_KRThreadPool      *tp;           /* thread pool */
};

T_KRRequest *kr_request_alloc(void)
{
    /*need calloc here*/
    T_KRRequest *ptRequest = kr_calloc(sizeof(*ptRequest));
    if (ptRequest == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptRequest failed!");
        return NULL;
    }
    return ptRequest;
}


void kr_request_free(T_KRRequest *ptRequest)
{
    if (ptRequest) {
        if (ptRequest->msgbuf) kr_free(ptRequest->msgbuf);
        kr_free(ptRequest); 
    }
}


T_KRResponse *kr_response_alloc(void)
{
    /*need calloc here*/
    T_KRResponse *ptResponse = kr_calloc(sizeof(*ptResponse));
    if (ptResponse == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptResponse failed!");
        return NULL;
    }
    return ptResponse;
}


void kr_response_free(T_KRResponse *ptResponse)
{
    if (ptResponse) {
        if (ptResponse->msgbuf) kr_free(ptResponse->msgbuf);
        kr_free(ptResponse); 
    }
}

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
    
    if (engine) {
        /* destroy context */
        if (engine->ctx) kr_engine_ctx_destroy(engine->ctx);
        /* destroy threadpool */
        if (engine->tp) kr_threadpool_destroy(engine->tp);
        /* destroy environment */
        if (engine->env) kr_engine_env_destroy(engine->env);
        if (engine->version) kr_free(engine->version);
        if (engine->info) kr_free(engine->info);
        kr_free(engine);
    }
}


int kr_engine_run(T_KREngine *engine, T_KREngineArg *arg)
{
    if (engine->ctx) {
        if (kr_engine_ctx_process(engine->ctx, arg) != 0) {
            KR_LOG(KR_LOGERROR, "kr_engine_ctx_process failed!");
            return -1;
        }
    } else {
        T_KRThreadPoolTask stTask = \
            {(KRThdTaskFunc )kr_engine_ctx_process, arg, sizeof(*arg)};
        if (kr_threadpool_add_task(engine->tp, &stTask) != 0) {
            KR_LOG(KR_LOGERROR, "kr_threadpool_add_task failed!");
            return -1;
        }
    }

    return 0;
}


int kr_engine_register(T_KREngine *engine, char *method, KRFunc func)
{
    kr_functable_register(engine->env->ptFuncTable, method, func);
    return 0;
}

