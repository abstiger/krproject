#include "kr_output_handle.h"

T_KROutputHandle* kr_output_handle_new(int iTableId, char *psFormat, T_KRModule *ptModule)
{
    KR_LOG(KR_LOGDEBUG, "loading io handle[%s],[%d]!", psFormat, iTableId);

    T_KROutputHandle *ptOutputHandle = kr_calloc(sizeof(*ptOutputHandle));
    if (ptOutputHandle == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptOutputHandle error!");
        return NULL;
    }
    ptOutputHandle->iTableId = iTableId;
    strncpy(ptOutputHandle->caFormat, psFormat, sizeof(ptOutputHandle->caFormat));
    
    char caFuncName[100] = {0};
    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_map_pre_func_%d", psFormat, iTableId);
    ptOutputHandle->pfOutputPre = 
        (KROutputPreFunc )kr_module_symbol(ptModule, caFuncName);
    if (ptOutputHandle->pfOutputPre == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", caFuncName);
        kr_free(ptOutputHandle);
        return NULL;
    }

    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_map_func_%d", psFormat, iTableId);
    ptOutputHandle->pfOutput = 
        (KROutputFunc )kr_module_symbol(ptModule, caFuncName);
    if (ptOutputHandle->pfOutput == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", caFuncName);
        kr_free(ptOutputHandle);
        return NULL;
    }

    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_map_post_func_%d", psFormat, iTableId);
    ptOutputHandle->pfOutputPost = 
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
    kr_free(ptOutputHandle);
}


int kr_output_handle_match(T_KROutputHandle *ptOutputHandle, char *psFormat)
{
    return strcmp(ptOutputHandle->caFormat, psFormat);
}


int kr_output_handle_process(T_KROutputHandle *ptOutputHandle, size_t size, void *buff, T_KRRecord *ptRecord)
{
    void *data = buff;

    /*
    if (ptOutputHandle->pfOutputPre) data=ptOutputHandle->pfOutputPre(buff);
    for (int fldno=0; fldno<ptRecord->ptOutputDefine->iFieldCnt; fldno++) {
        int fldlen = kr_record_get_field_length(ptRecord, fldno);
        void *fldval = kr_record_get_field_value(ptRecord, fldno);

        ptOutputHandle->pfOutput(fldval, fldno, fldlen, data);
    }
    if (ptOutputHandle->pfOutputPost) ptOutputHandle->pfOutputPost(data);
    */

    return 0;
}

