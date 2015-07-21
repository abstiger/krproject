#include "kr_output_handle.h"

T_KROutputHandle* kr_output_handle_new(int iOutputId, char *psFormat, T_KRModule *ptModule)
{
    KR_LOG(KR_LOGDEBUG, "loading output handle[%s],[%d]!", psFormat, iOutputId);

    T_KROutputHandle *ptOutputHandle = kr_calloc(sizeof(*ptOutputHandle));
    if (ptOutputHandle == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptOutputHandle error!");
        return NULL;
    }
    ptOutputHandle->iOutputId = iOutputId;
    strncpy(ptOutputHandle->caFormat, psFormat, sizeof(ptOutputHandle->caFormat));
    
    char caFuncName[100] = {0};
    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_output_source_pre_func_%d", psFormat, iOutputId);
    ptOutputHandle->pfOutputPre = \
        (KROutputPreFunc )kr_module_symbol(ptModule, caFuncName);
    if (ptOutputHandle->pfOutputPre == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", caFuncName);
        kr_free(ptOutputHandle);
        return NULL;
    }

    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_output_source_func_%d", psFormat, iOutputId);
    ptOutputHandle->pfOutput = \
        (KROutputFunc )kr_module_symbol(ptModule, caFuncName);
    if (ptOutputHandle->pfOutput == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", caFuncName);
        kr_free(ptOutputHandle);
        return NULL;
    }

    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_output_source_post_func_%d", psFormat, iOutputId);
    ptOutputHandle->pfOutputPost = \
        (KROutputPostFunc )kr_module_symbol(ptModule, caFuncName);
    if (ptOutputHandle->pfOutputPost == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", caFuncName);
        kr_free(ptOutputHandle);
        return NULL;
    }

    return ptOutputHandle;
}


void kr_output_handle_free(T_KROutputHandle* ptOutputHandle)
{
    if (ptOutputHandle) {
        kr_free(ptOutputHandle);
    }
}


int kr_output_handle_match(T_KROutputHandle *ptOutputHandle, char *psFormat)
{
    return strcmp(ptOutputHandle->caFormat, psFormat);
}
