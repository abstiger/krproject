#include "kr_output.h"

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
    ptOutputHandle->pfMapPre = 
        (KRDBMapFuncPre )kr_module_symbol(ptModule, caFuncName);
    if (ptOutputHandle->pfMapPre == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", caFuncName);
        kr_free(ptOutputHandle);
        return NULL;
    }

    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_map_func_%d", psFormat, iTableId);
    ptOutputHandle->pfMap = 
        (KRDBMapFunc )kr_module_symbol(ptModule, caFuncName);
    if (ptOutputHandle->pfMap == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", caFuncName);
        kr_free(ptOutputHandle);
        return NULL;
    }

    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_map_post_func_%d", psFormat, iTableId);
    ptOutputHandle->pfMapPost = 
        (KRDBMapFuncPost )kr_module_symbol(ptModule, caFuncName);
    if (ptOutputHandle->pfMapPost == NULL) {
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


int kr_output_handle_match(void *ptr, void *key)
{
    T_KROutputHandle *pt1 = (T_KROutputHandle *)ptr;
    T_KROutputHandle *pt2 = (T_KROutputHandle *)key;
    int iCmpTableId = pt1->iTableId - pt2->iTableId;
    int iCmpFormat = strcmp(pt1->caFormat, pt2->caFormat);
    if (iCmpTableId != 0) {
        return iCmpTableId;
    }
    return iCmpFormat;
}



T_KROutputHandle* kr_output_handle_get(T_KRIO *ptIO, int iTableId, char *psFormat)
{
    T_KROutputHandle stCompare = {0};
    stCompare.iTableId = iTableId;
    strncpy(stCompare.caFormat, psFormat, sizeof(stCompare.caFormat));
    
    T_KRListNode *ptListNode = \
        kr_list_search(ptIO->pOutputHandleList, &stCompare);
    if (ptListNode != NULL) {
        return (T_KROutputHandle *)kr_list_value(ptListNode);
    }
    
    /*load io handle*/
    T_KROutputHandle *ptOutputHandle = \
        kr_output_handle_new(iTableId, psFormat, ptIO->ptModule);
    if (ptOutputHandle != NULL) {
        /*add to table's interface handle list*/
        kr_list_add_sorted(ptIO->pOutputHandleList, 
                ptOutputHandle, &stCompare);
        return ptOutputHandle;
    }
    
    return NULL;
}


int kr_output_handle_process(T_KROutputHandle *ptOutputHandle, size_t size, void *buff, T_KRRecord *ptRecord)
{
    void *data = buff;

    if (ptOutputHandle->pfMapPre) data=ptOutputHandle->pfMapPre(buff);
    for (int fldno=0; fldno<ptRecord->ptTable->iFieldCnt; fldno++) {
        int fldlen = kr_field_get_length(ptRecord, fldno);
        void *fldval = kr_field_get_value(ptRecord, fldno);

        ptOutputHandle->pfMap(fldval, fldno, fldlen, data);
    }
    if (ptOutputHandle->pfMapPost) ptOutputHandle->pfMapPost(data);

    return 0;
}

