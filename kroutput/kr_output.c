#include "kr_output.h"
#include "kr_output_define.h"
#include "kr_output_handle.h"


static int _kr_output_define_new(char *psParamClassName, char *psParamObjectKey, char *psParamObjectString, void *ptParamObject, void *data)
{
    T_KRParamOutput *ptParamOutput = (T_KRParamOutput *)ptParamObject;
    T_KROutput *ptOutput = (T_KROutput *)data;
    
    T_KROutputDefine *ptOutputDefine = kr_output_define_new(ptParamOutput);
    if (ptOutputDefine == NULL) {
        KR_LOG(KR_LOGERROR, "kr_output_define_new [%ld] error!", \
            ptParamOutput->lOutputId);
        return -1;
    }
    
    kr_list_add_sorted(ptOutput->ptOutputDefineList, ptOutputDefine, NULL);
    
    return 0;
}


T_KROutput* kr_output_construct(T_KRParam *ptParam, T_KRModule *ptOutputModule)
{
    T_KROutput *ptOutput = kr_calloc(sizeof(*ptOutput));
    if (ptOutput == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptOutput failed!");
        return NULL;
    }
    
    ptOutput->ptOutputModule = ptOutputModule;
    
    /*alloc output define list*/
    ptOutput->ptOutputDefineList = kr_list_new();
    kr_list_set_match(ptOutput->ptOutputDefineList, (KRListMatchFunc )kr_output_define_match);
    kr_list_set_free(ptOutput->ptOutputDefineList, (KRListFreeFunc )kr_output_define_free);
    
    /*create output list*/
    if (kr_param_object_foreach(ptParam, KR_PARAM_OUTPUT, 
            _kr_output_define_new, ptOutput) != 0) {
        KR_LOG(KR_LOGERROR, "_kr_db_build_table Error!");
        return NULL;
    }
    
    ptOutput->tConstructTime = kr_param_load_time(ptParam);
    
    return ptOutput;
}


void kr_output_destruct(T_KROutput* ptOutput)
{
    if (ptOutput) {
        kr_list_destroy(ptOutput->ptOutputDefineList);
        kr_free(ptOutput);
    }
}


int kr_output_check(T_KROutput* ptOutput, T_KRParam *ptParam)
{
    /*check output param*/
    if (ptOutput->tConstructTime != kr_param_load_time(ptParam)) {
        KR_LOG(KR_LOGDEBUG, "reload ...[%ld][%ld]", 
                ptOutput->tConstructTime, kr_param_load_time(ptParam));

        //TODO:reconstruct ptOutput
    }
    return 0;
}


T_KROutputDefine* kr_output_get_define(T_KROutput *ptOutput, int iOutputId)
{
    T_KRListNode *ptListNode = kr_list_search(ptOutput->ptOutputDefineList, &iOutputId);
    if (ptListNode == NULL) {
        return NULL;
    }
    
    return (T_KROutputDefine *)kr_list_value(ptListNode);
}


T_KRRecord *kr_output_process(T_KROutput *ptOutput, T_KRMessage *ptMessage)
{
    T_KROutputDefine *ptOutputDefine = kr_output_get_define(ptOutput, ptMessage->datasrc);
    if (ptOutputDefine == NULL) {
        KR_LOG(KR_LOGERROR, "kr_output_get_define [%d] error!", ptMessage->datasrc);
        return NULL;
    }
    
    T_KROutputHandle *ptOutputHandle = kr_output_define_get_handle(ptOutputDefine, ptMessage->msgfmt, ptOutput->ptOutputModule);
    if (ptOutputHandle == NULL) {
        KR_LOG(KR_LOGERROR, "kr_output_define_get_handle [%s] error!", ptMessage->msgfmt);
        return NULL;
    }
    
    T_KRRecord *ptRecord = kr_record_new(ptOutputDefine);
    if (ptRecord == NULL) {
        KR_LOG(KR_LOGERROR, "kr_record_new [%s] error!", ptOutputDefine->iOutputId);
        return NULL;
    }
    
    int iResult = kr_output_handle_process(ptOutputHandle, ptMessage->msglen, ptMessage->msgbuf, ptRecord);
    if (iResult != 0) {
        KR_LOG(KR_LOGERROR, "kr_output_handle_process [%s] error!", ptMessage->msgid);
        kr_record_free(ptRecord);
        return NULL;
    }
    
    return ptRecord;
}
