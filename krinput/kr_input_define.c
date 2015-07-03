#include "kr_input_define.h"


T_KRInputDefine *kr_input_define_new(T_KRParamInput *ptParamInput)
{
    T_KRInputDefine *ptInputDefine = kr_calloc(sizeof(*ptInputDefine));
    if (ptInputDefine == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptInputDefine failed!");
        return NULL;
    }
    ptInputDefine->iInputId = ptParamInput->lOutInputId;
    strncpy(ptInputDefine->caInputName, ptParamInput->caOutInputName, 
            sizeof(ptInputDefine->caInputName)-1);
    strncpy(ptInputDefine->caInputDesc, ptParamInput->caOutInputDesc, 
            sizeof(ptInputDefine->caInputDesc)-1);
    
    // load fields define
    size_t ulFieldOffset = 0;
    for (int i=0; i<ptParamInput->lFieldCnt; ++i) {
        T_KRParamInputField *ptParamInputField = &ptParamInput->ptFieldDef[i];
        T_KRInputFieldDef *ptFieldDef = &ptInputDefine->ptFieldDef[iCnt];
        
        ptFieldDef->id = ptParamInputField->lOutFieldId;
        strncpy(ptFieldDef->name, ptParamInputField->caOutFieldName, \
                sizeof(ptFieldDef->name));
        ptFieldDef->type = ptParamInputField->caOutFieldType[0];
        ptFieldDef->length = ptParamInputField->lOutFieldLength;
        ptFieldDef->offset = ulFieldOffset;
        ulFieldOffset += ptFieldDef->length;
        /* String terminated with '\0' */
        if (ptFieldDef[iCnt].type == KR_TYPE_STRING) {
            ulFieldOffset += 1;
        }
    }
    
    /*alloc input handle list*/
    ptInputDefine->ptInputHandleList = kr_list_new();
    kr_list_set_match(ptInputDefine->ptInputHandleList, (KRCompareFunc )kr_input_handle_match);
    kr_list_set_free(ptInputDefine->ptInputHandleList, (KRFreeFunc )kr_input_handle_free);
            
    return ptInputDefine;
}


void kr_input_define_free(T_KRInputDefine *ptInputDefine)
{
    if (ptInputDefine) {
        if (ptInputDefine->ptFieldDef) {
            kr_free(ptInputDefine->ptFieldDef);
        }
        kr_list_destroy(ptInputDefine->ptInputHandleList);
        kr_free(ptInputDefine);
    }
}


int kr_input_define_match(T_KRInputDefine *ptInputDefine, int *piInputId)
{
    return (ptInputDefine->piInputId == *piInputId));
}


T_KRInputHandle* kr_input_define_get_handle(T_KRInputDefine *ptInputDefine, char *psFormat, T_KRModule *ptModule)
{
    T_KRListNode *ptListNode = kr_list_search(ptInputDefine->pInputHandleList, psFormat);
    if (ptListNode != NULL) {
        return (T_KRInputHandle *)kr_list_value(ptListNode);
    }
    
    /*load io handle*/
    pthread_mutex_lock(ptInputDefine->tLock);
    T_KRInputHandle *ptInputHandle = \
        kr_input_handle_new(ptInputDefine->iInputId, psFormat, ptModule);
    if (ptInputHandle != NULL) {
        /*add to table's interface handle list*/
        kr_list_add_sorted(ptInputDefine->pInputHandleList, 
                ptInputHandle, &stCompare);
    }
    pthread_mutex_unlock(ptInputDefine->tLock);
    
    return ptInputHandle;
}