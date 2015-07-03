#ifndef __KR_FLOW_H__
#define __KR_FLOW_H__

#include "krdata/kr_data.h"

typedef struct _kr_flow_t
{
    T_KRParam        *ptParam;
    T_KRData         *ptData;

    T_KRRecord       *ptCurrRec;
}T_KRFlow;


extern int kr_flow_detect(T_KRFlow *ptFlow, T_KRRecord *ptCurrRec);

#endif /* __KR_FLOW_H__ */

