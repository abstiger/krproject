#include "kr_flow.h"

T_KRFlow *kr_flow_construct(T_KRParam *ptParam, T_KRModule *ptModule)
{
    //TODO:this is a dummy now
    T_KRFlow *ptFlow = kr_calloc(sizeof(*ptFlow));
    if (ptFlow == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptFlow failed!");
        return NULL;
    }

    ptFlow->tConstructTime = kr_param_load_time(ptParam);
    return ptFlow;
}

void kr_flow_destruct(T_KRFlow *ptFlow)
{
    if (ptFlow) {
        kr_free(ptFlow);
    }
}

void kr_flow_init(T_KRFlow *ptFlow)
{
    //TODO:this is a dummy now

}

int kr_flow_check(T_KRFlow *ptFlow, T_KRParam *ptParam)
{
    /*check item table*/
    if (ptFlow->tConstructTime != kr_param_load_time(ptParam)) {
        KR_LOG(KR_LOGDEBUG, "reload ...[%ld][%ld]", 
                ptFlow->tConstructTime, kr_param_load_time(ptParam));

        //FIXME:

    }

    return 0;

}

int kr_flow_process(T_KRFlow *ptFlow, T_KRContext *ptContext)
{
    //TODO:this is a dummy now
    
    return 0;
}

