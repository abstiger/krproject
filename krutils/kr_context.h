#ifndef __KR_CONTEXT_H__
#define __KR_CONTEXT_H__

#include "krutils/kr_hashtable.h"

typedef void* (*KRBeanConstructFunc)(void *ptEnv);
typedef void (*KRBeanDestructFunc)(void *ptData);


typedef struct _kr_context_t 
{
    void                  *ptEnv;
    T_KRHashTable         *ptMap;
}T_KRContext;

/* function declaration */
T_KRContext *kr_context_create(void *ptEnv);
void kr_context_destroy(T_KRContext *ptContext);

int kr_context_add_func(T_KRContext *ptContext, char *name, 
    KRBeanConstructFunc pfConstruct, KRBeanDestructFunc pfDestruct);
int kr_context_add_data(T_KRContext *ptContext, char *name, void *data);
void *kr_context_get_data(T_KRContext *ptContext, char *name);
void kr_context_del(T_KRContext *ptContext, char *name);

    
#endif /* __KR_CONTEXT_H__ */
