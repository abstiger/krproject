#include "kr_io.h"

T_KRIface *kr_iface_create(T_KRModule *ptModule, T_DbsEnv *ptDbsEnv)
{
    T_KRIface *ptIface = kr_calloc(sizeof(*ptIface));
    if (ptIface == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptIface error!");
        return NULL;
    }
    pthread_mutex_init(&ptIface->tLock, NULL);
    ptIface->ptModule = ptModule;
    ptIface->pInputHandleList = kr_list_new();
    kr_list_set_match(ptIface->pInputHandleList, 
            (KRCompareFunc )kr_input_handle_match);
    kr_list_set_free(ptIface->pInputHandleList, 
            (KRFreeFunc )kr_input_handle_free);

    ptIface->pOutputHandleList = kr_list_new();
    kr_list_set_match(ptIface->pOutputHandleList, 
            (KRCompareFunc )kr_output_handle_match);
    kr_list_set_free(ptIface->pOutputHandleList, 
            (KRFreeFunc )kr_output_handle_free);

    return ptIface;
}


void kr_iface_destroy(T_KRIface *ptIface)
{
    pthread_mutex_destroy(&ptIface->tLock);
    kr_list_destroy(ptIface->pOutputHandleList);
    kr_list_destroy(ptIface->pInputHandleList);
    kr_free(ptIface);
}


int kr_iface_process_input(T_KRIface *ptIface, T_KRMessage *ptInput, T_KRRecord *ptRecord)
{

}


int kr_iface_generate_output(T_KRIface *ptIface, T_KRContext *ptContext, T_KRMessage *ptOutput)
{

}

