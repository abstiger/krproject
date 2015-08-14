#include "kr_output_define.h"


T_KROutputDefine *kr_output_define_new(T_KRParamOutput *ptParamOutput,
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue)
{
    T_KROutputDefine *ptOutputDefine = kr_calloc(sizeof(*ptOutputDefine));
    if (ptOutputDefine == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptOutputDefine failed!");
        return NULL;
    }
    ptOutputDefine->iOutputId = ptParamOutput->lOutputId;
    strncpy(ptOutputDefine->caOutputName, ptParamOutput->caOutputName, 
            sizeof(ptOutputDefine->caOutputName)-1);
    strncpy(ptOutputDefine->caOutputDesc, ptParamOutput->caOutputDesc, 
            sizeof(ptOutputDefine->caOutputDesc)-1);
    ptOutputDefine->iFieldCnt = ptParamOutput->lFieldCnt;
    ptOutputDefine->ptFieldDef = kr_calloc(ptOutputDefine->iFieldCnt * sizeof(T_KROutputFieldDef));
    if (ptOutputDefine->ptFieldDef == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptOutputDefine->ptFieldDef failed!");
        kr_free(ptOutputDefine);
        return NULL;
    }
    // load fields define
    size_t ulFieldOffset = 0;
    for (int i=0; i<ptParamOutput->lFieldCnt; ++i) {
        T_KRParamOutputField *ptParamOutputField = &ptParamOutput->ptFieldDef[i];
        T_KROutputFieldDef *ptFieldDef = &ptOutputDefine->ptFieldDef[i];
        
        ptFieldDef->id = ptParamOutputField->lFieldId;
        strncpy(ptFieldDef->name, ptParamOutputField->caFieldName, \
                sizeof(ptFieldDef->name));
        ptFieldDef->type = ptParamOutputField->caFieldType[0];
        ptFieldDef->length = ptParamOutputField->lFieldLength;
        ptFieldDef->offset = ulFieldOffset;
        ulFieldOffset += ptFieldDef->length;
        /* String terminated with '\0' */
        if (ptFieldDef->type == KR_TYPE_STRING) {
            ulFieldOffset += 1;
        }

        //construct field calc
        ptFieldDef->calc = kr_calc_construct(ptParamOutputField->caCalcFormat[0], 
                ptParamOutputField->caCalcString, pfGetType, pfGetValue);
        if (ptFieldDef->calc == NULL) {
            KR_LOG(KR_LOGERROR, "kr_calc_construct [%s]:[%s] failed!",
                    ptParamOutputField->caCalcFormat,
                    ptParamOutputField->caCalcString);
            return NULL;
        }
    }
    
    /*alloc output handle list*/
    ptOutputDefine->ptOutputHandleList = kr_list_new();
    kr_list_set_match(ptOutputDefine->ptOutputHandleList, (KRListMatchFunc )kr_output_handle_match);
    kr_list_set_free(ptOutputDefine->ptOutputHandleList, (KRListFreeFunc )kr_output_handle_free);
            
    return ptOutputDefine;
}


void kr_output_define_free(T_KROutputDefine *ptOutputDefine)
{
    if (ptOutputDefine) {
        if (ptOutputDefine->ptFieldDef) {
            kr_free(ptOutputDefine->ptFieldDef);
        }
        kr_list_destroy(ptOutputDefine->ptOutputHandleList);
        kr_free(ptOutputDefine);
    }
}


int kr_output_define_match(T_KROutputDefine *ptOutputDefine, int *piOutputId)
{
    return (ptOutputDefine->iOutputId == *piOutputId);
}


T_KROutputHandle* kr_output_define_get_handle(T_KROutputDefine *ptOutputDefine, 
        char *psFormat, T_KRModule *ptModule)
{
    T_KRListNode *ptListNode = \
        kr_list_search(ptOutputDefine->ptOutputHandleList, psFormat);
    if (ptListNode != NULL) {
        return (T_KROutputHandle *)kr_list_value(ptListNode);
    }
    
    /*load io handle*/
    T_KROutputHandle *ptOutputHandle = \
        kr_output_handle_new(ptOutputDefine->iOutputId, psFormat, ptModule);
    if (ptOutputHandle != NULL) {
        /*add to table's interface handle list*/
        kr_list_add_sorted(ptOutputDefine->ptOutputHandleList, ptOutputHandle, psFormat);
    }
    
    return ptOutputHandle;
}
