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

    ptContext->ptDym = kr_dynamicmem_init(ptEnv->ptShm);
    if (ptContext->ptDym == NULL) {
        KR_LOG(KR_LOGERROR, "kr_dynamicmem_init failed!");
        kr_free(ptContext);
        return NULL;
    }
    
    return ptContext;
}


int kr_context_set(T_KRContext *ptContext, T_KRRecord *ptCurrRec)
{
    /* check dynamic memory, reload if needed */
    if (kr_dynamicmem_check(ptContext->ptDym, ptContext->ptEnv->ptShm) != 0) {
        KR_LOG(KR_LOGERROR, "kr_dynamicmem_check failed");
        return -1;
    }

    /* record comes from kr_db_insert() */
    ptContext->ptCurrRec = ptCurrRec;

    return 0;
}


void kr_context_fini(T_KRContext *ptContext)
{
    if (ptContext) {
        kr_dynamicmem_fini(ptContext->ptDym);
        kr_free(ptContext);
    }
}
