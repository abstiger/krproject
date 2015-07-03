#ifndef __KR_INPUT_H__
#define __KR_INPUT_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krengine/kr_engine.h"

/*record*/
typedef struct _kr_record_t
{
    KRFreeFunc       pfFree;
    int              iInputId;
    char             *pRecBuf;
}T_KRRecord;

typedef struct _kr_input_t
{
    pthread_mutex_t  tLock;
    T_KRList         *ptInputDefineList;
    time_t           tConstructTime;
}T_KRInput;


extern T_KRInput* kr_input_construct(T_KRParam *ptParam, T_KRModule *ptInputModule);
extern void kr_input_destruct(T_KRInput* ptInput);
extern int kr_input_check(T_KRInput* ptInput, T_KRParam *ptParam);
extern T_KRRecord *kr_input_process(T_KRInput *ptInput, T_KRMessage *ptMessage);

#endif /* __KR_INPUT_H__ */
