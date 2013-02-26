#include "krutils/kr_utils.h"
#include "krutils/kr_threadpool.h"
#include "krshm/kr_shm.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"
#include "krrule/kr_rule.h"
#include "dbs/dbs_basopr.h"
#include "kr_engine.h"

void *kr_engine_thread_init(void *env)
{
    /* intialize rule detecting context */
    T_KRContextEnv *krctxenv = kr_calloc(sizeof(T_KRContextEnv));
    if (krctxenv == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc krctxenv failed!");
        return NULL;
    }

    /* reconnect database in thread */
    T_DbsEnv *dbsenv = ((T_KRContextEnv *)env)->ptDbs;
    krctxenv->ptDbs = dbsConnect(
            dbsenv->dsn, dbsenv->user, dbsenv->pass);
    if (krctxenv->ptDbs == NULL) {
        KR_LOG(KR_LOGERROR, "dbsConnect [%s] [%s] [%s] in thread failed!\n",
                dbsenv->dsn, dbsenv->user, dbsenv->pass);
        kr_free(krctxenv);
        return NULL;
    }
    krctxenv->ptShm = ((T_KRContextEnv *)env)->ptShm;
    krctxenv->ptKRDB = ((T_KRContextEnv *)env)->ptKRDB;
    krctxenv->ptHDICache = ((T_KRContextEnv *)env)->ptHDICache;

    T_KRContext *krcontext = kr_context_init(krctxenv);
    if (krcontext == NULL) {
        KR_LOG(KR_LOGERROR, "kr_context_init failed!\n");
        dbsDisconnect(krctxenv->ptDbs);
        kr_free(krctxenv);
        return NULL;
    }
    
    return krcontext;
}


void kr_engine_thread_fini(void *ctx)
{
    /* finalize rule detecting context */
    T_KRContext *krcontext = (T_KRContext *)ctx;
    /* disconnect db */
    dbsDisconnect(krcontext->ptEnv->ptDbs);
    /* free context environment */
    kr_free(krcontext->ptEnv);
    /* finalize context */
    kr_context_fini(krcontext);
}
