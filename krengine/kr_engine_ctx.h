#ifndef __KR_ENGINE_CTX_H__
#define __KR_ENGINE_CTX_H__

#include "kr_engine_env.h"

T_KRContext *kr_engine_ctx_create(T_KREngineEnv *ptEnv);
void kr_engine_ctx_destroy(T_KRContext *ptEngineCtx);
int kr_engine_ctx_check(T_KRContext *ptCtx);
void kr_engine_ctx_clean(T_KRContext *ptCtx);
int kr_engine_ctx_process(T_KRContext *ptCtx, T_KREngineArg *ptArg);

#endif /* __KR_ENGINE_CTX_H__ */
