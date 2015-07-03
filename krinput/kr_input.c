#include "kr_input.h"
#include "kr_input_define.h"
#include "kr_input_handle.h"


static int _kr_input_define_new(char *psParamClassName, char *psParamObjectKey, char *psParamObjectString, void *ptParamObject, void *data)
{
    T_KRParamInput *ptParamInput = (T_KRParamInput *)ptParamObject;
    T_KRInput *ptInput = (T_KRInput *)data;
    
    T_KRInputDefine *ptInputDefine = kr_input_define_new(ptParamInput);
    if (ptInputDefine == NULL) {
        KR_LOG(KR_LOGERROR, "kr_input_define_new [%ld] error!", \
            ptParamInput->lInputId);
        return -1;
    }
    
    kr_list_add_sorted(ptInput->ptInputDefineList, ptInputDefine, NULL);
    
    return 0;
}


T_KRInput* kr_input_construct(T_KRParam *ptParam, T_KRModule *ptInputModule)
{
    T_KRInput *ptInput = kr_calloc(sizeof(*ptInput));
    if (ptInput == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptInput failed!");
        return NULL;
    }
    
    /*alloc input define list*/
    ptInput->ptInputDefineList = kr_list_new();
    kr_list_set_match(ptInput->ptInputDefineList, (KRCompareFunc )kr_input_define_match);
    kr_list_set_free(ptInput->ptInputDefineList, (KRFreeFunc )kr_input_define_free);
    
    /*create input list*/
    if (kr_param_object_foreach(ptParam, KR_PARAM_INPUT, 
            _kr_input_define_new, ptInput) != 0) {
        KR_LOG(KR_LOGERROR, "_kr_db_build_table Error!");
        return NULL;
    }
    
    ptInput->tConstructTime = kr_param_load_time(ptParam);
    
    return ptInput;
}


void kr_input_destruct(T_KRInput* ptInput)
{
    if (ptInput) {
        kr_list_destroy(ptInput->ptInputDefineList);
        kr_free(ptInput);
    }
}


int kr_input_check(T_KRInput* ptInput, T_KRParam *ptParam)
{
    //TODO
    return 0;
}


T_KRRecord *kr_input_process(T_KRInput *ptInput, T_KRMessage *ptMessage)
{
    //TODO
    return NULL;
}
