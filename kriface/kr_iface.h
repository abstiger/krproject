#ifndef __KR_IFACE_H__
#define __KR_IFACE_H__

#include "kr_iface_input.h"
#include "kr_iface_output.h"
#include "krengine/kr_engine.h"


typedef struct _kr_iface_t
{
    pthread_mutex_t  tLock;
    T_KRModule       *ptModule;         
    T_KRList         *pInputHandleList;
    T_KRList         *pOutputHandleList;
}T_KRIface;

T_KRIface *kr_iface_create(T_KRModule *ptModule); 
void kr_iface_destroy(T_KRIface *ptIface);

int kr_iface_process_input(T_KRIface *ptIface, T_KRMessage *ptInput, T_KRRecord *ptRecord);
int kr_iface_generate_output(T_KRIface *ptIface, T_KRContext *ptContext, T_KRMessage *ptOutput);

#endif /* __KR_IFACE_H__ */
