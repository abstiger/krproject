#include "kr_context.h"

/* initialize rule detecting context */
T_KRContext *kr_context_init(T_KRContextEnv *ptEnv)
{
    T_KRContext *ptContext = kr_calloc(sizeof(T_KRContext));
    if (ptContext == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptContext failed!");
        return NULL;
    }

    ptContext->ptEnv = ptEnv;

    /* reconnect database in thread */
    T_DbsEnv *dbsenv = ((T_KRContextEnv *)ptEnv)->ptDbs;
    ptContext->ptDbs = dbsConnect(dbsenv->dsn, dbsenv->user, dbsenv->pass);
    if (ptContext->ptDbs == NULL) {
        KR_LOG(KR_LOGERROR, "dbsConnect [%s] [%s] [%s] failed!", \
                dbsenv->dsn, dbsenv->user, dbsenv->pass);
        kr_free(ptContext);
        return NULL;
    }

    ptContext->ptArg = kr_calloc(sizeof(T_KRContextArg));
    if (ptContext->ptArg == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptContext->ptArg failed!");
        dbsDisconnect(ptContext->ptDbs);
        kr_free(ptContext);
        return NULL;
    }

    ptContext->ptDym = kr_dynamicmem_init(ptEnv->ptShm, \
            ptEnv->dataModule, ptEnv->ruleModule);
    if (ptContext->ptDym == NULL) {
        KR_LOG(KR_LOGERROR, "kr_dynamicmem_init failed!");
        dbsDisconnect(ptContext->ptDbs);
        kr_free(ptContext->ptArg);
        kr_free(ptContext);
        return NULL;
    }
    
    return ptContext;
}


int kr_context_set(T_KRContext *ptContext, T_KRContextArg *ptContextArg)
{
    T_KRContextEnv *ptEnv = ptContext->ptEnv;
    /* check dynamic memory, reload if needed */
    if (kr_dynamicmem_check(ptContext->ptDym, ptEnv->ptShm, \
                ptEnv->dataModule, ptEnv->ruleModule) != 0) {
        KR_LOG(KR_LOGERROR, "kr_dynamicmem_check failed");
        return -1;
    }

    /* deep copy arguments */
    memcpy(ptContext->ptArg, ptContextArg, sizeof(T_KRContextArg));

    /* set current record */
    ptContext->ptCurrRec = ptContextArg->ptCurrRec;

    return 0;
}


void kr_context_fini(T_KRContext *ptContext)
{
    if (ptContext) {
        kr_dynamicmem_fini(ptContext->ptDym);
        if (ptContext->ptArg) kr_free(ptContext->ptArg);
        dbsDisconnect(ptContext->ptDbs);
        kr_free(ptContext);
    }
}
