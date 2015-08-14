#include "kr_output.h"
#include "kr_output_define.h"
#include "kr_output_handle.h"

struct _kr_output_t
{
    T_KRModule       *ptOutputModule;
    KRGetTypeFunc    pfGetType;
    KRGetValueFunc   pfGetValue;
    T_KRList         *ptOutputDefineList;
    time_t           tConstructTime;
};

static int _kr_output_define_new(char *psParamClassName, char *psParamObjectKey, 
        char *psParamObjectString, void *ptParamObject, void *data)
{
    T_KRParamOutput *ptParamOutput = (T_KRParamOutput *)ptParamObject;
    T_KROutput *ptOutput = (T_KROutput *)data;
    
    T_KROutputDefine *ptOutputDefine = kr_output_define_new(ptParamOutput, 
            ptOutput->pfGetType, ptOutput->pfGetValue);
    if (ptOutputDefine == NULL) {
        KR_LOG(KR_LOGERROR, "kr_output_define_new [%ld] error!", \
            ptParamOutput->lOutputId);
        return -1;
    }
    
    kr_list_add_sorted(ptOutput->ptOutputDefineList, ptOutputDefine, NULL);
    
    return 0;
}


T_KROutput* kr_output_construct(T_KRParam *ptParam, char *psOutputModule, 
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue)
{
    T_KROutput *ptOutput = kr_calloc(sizeof(*ptOutput));
    if (ptOutput == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptOutput failed!");
        return NULL;
    }
    
    /* load output module */
    ptOutput->ptOutputModule = kr_module_open(psOutputModule, RTLD_LAZY);
    if (ptOutput->ptOutputModule == NULL) {
        KR_LOG(KR_LOGERROR, "kr_module_open %s failed!", psOutputModule);
        kr_output_destruct(ptOutput);
        return NULL;
    }
    ptOutput->pfGetType = pfGetType;
    ptOutput->pfGetValue = pfGetValue;
    
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
        if (ptOutput->ptOutputDefineList) {
            kr_list_destroy(ptOutput->ptOutputDefineList);
        }
        if (ptOutput->ptOutputModule) {
            kr_module_close(ptOutput->ptOutputModule);
        }
        kr_free(ptOutput);
    }
}


int kr_output_check(T_KROutput* ptOutput, T_KRParam *ptParam)
{
    /*check output param*/
    if (ptOutput->tConstructTime != kr_param_load_time(ptParam)) {
        KR_LOG(KR_LOGDEBUG, "reload output ...[%ld][%ld]", 
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


int kr_output_handle_process(T_KROutputHandle *ptOutputHandle, 
        T_KROutputDefine *ptOutputDefine, size_t *size, char **buff, 
        T_KRContext *ptContext)
{
    void *data = NULL;
    
    if (ptOutputHandle->pfOutputPre) data=ptOutputHandle->pfOutputPre();
    for (int fldno=0; fldno<ptOutputDefine->iFieldCnt; fldno++) {
        T_KROutputFieldDef *ptFieldDef = &ptOutputDefine->ptFieldDef[fldno];
        
        if (kr_calc_eval(ptFieldDef->calc, ptContext) != 0) {
            KR_LOG(KR_LOGERROR, "kr_calc_eval field:[%d] failed!", fldno);
            return -1;
        }

        void *fldval = kr_get_value(kr_calc_value(ptFieldDef->calc), 
                                    kr_calc_type(ptFieldDef->calc));
        ptOutputHandle->pfOutput(data, fldno, ptFieldDef->length, fldval);
    }
    if (ptOutputHandle->pfOutputPost) ptOutputHandle->pfOutputPost(data, size, buff);

    return 0;
}


T_KRResponse *kr_output_process(T_KROutput *ptOutput, T_KRContext *ptContext)
{
    int iOutputId = *(int *)kr_context_get_data(ptContext, "output_id");
    char *psOutputFmt = (char *)kr_context_get_data(ptContext, "output_fmt");
     
    T_KROutputDefine *ptOutputDefine = \
        kr_output_get_define(ptOutput, iOutputId);
    if (ptOutputDefine == NULL) {
        KR_LOG(KR_LOGERROR, "kr_output_get_define [%d] error!", iOutputId);
        return NULL;
    }
    
    T_KROutputHandle *ptOutputHandle = \
        kr_output_define_get_handle(ptOutputDefine, psOutputFmt, ptOutput->ptOutputModule);
    if (ptOutputHandle == NULL) {
        KR_LOG(KR_LOGERROR, "kr_output_define_get_handle [%s] error!", psOutputFmt);
        return NULL;
    }
    
    T_KRResponse *ptResponse = kr_response_alloc();
    if (ptResponse == NULL) {
        KR_LOG(KR_LOGERROR, "kr_response_alloc error!");
        return NULL;
    }
    ptResponse->msgsrc = iOutputId;
    strncpy(ptResponse->msgfmt, psOutputFmt, sizeof(ptResponse->msgfmt)-1);
    
    int iResult = kr_output_handle_process(ptOutputHandle, ptOutputDefine, 
                    &ptResponse->msglen, &ptResponse->msgbuf, ptContext);
    if (iResult != 0) {
        KR_LOG(KR_LOGERROR, "kr_output_handle_process [%d][%s] error!", iOutputId, psOutputFmt);
        kr_response_free(ptResponse);
        return NULL;
    }
    
    return ptResponse;
}
