#include "kr_config.h"
#include "krutils/kr_utils.h"
#include "krutils/kr_json.h"
#include "krutils/kr_threadpool.h"
#include "krutils/kr_cache.h"
#include "krshm/kr_shm.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"
#include "krrule/kr_rule.h"
#include "kr_engine.h"

extern void *kr_engine_thread_init(void *env);
extern void kr_engine_thread_fini(void *ctx);

static int kr_engine_worker(void *ctx, void *arg);

struct _kr_engine_t
{
    char             *version;      /* version of krengine */
    char             *info;         /* information of krengine */

    int              shmkey;        /* share memory key */
    char             *krdbname;     /* name of krdb */
    char             *krdbmodule;   /* name of krdb's module */
    char             *datamodule;   /* name of data's module */
    char             *rulemodule;   /* name of rule's module */
    int              hdicachesize;  /* hdi cache size */
    int              threadcnt;     /* thread pool size */
    int              hwm;           /* thread pool high water mark */
    void             *eventloop;    /* event loop */

    T_KRContextEnv   *krctxenv;     /* environment of the context */
    T_KRContext      *krctx;        /* dynamic memory address */
    T_KRThreadPool   *krtp;         /* thread pool */
};

T_KREngine *kr_engine_startup(
        char *dsn, char *user, char *pass,
        char *logpath, char *logname, int loglevel,
        int shmkey, char *krdbname, char *krdbmodule, 
        char *datamodule, char *rulemodule, 
        int hdicachesize, int threadcnt, int hwm, 
        void *eventloop)
{
    /* Set log file and level */
    if (logpath) kr_log_set_path(logpath);
    if (logname) kr_log_set_name(logname);
    if (loglevel) kr_log_set_level(loglevel);

    /* Init cJSON hooks */
    cJSON_Hooks cjsonhooks = {kr_malloc, kr_free};
    cJSON_InitHooks(&cjsonhooks);

    T_KREngine *krengine = kr_calloc(sizeof(T_KREngine));
    if (krengine == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krengine failed!");
        goto FAILED;
    }
    
    char caInfo[1024] = {0};
    krengine->version = kr_strdup(VERSION);
    snprintf(caInfo, sizeof(caInfo), \
            "Welcome to %s !\n", 
            "This is Version %s.\n",
            "If you find any bug or you'd like an enhancement,\n",
            "Please feel free to contact me: %s \n",
            "Check %s for details.\n",
            PACKAGE, PACKAGE_VERSION, PACKAGE_BUGREPORT, PACKAGE_URL);
    krengine->info = kr_strdup(caInfo);

    /* set engine's context environment */
    T_KRContextEnv *krctxenv = kr_calloc(sizeof(T_KRContextEnv));
    if (krctxenv == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krctxenv failed!");
        goto FAILED;
    }
    krengine->krctxenv = krctxenv;
    krctxenv->krEventLoop = eventloop;

    /* load pludge-in modules */
    if (krdbmodule) {
        krengine->krdbmodule = kr_strdup(krdbmodule);
        krctxenv->krdbModule = kr_module_open(krdbmodule, RTLD_LAZY);
        if (krctxenv->krdbModule == NULL) {
            KR_LOG(KR_LOGERROR, "kr_module_open %s failed!\n", krdbmodule);
            goto FAILED;
        }
    } else {
        KR_LOG(KR_LOGERROR, "krdb-module-file not configured!\n");
        goto FAILED;
    }


    if (datamodule) {
        krengine->datamodule = kr_strdup(datamodule);
        krctxenv->dataModule = kr_module_open(datamodule, RTLD_LAZY);
        if (krctxenv->dataModule == NULL) {
            KR_LOG(KR_LOGERROR, "kr_module_open %s failed!\n", datamodule);
            goto FAILED;
        }
    }

    if (rulemodule) {
        krengine->rulemodule= kr_strdup(rulemodule);
        krctxenv->ruleModule = kr_module_open(rulemodule, RTLD_LAZY);
        if (krctxenv->ruleModule == NULL) {
            KR_LOG(KR_LOGERROR, "kr_module_open %s failed!\n", rulemodule);
            goto FAILED;
        }
    }
    
    /* Connect to database */
    if (dsn) {
        krctxenv->ptDbs = dbsConnect(dsn, user, pass);
        if (krctxenv->ptDbs == NULL) {
            KR_LOG(KR_LOGERROR, "dbsConnect [%s] [%s] [%s] failed!\n",
                    dsn, user, pass);
            goto FAILED;
        }
    } else {
        KR_LOG(KR_LOGERROR, "db-name not configured!\n");
        goto FAILED;
    }

    /* Attach share memory */
    if (shmkey) {
        krengine->shmkey = shmkey;
        krctxenv->ptShm  = kr_shm_attach(krengine->shmkey);
        if (krctxenv->ptShm == NULL) {
            KR_LOG(KR_LOGERROR, "kr_shm_attach [%d] failed!", krengine->shmkey);
            goto FAILED;
        }
    } else {
        KR_LOG(KR_LOGERROR, "shm-key not configured!\n");
        goto FAILED;
    }

    /* Start up krdb */
    if (krdbname) {
        krengine->krdbname = kr_strdup(krdbname);
        krctxenv->ptKRDB = kr_db_startup(
                krctxenv->ptDbs, krengine->krdbname, krctxenv->krdbModule);
        if (krctxenv->ptKRDB == NULL) {
            KR_LOG(KR_LOGERROR, "kr_db_startup [%s] [%s] failed!", \
                    krengine->krdbname, krengine->krdbmodule);
            goto FAILED;
        }
    } else {
        KR_LOG(KR_LOGERROR, "krdb-name not configured!\n");
        goto FAILED;
    }

    /* Create hdi cache */
    if (hdicachesize > 0) {
        krengine->hdicachesize = hdicachesize;
        krctxenv->ptHDICache = kr_hdi_cache_create(hdicachesize);
        if (krctxenv->ptHDICache == NULL) {
            KR_LOG(KR_LOGERROR, "kr_hdi_cache_create [%d] failed!", \
                    hdicachesize);
            goto FAILED;
        }
    }
    
    /* initialize krengine's context */
    if (threadcnt <= 0) {
        /* if no threadpool, initialize rule detecting context */
        krengine->krctx = kr_context_init(krengine->krctxenv);
        if (krengine->krctx == NULL) {
            KR_LOG(KR_LOGERROR, "kr_context_init failed!");
            goto FAILED;
        }
    } else { 
        /* else create and run thread pool */
        krengine->threadcnt = threadcnt;
        krengine->hwm = hwm;
        krengine->krtp = kr_threadpool_create(
                krengine->threadcnt, krengine->hwm, krengine->krctxenv, 
                kr_engine_thread_init, 
                kr_engine_worker, 
                kr_engine_thread_fini);
        if (krengine->krtp == NULL) {
            KR_LOG(KR_LOGERROR, "kr_threadpool_create failed!");
            goto FAILED;
        }
        /* run thread pool */
        kr_threadpool_run(krengine->krtp);
    }
    return krengine;

FAILED:
    kr_engine_shutdown(krengine);
    return NULL;
}


void kr_engine_shutdown(T_KREngine *krengine)
{
    if (krengine == NULL) return;

    /* destroy rule detecting context */
    if (krengine->threadcnt <= 0) {
        /* context finalize */
        kr_context_fini(krengine->krctx);
    } else { 
        /* threadpool destroy */
        kr_threadpool_destroy(krengine->krtp);
    }

    /* destroy rule detecting environment */
    if (krengine->krctxenv) {
        T_KRContextEnv *krctxenv=krengine->krctxenv;
        if (krctxenv->krdbModule) kr_module_close(krctxenv->krdbModule);
        if (krctxenv->dataModule) kr_module_close(krctxenv->dataModule);
        if (krctxenv->ruleModule) kr_module_close(krctxenv->ruleModule);
        if (krctxenv->ptDbs) dbsDisconnect(krctxenv->ptDbs);
        if (krctxenv->ptShm) kr_shm_detach(krctxenv->ptShm);
        if (krctxenv->ptKRDB) kr_db_shutdown(krctxenv->ptKRDB);
        if (krctxenv->ptHDICache) kr_hdi_cache_destroy(krctxenv->ptHDICache);
        kr_free(krengine->krctxenv);
    }
    if (krengine->version) kr_free(krengine->version);
    if (krengine->info) kr_free(krengine->info);
    if (krengine->krdbname) kr_free(krengine->krdbname);
    if (krengine->krdbmodule) kr_free(krengine->krdbmodule);
    if (krengine->datamodule) kr_free(krengine->datamodule);
    if (krengine->rulemodule) kr_free(krengine->rulemodule);
    kr_free(krengine);
}


int kr_engine_worker(void *ctx, void *arg)
{
    T_KRContext *krcontext = (T_KRContext *)ctx;
    T_KRContextArg *krarg = (T_KRContextArg *)arg;

    /* set context with arg */
    if (kr_context_set(krcontext, krarg) != 0) {
        KR_LOG(KR_LOGERROR, "kr_context_set failed!");
        return -1;
    }

    /* group list route and rule group detect*/
    if (kr_group_list_detect(krcontext) != 0) {
        KR_LOG(KR_LOGERROR, "kr_group_list_detect failed!");
        return -1;
    }

    /* clean arguments' user extra data*/
    if (krarg->ExtraFreeFunc) krarg->ExtraFreeFunc(krarg->pExtra);

    return 0;
}


int kr_engine_run(T_KREngine *krengine, E_KROprCode oprcode, int datasrc, char *msgbuf, void *extra)
{
    int iResult = 0;
    T_KRContextArg stArg = {NULL, extra, NULL};

    switch(oprcode) 
    {
        case KR_OPRCODE_INSERT:
        {
            /* insert krdb */
            stArg.ptCurrRec = \
                kr_db_insert(krengine->krctxenv->ptKRDB, datasrc, msgbuf);
            if (stArg.ptCurrRec == NULL) {
                KR_LOG(KR_LOGERROR, "kr_db_insert failed!");
                return -1;
            }
            break;
        }
        case KR_OPRCODE_DETECT:
        {
            /* insert krdb */
            stArg.ptCurrRec = \
                kr_db_insert(krengine->krctxenv->ptKRDB, datasrc, msgbuf);
            if (stArg.ptCurrRec == NULL) {
                KR_LOG(KR_LOGERROR, "kr_db_insert failed!");
                return -1;
            }

            /* rule detect */
            if (krengine->threadcnt <= 0) {
                iResult = kr_engine_worker(krengine->krctx, &stArg);
                if (iResult != 0) {
                    KR_LOG(KR_LOGERROR, "kr_server_detect failed!");
                    return -1;
                }
            } else {
                iResult = kr_threadpool_add_task(krengine->krtp, &stArg);
                if (iResult != 0) {
                    KR_LOG(KR_LOGERROR, "kr_threadpool_add_task failed!");
                    return -1;
                }
            }
            break;
        }
        default:
        {
            KR_LOG(KR_LOGERROR, "Unsupported oprcode:[%d]!", oprcode);
            return -1;
        }
    }

    return 0;
}


char *kr_engine_info(T_KREngine *krengine)
{
    if (krengine == NULL) return NULL;

    return krengine->info;
}


