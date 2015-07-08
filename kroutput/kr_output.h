#ifndef __KR_OUTPUT_H__
#define __KR_OUTPUT_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krengine/kr_engine.h"

typedef struct _kr_record_t T_KRRecord;
typedef struct _kr_output_handle_t T_KROutputHandle;
typedef struct _kr_output_define_t T_KROutputDefine;

 
typedef struct _kr_output_t
{
    T_KRModule       *ptOutputModule;
    T_KRList         *ptOutputDefineList;
    time_t           tConstructTime;
}T_KROutput;


extern T_KROutput* kr_output_construct(T_KRParam *ptParam, T_KRModule *ptOutputModule);
extern void kr_output_destruct(T_KROutput* ptOutput);
extern int kr_output_check(T_KROutput* ptOutput, T_KRParam *ptParam);
extern T_KRRecord *kr_output_process(T_KROutput *ptOutput, T_KRMessage *ptMessage);

#endif /* __KR_OUTPUT_H__ */
