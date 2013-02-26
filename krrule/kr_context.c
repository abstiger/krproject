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

    ptContext->ptArg = kr_calloc(sizeof(T_KRContextArg));
    if (ptContext == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptContext->ptArg failed!");
        kr_free(ptContext);
        return NULL;
    }

    ptContext->ptDym = kr_dynamicmem_init(ptEnv);
    if (ptContext->ptDym == NULL) {
        KR_LOG(KR_LOGERROR, "kr_dynamicmem_init failed!");
        kr_free(ptContext->ptArg);
        kr_free(ptContext);
        return NULL;
    }
    
    return ptContext;
}


int kr_context_set(T_KRContext *ptContext, T_KRContextArg *ptContextArg)
{
    /* check dynamic memory, reload if needed */
    if (kr_dynamicmem_check(ptContext->ptDym, ptContext->ptEnv) != 0) {
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
        kr_free(ptContext);
    }
}
