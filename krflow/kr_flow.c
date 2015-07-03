#include "kr_flow.h"


int kr_flow_detect(T_KRFlow *ptFlow, T_KRRecord *ptCurrRec)
{
    ptFlow->ptData->ptCurrRec = ptCurrRec;
    
    return 0;
}

