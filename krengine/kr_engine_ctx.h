#ifndef __KR_ENGINE_CTX_H__
#define __KR_ENGINE_CTX_H__

#include "kr_engine_env.h"

/* engine context, create with engine environment */
typedef struct _kr_engine_ctx_t
{
    T_KRParam        *ptParam;     /* parameter, read only in thread */
    T_KRIO           *ptIO;        /* interface module */
    T_KRDB           *ptDB;        /* krdb, read only in thread */

    T_KRData         *ptData;
    T_KRFlow         *ptFlow;

    T_KREngineArg    *ptArg;
}T_KREngineCtx;


T_KREngineCtx *kr_engine_ctx_create(T_KREngineEnv *ptEnv);
void kr_engine_ctx_destroy(T_KREngineCtx *ptEngineCtx);

#endif /* __KR_ENGINE_CTX_H__ */
