#include "kr_config.h"
#include "krutils/kr_utils.h"
#include "krutils/kr_threadpool.h"
#include "krutils/kr_cache.h"
#include "krparam/kr_param.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"
#include "krdata/kr_data.h"
#include "krflow/kr_flow.h"
#include "kr_engine_context.h"
#include "kr_engine.h"
#include "assert.h"

extern int kr_engine_register_default(T_KREngine *engine);

static int kr_engine_handle(void *ctx, void *arg);

/* engine inner declaration */
struct _kr_engine_t
{
    char             *version;      /* version of engine */
    char             *info;         /* information of engine */

    T_KRContextEnv   *ctx_env;      /* environment of the context */
    T_KRContext      *ctx;          /* dynamic memory address */
    T_KRThreadPool   *tp;           /* thread pool */
};

T_KREngine *kr_engine_startup(T_KREngineConfig *cfg, void *data)
{
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

    /* Set log file and level */
    if (cfg->logpath) kr_log_set_path(cfg->logpath);
    if (cfg->logname) kr_log_set_name(cfg->logname);
    if (cfg->loglevel) kr_log_set_level(cfg->loglevel);

    /* set engine's context environment */
    T_KRContextEnv *ctx_env = kr_calloc(sizeof(T_KRContextEnv));
    if (ctx_env == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ctx_env failed!");
        goto FAILED;
    }
    engine->ctx_env = ctx_env;
    ctx_env->extra = data;

    /* load pludge-in modules */
    if (cfg->krdb_module) {
        ctx_env->krdbModule = kr_module_open(cfg->krdb_module, RTLD_LAZY);
        if (ctx_env->krdbModule == NULL) {
            KR_LOG(KR_LOGERROR, "kr_module_open %s failed!", cfg->krdb_module);
            goto FAILED;
        }
    } else {
        KR_LOG(KR_LOGERROR, "krdb-module-file not configured!");
        goto FAILED;
    }

    if (cfg->data_module) {
        ctx_env->dataModule = kr_module_open(cfg->data_module, RTLD_LAZY);
        if (ctx_env->dataModule == NULL) {
            KR_LOG(KR_LOGERROR, "kr_module_open %s failed!", cfg->data_module);
            goto FAILED;
        }
    }

    /* Connect to database */
    if (cfg->dbname) {
        ctx_env->ptDbsEnv = dbsConnect(cfg->dbname, cfg->dbuser, cfg->dbpass);
        if (ctx_env->ptDbsEnv == NULL) {
            KR_LOG(KR_LOGERROR, "dbsConnect [%s] [%s] [%s] failed!",
                    cfg->dbname, cfg->dbuser, cfg->dbpass);
            goto FAILED;
        }
    } else {
        KR_LOG(KR_LOGERROR, "db-name not configured!");
        goto FAILED;
    }

    /* Create parameter memory */
    ctx_env->ptParam = kr_param_create(ctx_env->ptDbsEnv);
    if (ctx_env->ptParam == NULL) {
        KR_LOG(KR_LOGERROR, "kr_param_create failed!");
        goto FAILED;
    }

    /* Start up krdb */
    ctx_env->ptDB = kr_db_new("KRDB", ctx_env->ptDbsEnv, ctx_env->krdbModule);
    if (ctx_env->ptDB == NULL) {
        KR_LOG(KR_LOGERROR, "kr_db_startup failed!");
        goto FAILED;
    }

    /* Create hdi cache */
    if (cfg->hdi_cache_size > 0) {
        ctx_env->ptHDICache = kr_hdi_cache_create(cfg->hdi_cache_size);
        if (ctx_env->ptHDICache == NULL) {
            KR_LOG(KR_LOGERROR, "kr_hdi_cache_create [%d] failed!", \
                    cfg->hdi_cache_size);
            goto FAILED;
        }
    }

    /* Create function table */
    ctx_env->ptFuncTable = kr_functable_create("engine");
    if (ctx_env->ptFuncTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_functable_create engine failed!");
        goto FAILED;
    }
    
    /* initialize engine's context */
    if (cfg->thread_pool_size <= 0) {
        /* if no threadpool, initialize rule detecting context */
        engine->ctx = kr_context_init(engine->ctx_env);
        if (engine->ctx == NULL) {
            KR_LOG(KR_LOGERROR, "kr_context_init failed!");
            goto FAILED;
        }
    } else { 
        /* else create and run thread pool */
        engine->tp = kr_threadpool_create(
                cfg->thread_pool_size, cfg->high_water_mark, ctx_env,
                (KRThdInitFunc )kr_context_init, 
                (KRThdFiniFunc )kr_context_fini);
        if (engine->tp == NULL) {
            KR_LOG(KR_LOGERROR, "kr_threadpool_create failed!");
            goto FAILED;
        }
        /* run thread pool */
        kr_threadpool_run(engine->tp);
    }

    /* register default engine handles */
    if (kr_engine_register_default(engine) != 0) {
        KR_LOG(KR_LOGERROR, "kr_engine_register_default failed!");
        goto FAILED;
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
        kr_context_fini(engine->ctx);
    } else { 
        /* destroy threadpool */
        kr_threadpool_destroy(engine->tp);
    }

    /* destroy rule detecting environment */
    if (engine->ctx_env) {
        T_KRContextEnv *ctx_env=engine->ctx_env;
        if (ctx_env->krdbModule) kr_module_close(ctx_env->krdbModule);
        if (ctx_env->dataModule) kr_module_close(ctx_env->dataModule);
        if (ctx_env->ptDbsEnv) dbsDisconnect(ctx_env->ptDbsEnv);
        if (ctx_env->ptParam) kr_param_destroy(ctx_env->ptParam);
        if (ctx_env->ptDB) kr_db_free(ctx_env->ptDB);
        if (ctx_env->ptHDICache) kr_hdi_cache_destroy(ctx_env->ptHDICache);
        if (ctx_env->ptFuncTable) kr_functable_destroy(ctx_env->ptFuncTable);
        kr_free(engine->ctx_env);
    }

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
    T_KRContext *krctx = (T_KRContext *)ctx;
    T_KREngineArg *krarg = (T_KREngineArg *)arg;
    T_KRMessage *apply = (T_KRMessage *)krarg->apply;
    T_KRMessage *reply = (T_KRMessage *)krarg->reply;

    /* set context with arg */
    if (kr_context_set(krctx, krarg) != 0) {
        KR_LOG(KR_LOGERROR, "kr_context_set [%p] failed!", krarg);
        //FIXME:set error code
        //reply->msgtype = KR_MSGTYPE_ERROR;
        goto RESP;
    }

    /* search handle for this apply message */
    KRFunc handle_func = kr_functable_search(\
            krctx->ptEnv->ptFuncTable, apply->method);
    if (handle_func == NULL) {
        KR_LOG(KR_LOGERROR, "unsupported method[%d]!", apply->method);
        //FIXME:set error code
        //reply->msgtype = KR_MSGTYPE_ERROR;
        goto RESP;
    }

    /* excute handle function */
    KR_LOG(KR_LOGDEBUG, "invoking handle for method [%s]...", apply->method);
    handle_func(krctx, krarg);

RESP:
    /*TODO: call interface module to construct reply message */

    /* run user's callback function */
    if (krarg->cb_func) {
        krarg->cb_func(apply, reply, krarg->data);
    }

    /* clean context */
    kr_context_clean(krctx);
    return 0;
}


int kr_engine_register(T_KREngine *engine, char *method, KRFunc func)
{
    kr_functable_register(engine->ctx_env->ptFuncTable, method, func);
    return 0;
}

