#include "kr_input_handle.h"

T_KRInputHandle* kr_input_handle_new(int iTableId, char *psFormat, T_KRModule *ptModule)
{
    KR_LOG(KR_LOGDEBUG, "loading io handle[%s],[%d]!", psFormat, iTableId);

    T_KRInputHandle *ptInputHandle = kr_calloc(sizeof(*ptInputHandle));
    if (ptInputHandle == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptInputHandle error!");
        return NULL;
    }
    ptInputHandle->iTableId = iTableId;
    strncpy(ptInputHandle->caFormat, psFormat, sizeof(ptInputHandle->caFormat));
    
    char caFuncName[100] = {0};
    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_map_pre_func_%d", psFormat, iTableId);
    ptInputHandle->pfInputPre = 
        (KRInputFuncPre )kr_module_symbol(ptModule, caFuncName);
    if (ptInputHandle->pfInputPre == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", caFuncName);
        kr_free(ptInputHandle);
        return NULL;
    }

    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_map_func_%d", psFormat, iTableId);
    ptInputHandle->pfInput = 
        (KRInputFunc )kr_module_symbol(ptModule, caFuncName);
    if (ptInputHandle->pfInput == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", caFuncName);
        kr_free(ptInputHandle);
        return NULL;
    }

    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_map_post_func_%d", psFormat, iTableId);
    ptInputHandle->pfInputPost = 
        (KRInputFuncPost )kr_module_symbol(ptModule, caFuncName);
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


int kr_input_handle_process(T_KRInputHandle *ptInputHandle, size_t size, void *buff, T_KRRecord *ptRecord)
{
    void *data = buff;

    if (ptInputHandle->pfInputPre) data=ptInputHandle->pfInputPre(buff);
    for (int fldno=0; fldno<ptRecord->ptTable->iFieldCnt; fldno++) {
        int fldlen = kr_field_get_length(ptRecord, fldno);
        void *fldval = kr_field_get_value(ptRecord, fldno);

        ptInputHandle->pfInput(fldval, fldno, fldlen, data);
    }
    if (ptInputHandle->pfInputPost) ptInputHandle->pfInputPost(data);

    return 0;
}

