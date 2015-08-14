#include "kr_engine_env.h"

extern E_KRType kr_engine_ctx_get_type(char kind, int id, void *data);
extern void *kr_engine_ctx_get_value(char kind, int id, void *data);

T_KREngineEnv *kr_engine_env_create(T_KREngineConfig *ptConfig)
{
    /* set engine's environment */
    T_KREngineEnv *ptEnv = kr_calloc(sizeof(T_KREngineEnv));
    if (ptEnv == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptEnv failed!");
        goto FAILED;
    }
    ptEnv->ptConfig = ptConfig;

    // Create parameter memory 
    ptEnv->ptParam = kr_param_create();
    if (ptEnv->ptParam == NULL) {
        KR_LOG(KR_LOGERROR, "kr_param_create failed!");
        goto FAILED;
    }
    // Load parameter 
    if (kr_param_load(ptEnv->ptParam, ptConfig->param_url) < 0) {
        KR_LOG(KR_LOGERROR, "kr_param_load [%s] failed!", ptConfig->param_url);
        goto FAILED;
    }

    // create input module
    ptEnv->ptInput = kr_input_construct(ptEnv->ptParam, ptConfig->input_module);
    if (ptEnv->ptInput == NULL) {
        KR_LOG(KR_LOGERROR, "kr_input_construct %s failed!", \
                ptConfig->input_module);
        return NULL;
    }
    
    //create output module
    ptEnv->ptOutput = kr_output_construct(ptEnv->ptParam, ptConfig->output_module, 
            kr_engine_ctx_get_type, kr_engine_ctx_get_value);
    if (ptEnv->ptOutput == NULL) {
        KR_LOG(KR_LOGERROR, "kr_output_construct %s failed!", \
                ptConfig->output_module);
        return NULL;
    }

    //create krdb
    ptEnv->ptDB = kr_db_new("KRDB", ptEnv->ptParam);
    if (ptEnv->ptDB == NULL) {
        KR_LOG(KR_LOGERROR, "kr_db_startup failed!");
        goto FAILED;
    }

    // Create function table
    ptEnv->ptFuncTable = kr_functable_create("engine");
    if (ptEnv->ptFuncTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_functable_create engine failed!");
        goto FAILED;
    }

    return ptEnv;

FAILED:
    kr_engine_env_destroy(ptEnv);
    return NULL;
}


void kr_engine_env_destroy(T_KREngineEnv *ptEnv)
{
    if (ptEnv) {
        if (ptEnv->ptFuncTable) kr_functable_destroy(ptEnv->ptFuncTable);
        if (ptEnv->ptDB) kr_db_free(ptEnv->ptDB);
        if (ptEnv->ptOutput) kr_output_destruct(ptEnv->ptOutput);
        if (ptEnv->ptInput) kr_input_destruct(ptEnv->ptInput);
        if (ptEnv->ptParam) kr_param_destroy(ptEnv->ptParam);
        kr_free(ptEnv);
    }
}
