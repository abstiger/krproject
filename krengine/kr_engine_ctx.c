#include "kr_engine_ctx.h"

/* engine context */
T_KREngineCtx *kr_engine_ctx_create(T_KREngineEnv *ptEnv)
{
    T_KREngineCtx *ptCtx = kr_calloc(sizeof(*ptCtx));
    if (ptCtx == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptCtx failed!");
        return NULL;
    }
    
    ptCtx->ptParam = ptEnv->ptParam;
    ptCtx->ptDB = ptEnv->ptDB;
    
    //FIME: remove krdbModule
    ptCtx->ptInput = kr_input_construct(ptEnv->ptParam, ptEnv->krdbModule);
    if (ptCtx->ptData == NULL) {
        KR_LOG(KR_LOGERROR, "kr_input_construct failed!");
        return NULL;
    }
    
    //FIME: remove krdbModule
    ptCtx->ptOutput = kr_output_construct(ptEnv->ptParam, ptEnv->krdbModule);
    if (ptCtx->ptData == NULL) {
        KR_LOG(KR_LOGERROR, "kr_output_construct failed!");
        return NULL;
    }
    
    //FIME: remove krdbModule
    ptCtx->ptData = kr_data_construct(ptEnv->ptParam, ptEnv->krdbModule);
    if (ptCtx->ptData == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_construct failed!");
        return NULL;
    }
    /*TODO: construct flow */
    /*
    ptCtx->ptFlow = kr_flow_construct(ptEnv->ptParam);
    if (ptCtx->ptFlow == NULL) {
        KR_LOG(KR_LOGERROR, "kr_flow_construct failed!");
        return NULL;
    }
    */
    
    return ptCtx;
}

void kr_engine_ctx_destroy(T_KREngineCtx *ptCtx)
{
    if (ptCtx) {
        kr_input_destruct(ptCtx->ptInput);
        kr_output_destruct(ptCtx->ptOutput);
        kr_data_destruct(ptCtx->ptData);
        //kr_flow_destruct(ptCtx->ptFlow);
        kr_free(ptCtx);
    }
}

int kr_engine_ctx_set(T_KREngineCtx *ptCtx, T_KREngineArg *ptArg)
{
    /* set argument */
    ptCtx->ptArg = ptArg;

    /* check input, reload if needed */
    if (kr_input_check(ptCtx->ptInput, ptCtx->ptParam) != 0) {
        KR_LOG(KR_LOGERROR, "kr_input_check failed");
        return -1;
    }
    
    /* check output, reload if needed */
    if (kr_output_check(ptCtx->ptOutput, ptCtx->ptParam) != 0) {
        KR_LOG(KR_LOGERROR, "kr_output_check failed");
        return -1;
    }
    
    /* check data, reload if needed */
    if (kr_data_check(ptCtx->ptData, ptCtx->ptParam) != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_check failed");
        return -1;
    }
    
    /* check flow, reload if needed */
    /*
    if (kr_flow_check(ptCtx->ptFlow, ptCtx->ptParam) != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_check failed");
        return -1;
    }
    */

    return 0;
}

void kr_engine_ctx_clean(T_KREngineCtx *ptCtx)
{
    /*initialize dynamic data memory*/
    //kr_data_init(ptCtx->ptData);

    /*initialize dynamic flow memory*/
    //kr_flow_init(ptCtx->ptFlow);

    /*initialize others*/
    //ptCtx->ptArg = NULL;
    //ptCtx->ptCurrRec = NULL;
}
