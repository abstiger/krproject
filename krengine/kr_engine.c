#include "kr_config.h"
#include "krutils/kr_utils.h"
#include "krutils/kr_threadpool.h"
#include "krutils/kr_cache.h"
#include "krshm/kr_shm.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"
#include "krrule/kr_rule.h"
#include "kr_engine.h"

extern void *kr_engine_thread_init(void *env);
extern int kr_engine_thread_worker(void *ctx, void *arg);
extern void kr_engine_thread_fini(void *ctx);

struct _kr_engine_t
{
    char             *version;      /* version of krengine */
    char             *info;         /* information of krengine */

    int              shmkey;        /* share memory key */
    char             *dbname;       /* name of krdb */
    char             *dbmodulename; /* name of krdb's module */
    int              hdicachesize;  /* hdi cache size */
    int              threadcnt;     /* thread pool size */

    T_KRShareMem     *krshm;        /* share memory address */
    T_KRDB           *krdb;         /* krdb for this server */
    T_KRCache        *krhdicache;   /* cache for hdi */
    T_KRContextEnv   *krctxenv;     /* environment of the context */
    T_KRContext      *krctx;        /* dynamic memory address */
    T_KRThreadPool   *krtp;         /* thread pool */
};



int kr_engine_external_startup(char *logpath, char *logname, int loglevel)
{
    /* Set log file and level */
    if (logpath) kr_log_set_path(logpath);
    if (logname) kr_log_set_name(logname);
    if (loglevel) kr_log_set_level(loglevel);

    /* Connect db */
    if (dbsDbConnect() != 0) {
        KR_LOG(KR_LOGERROR, "dbsDbConnect failed!\n");
        return -1;
    }

    return 0;
}


void kr_engine_external_shutdown(void)
{
    /* disconnect db */
    dbsDbDisconnect();
}


T_KREngine *kr_engine_startup(int shmkey, char *dbname, char *dbmodulename, int hdicachesize, int threadcnt)
{
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

    krengine->shmkey = shmkey;
    krengine->dbname = kr_strdup(dbname);
    krengine->dbmodulename = kr_strdup(dbmodulename);
    krengine->hdicachesize = hdicachesize;
    krengine->threadcnt = threadcnt;


    /* Attach share memory */
    krengine->krshm  = kr_shm_attach(krengine->shmkey);
    if (krengine->krshm == NULL) {
        KR_LOG(KR_LOGERROR, "kr_shm_attach [%d] failed!", krengine->shmkey);
        goto FAILED;
    }

    /* Start up krdb */
    krengine->krdb = kr_db_startup(krengine->dbname, krengine->dbmodulename);
    if (krengine->krdb == NULL) {
        KR_LOG(KR_LOGERROR, "kr_db_startup [%s] [%s] failed!", \
				krengine->dbname, krengine->dbmodulename);
        goto FAILED;
    }

    /* if hdi cache size greater than zero, create the hdi cache */
    if (krengine->hdicachesize > 0) {
        krengine->krhdicache = kr_hdi_cache_create(krengine->hdicachesize);
        if (krengine->krhdicache == NULL) {
            KR_LOG(KR_LOGERROR, "kr_hdi_cache_create [%s] failed!", \
                    krengine->hdicachesize);
            goto FAILED;
        }
    }

    /* create rule detecting environment */
    krengine->krctxenv = kr_calloc(sizeof(T_KRContextEnv));
    if (krengine->krctxenv == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krctxenv failed!");
        goto FAILED;
    }
    krengine->krctxenv->ptShm = krengine->krshm;
    krengine->krctxenv->ptKRDB = krengine->krdb;
    krengine->krctxenv->ptHDICache = krengine->krhdicache;
    
    if (krengine->threadcnt <= 0) {
        /* if no threadpool, initialize rule detecting context */
        krengine->krctx = kr_context_init(krengine->krctxenv);
        if (krengine->krctx == NULL) {
            KR_LOG(KR_LOGERROR, "kr_context_init failed!");
            goto FAILED;
        }
    } else { 
        /* else create and run thread pool */
        krengine->krtp = kr_threadpool_create(
                krengine->threadcnt, krengine->krctxenv, 
                kr_engine_thread_init, 
                kr_engine_thread_worker, 
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
    if (krengine) {
        if (krengine->krctx) kr_context_fini(krengine->krctx);
        if (krengine->krtp) kr_threadpool_destroy(krengine->krtp);
        if (krengine->krctxenv) kr_free(krengine->krctxenv);
        if (krengine->krhdicache) kr_hdi_cache_destroy(krengine->krhdicache);
        if (krengine->krdb) kr_db_shutdown(krengine->krdb);
        if (krengine->krshm) kr_shm_detach(krengine->krshm);
        if (krengine->version) kr_free(krengine->version);
        if (krengine->info) kr_free(krengine->info);
        if (krengine->dbname) kr_free(krengine->dbname);
        if (krengine->dbmodulename) kr_free(krengine->dbmodulename);
        kr_free(krengine);
    }
    return NULL;
}


void kr_engine_shutdown(T_KREngine *krengine)
{
    if (krengine == NULL) return;

    /* destroy rule detecting environment */
    if (krengine->threadcnt <= 0) {
        /* context finalize */
        kr_context_fini(krengine->krctx);
    } else { 
        /* threadpool destroy */
        kr_threadpool_destroy(krengine->krtp);
    }

    /* destroy rule detecting environment */
    if (krengine->krctxenv) kr_free(krengine->krctxenv);
    /* hdi cache destroy */
    if (krengine->krhdicache) kr_hdi_cache_destroy(krengine->krhdicache);
    /* krdb shutdown */
    if (krengine->krdb) kr_db_shutdown(krengine->krdb);
    /* share memory detach */
    if (krengine->krshm) kr_shm_detach(krengine->krshm);
    if (krengine->version) kr_free(krengine->version);
    if (krengine->info) kr_free(krengine->info);
    if (krengine->dbname) kr_free(krengine->dbname);
    if (krengine->dbmodulename) kr_free(krengine->dbmodulename);
    kr_free(krengine);
}



int kr_engine_run(T_KREngine *krengine, E_KROprCode oprcode, int datasrc, char *msgbuf)
{
    int iResult = 0;
    switch(oprcode) 
    {
        case KR_OPRCODE_INSERT:
        {
            /* insert krdb */
            T_KRRecord *ptCurrRec = \
                kr_db_insert(krengine->krdb, datasrc, msgbuf);
            if (ptCurrRec == NULL) {
                KR_LOG(KR_LOGERROR, "kr_db_insert failed!");
                return -1;
            }
            break;
        }
        case KR_OPRCODE_DETECT:
        {
            /* insert krdb */
            T_KRRecord *ptCurrRec = \
                kr_db_insert(krengine->krdb, datasrc, msgbuf);
            if (ptCurrRec == NULL) {
                KR_LOG(KR_LOGERROR, "kr_db_insert failed!");
                return -1;
            }

            /* rule detect */
            if (krengine->threadcnt <= 0) {
                iResult = kr_engine_thread_worker(krengine->krctx, ptCurrRec);
                if (iResult != 0) {
                    KR_LOG(KR_LOGERROR, "kr_server_detect failed!");
                    return -1;
                }
            } else {
                iResult = kr_threadpool_add_task(krengine->krtp, ptCurrRec);
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


