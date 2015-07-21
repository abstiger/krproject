#include "kr_engine_ctx.h"

/* engine context */
T_KRContext *kr_engine_ctx_create(T_KREngineEnv *ptEnv)
{
    T_KRContext *ptCtx = kr_context_create(ptEnv);
    if (ptCtx == NULL) {
        KR_LOG(KR_LOGERROR, "kr_context_create ptCtx failed!");
        return NULL;
    }

    kr_context_add_data(ptCtx, "param", ptEnv->ptParam);
    kr_context_add_data(ptCtx, "input", ptEnv->ptInput);
    kr_context_add_data(ptCtx, "output", ptEnv->ptOutput);
    kr_context_add_data(ptCtx, "db", ptEnv->ptDB);
    
    //TODO: construct data
    /*
    ptCtx->ptData = kr_data_construct(ptEnv->ptParam, ptEnv->krdbModule);
    if (ptCtx->ptData == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_construct failed!");
        return NULL;
    }
    */
    //TODO: construct flow
    /*
    ptCtx->ptFlow = kr_flow_construct(ptEnv->ptParam);
    if (ptCtx->ptFlow == NULL) {
        KR_LOG(KR_LOGERROR, "kr_flow_construct failed!");
        return NULL;
    }
    */
    
    return ptCtx;
}

void kr_engine_ctx_destroy(T_KRContext *ptCtx)
{
    if (ptCtx) {
        kr_context_destroy(ptCtx);
    }
}

int kr_engine_ctx_check(T_KRContext *ptCtx)
{
    /* check input, reload if needed */
    /*
    if (kr_input_check(ptCtx->ptInput, ptCtx->ptParam) != 0) {
        KR_LOG(KR_LOGERROR, "kr_input_check failed");
        return -1;
    }
    */
    
    /* check output, reload if needed */
    /*
    if (kr_output_check(ptCtx->ptOutput, ptCtx->ptParam) != 0) {
        KR_LOG(KR_LOGERROR, "kr_output_check failed");
        return -1;
    }
    */
    
    /* check data, reload if needed */
    /*
    if (kr_data_check(ptCtx->ptData, ptCtx->ptParam) != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_check failed");
        return -1;
    }
    */
    
    /* check flow, reload if needed */
    /*
    if (kr_flow_check(ptCtx->ptFlow, ptCtx->ptParam) != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_check failed");
        return -1;
    }
    */

    return 0;
}

void kr_engine_ctx_clean(T_KRContext *ptCtx)
{
    /*initialize dynamic data memory*/
    //kr_data_init(ptCtx->ptData);

    /*initialize dynamic flow memory*/
    //kr_flow_init(ptCtx->ptFlow);

    /*initialize others*/
    //ptCtx->ptArg = NULL;
    //ptCtx->ptCurrRec = NULL;
}


int kr_engine_ctx_process(T_KRContext *ptCtx, T_KREngineArg *ptArg)
{
    T_KRMessage *apply = (T_KRMessage *)ptArg->apply;
    T_KRMessage *reply = (T_KRMessage *)ptArg->reply;

    /* set argument */
    kr_context_add_data(ptCtx, "arg", ptArg);

    //check context, reload if parameter changed
    if (kr_engine_ctx_check(ptCtx) != 0) {
        KR_LOG(KR_LOGERROR, "kr_engine_ctx_check failed!");
        //FIXME:set error code
        //reply->msgtype = KR_MSGTYPE_ERROR;
        goto RESP;
    }
    
    /*
    T_KRInput *ptInput = kr_context_get_data(ptCtx, "input");
    if (ptInput == NULL) {
        KR_LOG(KR_LOGERROR, "kr_context_get_data input failed!");
        //FIXME:set error code
        //reply->msgtype = KR_MSGTYPE_ERROR;
        goto RESP;
    }

    T_KROutput *ptOutput = kr_context_get_data(ptCtx, "output");
    if (ptOutput == NULL) {
        KR_LOG(KR_LOGERROR, "kr_context_get_data output failed!");
        //FIXME:set error code
        //reply->msgtype = KR_MSGTYPE_ERROR;
        goto RESP;
    }

    //call input module to process apply message
    T_KRRecord *ptCurrRec = kr_input_process(ptInput, apply);
    if (ptCurrRec == NULL) {
        KR_LOG(KR_LOGERROR, "kr_input_process failed!");
        //FIXME:set error code
        //reply->msgtype = KR_MSGTYPE_ERROR;
        goto RESP;
    }

    // set current record
    kr_context_add_data(ptCtx, "curr_rec", ptCurrRec);
    
    //call output module to genrate reply message
    reply = kr_output_process(ptOutput, ptCtx);
    if (reply == NULL) {
        KR_LOG(KR_LOGERROR, "kr_output_process failed!");
        return -1;
    }
    */

    /* invoke flow */
    T_KRFlow *ptFlow = kr_context_get_data(ptCtx, "flow");
    if (ptFlow == NULL) {
        KR_LOG(KR_LOGERROR, "kr_context_get_data flow failed!");
        //FIXME:set error code
        //reply->msgtype = KR_MSGTYPE_ERROR;
        goto RESP;
    }
    
    if (kr_flow_process(ptFlow, ptCtx) != 0) {
        KR_LOG(KR_LOGERROR, "kr_flow_process failed!");
        //FIXME:set error code
        //reply->msgtype = KR_MSGTYPE_ERROR;
        goto RESP;
    }

RESP:
    /* run user's callback function */
    if (ptArg->cb_func) {
        ptArg->cb_func(apply, reply, ptArg->data);
    }

    /* clean context */
    kr_engine_ctx_clean(ptCtx);

    return 0;
}


E_KRType kr_engine_ctx_get_type(char kind, int id, void *data)
{
    T_KRContext *ptCtx = (T_KRContext *)data;
    switch(kind) 
    {
        case KR_CALCKIND_CID:
            {
                T_KRRecord *ptCurrRec = 
                    (T_KRRecord *)kr_context_get_data(ptCtx, "curr_rec");
                return kr_record_get_field_type(ptCurrRec, id);
            }
        case KR_CALCKIND_FID:
            {
                T_KRRecord *ptTravRec = 
                    (T_KRRecord *)kr_context_get_data(ptCtx, "trav_rec");
                return kr_record_get_field_type(ptTravRec, id);
            }
        case KR_CALCKIND_SET:
        case KR_CALCKIND_SID:
        case KR_CALCKIND_DID:
        case KR_CALCKIND_HID:
            {
                T_KRData *ptData = 
                    (T_KRData *)kr_context_get_data(ptCtx, "data");
                return kr_data_get_type(ptData, kind, id, ptCtx);
            }
        default:
            return KR_TYPE_UNKNOWN;
    }
}

void *kr_engine_ctx_get_value(char kind, int id, void *data)
{
    T_KRContext *ptCtx = (T_KRContext *)data;
    switch(kind) 
    {
        case KR_CALCKIND_CID:
            {
                T_KRRecord *ptCurrRec = 
                    (T_KRRecord *)kr_context_get_data(ptCtx, "curr_rec");
                return kr_record_get_field_value(ptCurrRec, id);
            }
        case KR_CALCKIND_FID:
            {
                T_KRRecord *ptTravRec = 
                    (T_KRRecord *)kr_context_get_data(ptCtx, "trav_rec");
                return kr_record_get_field_value(ptTravRec, id);
            }
        case KR_CALCKIND_SET:
        case KR_CALCKIND_SID:
        case KR_CALCKIND_DID:
        case KR_CALCKIND_HID:
            {
                T_KRData *ptData = 
                    (T_KRData *)kr_context_get_data(ptCtx, "data");
                return kr_data_get_value(ptData, kind, id, ptCtx);
            }
        default:
            return NULL;
    }
}
