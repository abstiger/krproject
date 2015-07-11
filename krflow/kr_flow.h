#ifndef __KR_FLOW_H__
#define __KR_FLOW_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krcalc/kr_calc.h"

typedef struct _kr_flow_t
{
    T_KRModule        *ptModule;

    time_t            tConstructTime;
}T_KRFlow;


T_KRFlow *kr_flow_construct(T_KRParam *ptParam, T_KRModule *ptModule);
void kr_flow_destruct(T_KRFlow *ptFlow);
void kr_flow_init(T_KRFlow *ptFlow);
int kr_flow_check(T_KRFlow *ptFlow, T_KRParam *ptParam);

int kr_flow_process(T_KRFlow *ptFlow, T_KRContext *ptContext);

#endif /* __KR_FLOW_H__ */

