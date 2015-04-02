#include "kr_iface.h"

T_KRInputHandle* kr_input_handle_new(int iTableId, char *psFormat, T_KRModule *ptModule)
{
    KR_LOG(KR_LOGDEBUG, "loading iface handle[%s],[%d]!", psFormat, iTableId);

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
    ptInputHandle->pfMapPre = 
        (KRDBMapFuncPre )kr_module_symbol(ptModule, caFuncName);
    if (ptInputHandle->pfMapPre == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", caFuncName);
        kr_free(ptInputHandle);
        return NULL;
    }

    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_map_func_%d", psFormat, iTableId);
    ptInputHandle->pfMap = 
        (KRDBMapFunc )kr_module_symbol(ptModule, caFuncName);
    if (ptInputHandle->pfMap == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", caFuncName);
        kr_free(ptInputHandle);
        return NULL;
    }

    memset(caFuncName, 0x00, sizeof(caFuncName));
    snprintf(caFuncName, sizeof(caFuncName), \
            "%s_map_post_func_%d", psFormat, iTableId);
    ptInputHandle->pfMapPost = 
        (KRDBMapFuncPost )kr_module_symbol(ptModule, caFuncName);
    if (ptInputHandle->pfMapPost == NULL) {
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


int kr_input_handle_match(void *ptr, void *key)
{
    T_KRInputHandle *pt1 = (T_KRInputHandle *)ptr;
    T_KRInputHandle *pt2 = (T_KRInputHandle *)key;
    int iCmpTableId = pt1->iTableId - pt2->iTableId;
    int iCmpFormat = strcmp(pt1->caFormat, pt2->caFormat);
    if (iCmpTableId != 0) {
        return iCmpTableId;
    }
    return iCmpFormat;
}



T_KRInputHandle* kr_input_handle_get(T_KRIface *ptIface, int iTableId, char *psFormat)
{
    T_KRInputHandle stCompare = {0};
    stCompare.iTableId = iTableId;
    strncpy(stCompare.caFormat, psFormat, sizeof(stCompare.caFormat));
    
    T_KRListNode *ptListNode = \
        kr_list_search(ptIface->pInputHandleList, &stCompare);
    if (ptListNode != NULL) {
        return (T_KRInputHandle *)kr_list_value(ptListNode);
    }
    
    /*load iface handle*/
    T_KRInputHandle *ptInputHandle = \
        kr_input_handle_new(iTableId, psFormat, ptIface->ptModule);
    if (ptInputHandle != NULL) {
        /*add to table's interface handle list*/
        kr_list_add_sorted(ptIface->pInputHandleList, 
                ptInputHandle, &stCompare);
        return ptInputHandle;
    }
    
    return NULL;
}


int kr_input_handle_process(T_KRInputHandle *ptInputHandle, size_t size, void *buff, T_KRRecord *ptRecord)
{
    void *data = buff;

    if (ptInputHandle->pfMapPre) data=ptInputHandle->pfMapPre(buff);
    for (int fldno=0; fldno<ptRecord->ptTable->iFieldCnt; fldno++) {
        int fldlen = kr_field_get_length(ptRecord, fldno);
        void *fldval = kr_field_get_value(ptRecord, fldno);

        ptInputHandle->pfMap(fldval, fldno, fldlen, data);
    }
    if (ptInputHandle->pfMapPost) ptInputHandle->pfMapPost(data);

    return 0;
}

