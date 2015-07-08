#include "kr_input_handle.h"

T_KRInputHandle* kr_input_handle_new(int iInputId, char *psFormat, T_KRModule *ptModule)
{
    KR_LOG(KR_LOGDEBUG, "loading input handle[%d],[%s]!", iInputId, psFormat);

    T_KRInputHandle *ptInputHandle = kr_calloc(sizeof(*ptInputHandle));
    if (ptInputHandle == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptInputHandle error!");
        return NULL;
    }
    ptInputHandle->iInputId = iInputId;
    strncpy(ptInputHandle->caFormat, psFormat, sizeof(ptInputHandle->caFormat));
    
    char caFuncName[100] = {0};
    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_map_pre_func_%d", psFormat, iInputId);
    ptInputHandle->pfInputPre = 
        (KRInputPreFunc )kr_module_symbol(ptModule, caFuncName);
    if (ptInputHandle->pfInputPre == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", caFuncName);
        kr_free(ptInputHandle);
        return NULL;
    }

    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_map_func_%d", psFormat, iInputId);
    ptInputHandle->pfInput = 
        (KRInputFunc )kr_module_symbol(ptModule, caFuncName);
    if (ptInputHandle->pfInput == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", caFuncName);
        kr_free(ptInputHandle);
        return NULL;
    }

    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_map_post_func_%d", psFormat, iInputId);
    ptInputHandle->pfInputPost = 
        (KRInputPostFunc )kr_module_symbol(ptModule, caFuncName);
    if (ptInputHandle->pfInputPost == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", caFuncName);
        kr_free(ptInputHandle);
        return NULL;
    }

    return ptInputHandle;
}


void kr_input_handle_free(T_KRInputHandle* ptInputHandle)
{
    kr_free(ptInputHandle);
}


int kr_input_handle_match(T_KRInputHandle *ptInputHandle, char *psFormat)
{
    return strcmp(ptInputHandle->caFormat, psFormat);
}
