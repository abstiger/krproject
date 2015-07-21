#include "kr_engine_env.h"


T_KREngineEnv *kr_engine_env_create(T_KREngineConfig *ptConfig)
{
    /* set engine's environment */
    T_KREngineEnv *ptEnv = kr_calloc(sizeof(T_KREngineEnv));
    if (ptEnv == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptEnv failed!");
        goto FAILED;
    }

    /* Create parameter memory */
    ptEnv->ptParam = kr_param_create();
    if (ptEnv->ptParam == NULL) {
        KR_LOG(KR_LOGERROR, "kr_param_create failed!");
        goto FAILED;
    }
    //TODO: load parameter 

    //FIME: remove krdbModule
    ptEnv->ptInput = kr_input_construct(ptEnv->ptParam, ptConfig->krdb_module);
    if (ptEnv->ptInput == NULL) {
        KR_LOG(KR_LOGERROR, "kr_input_construct failed!");
        return NULL;
    }
    
    //FIME: remove krdbModule
    ptEnv->ptOutput = kr_output_construct(ptEnv->ptParam, ptConfig->krdb_module, 
            NULL, NULL); //FIXME:pfGetType and pfGetValue
    if (ptEnv->ptOutput == NULL) {
        KR_LOG(KR_LOGERROR, "kr_output_construct failed!");
        return NULL;
    }

    /* Start up krdb */
    ptEnv->ptDB = kr_db_new("KRDB", ptEnv->ptParam);
    if (ptEnv->ptDB == NULL) {
        KR_LOG(KR_LOGERROR, "kr_db_startup failed!");
        goto FAILED;
    }

    /* Create function table */
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
        if (ptEnv->ptInput) kr_input_destruct(ptEnv->ptInput);
        if (ptEnv->ptOutput) kr_output_destruct(ptEnv->ptOutput);
        if (ptEnv->ptParam) kr_param_destroy(ptEnv->ptParam);
        kr_free(ptEnv);
    }
}
