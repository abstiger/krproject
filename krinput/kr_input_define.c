#include "kr_input_define.h"

#define KR_MEMALIGN(size)   (((size) + 0xf) & ~(size_t) 0xf)

T_KRInputDefine *kr_input_define_new(T_KRParamInput *ptParamInput)
{
    T_KRInputDefine *ptInputDefine = kr_calloc(sizeof(*ptInputDefine));
    if (ptInputDefine == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptInputDefine failed!");
        return NULL;
    }
    ptInputDefine->iInputId = ptParamInput->lInputId;
    strncpy(ptInputDefine->caInputName, ptParamInput->caInputName, 
            sizeof(ptInputDefine->caInputName)-1);
    strncpy(ptInputDefine->caInputDesc, ptParamInput->caInputDesc, 
            sizeof(ptInputDefine->caInputDesc)-1);
    ptInputDefine->iFieldCnt = ptParamInput->lFieldCnt;
    ptInputDefine->ptFieldDef = kr_calloc(ptInputDefine->iFieldCnt * sizeof(T_KRInputFieldDef));
    if (ptInputDefine->ptFieldDef == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptInputDefine->ptFieldDef failed!");
        kr_free(ptInputDefine);
        return NULL;
    }
    // load fields define
    ptInputDefine->iRecordSize = 0;
    for (int i=0; i<ptParamInput->lFieldCnt; ++i) {
        T_KRParamInputField *ptParamInputField = &ptParamInput->ptFieldDef[i];
        T_KRInputFieldDef *ptFieldDef = &ptInputDefine->ptFieldDef[i];
        
        ptFieldDef->id = ptParamInputField->lFieldId;
        strncpy(ptFieldDef->name, ptParamInputField->caFieldName, \
                sizeof(ptFieldDef->name));
        ptFieldDef->type = ptParamInputField->caFieldType[0];
        ptFieldDef->length = ptParamInputField->lFieldLength;
        ptFieldDef->offset = ptInputDefine->iRecordSize;
        ptInputDefine->iRecordSize += ptFieldDef->length;
        /* String terminated with '\0' */
        if (ptFieldDef->type == KR_TYPE_STRING) {
            ptInputDefine->iRecordSize += 1;
        }
    }
    ptInputDefine->iRecordSize = KR_MEMALIGN(ptInputDefine->iRecordSize);
    
    /*alloc input handle list*/
    ptInputDefine->ptInputHandleList = kr_list_new();
    kr_list_set_match(ptInputDefine->ptInputHandleList, (KRListMatchFunc )kr_input_handle_match);
    kr_list_set_free(ptInputDefine->ptInputHandleList, (KRListFreeFunc )kr_input_handle_free);
            
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
    return (ptInputDefine->iInputId == *piInputId);
}


T_KRInputHandle* kr_input_define_get_handle(T_KRInputDefine *ptInputDefine, char *psFormat, T_KRModule *ptModule)
{
    T_KRListNode *ptListNode = kr_list_search(ptInputDefine->ptInputHandleList, psFormat);
    if (ptListNode != NULL) {
        return (T_KRInputHandle *)kr_list_value(ptListNode);
    }
    
    /*load io handle*/
    T_KRInputHandle *ptInputHandle = \
        kr_input_handle_new(ptInputDefine->iInputId, psFormat, ptModule);
    if (ptInputHandle != NULL) {
        /*add to table's interface handle list*/
        kr_list_add_sorted(ptInputDefine->ptInputHandleList, ptInputHandle, psFormat);
    }
    
    return ptInputHandle;
}
