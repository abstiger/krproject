#ifndef __KR_OUTPUT_H__
#define __KR_OUTPUT_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krcalc/kr_calc.h"
#include "krengine/kr_engine.h"

typedef struct _kr_output_t T_KROutput;

T_KROutput* kr_output_construct(T_KRParam *ptParam, char *psOutputModule,
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue);
void kr_output_destruct(T_KROutput* ptOutput);
int kr_output_check(T_KROutput* ptOutput, T_KRParam *ptParam);

T_KRResponse *kr_output_process(T_KROutput *ptOutput, T_KRContext *ptContext);

#endif /* __KR_OUTPUT_H__ */
