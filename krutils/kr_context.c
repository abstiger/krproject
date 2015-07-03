#include "kr_context.h"
#include "krutils/kr_string.h"

typedef struct _kr_bean_t
{
    char                  *name;
    void                  *data;
    KRBeanConstructFunc   pfConstruct;
    KRBeanDestructFunc    pfDestruct;
}T_KRBean;


static T_KRBean *kr_bean_new(char *name, void *data, 
    KRBeanConstructFunc pfConstruct, KRBeanDestructFunc pfDestruct)
{
    T_KRBean *ptBean = kr_calloc(sizeof(*ptBean));
    if (ptBean == NULL) {
        fprintf(stderr, "kr_calloc ptBean failed!");
        return NULL;
    }
    
    ptBean->name = kr_strdup(name);
    ptBean->data = data;
    ptBean->pfConstruct = pfConstruct;
    ptBean->pfDestruct = pfDestruct;
    
    return ptBean;
}

static void kr_bean_free(T_KRBean *ptBean)
{
    if (ptBean) {
        if (ptBean->pfDestruct) {
            ptBean->pfDestruct(ptBean->data);
        }
        kr_free(ptBean);
    }
}

            
/* initialize rule detecting context */
T_KRContext *kr_context_create(void *ptEnv)
{
    T_KRContext *ptContext = kr_calloc(sizeof(T_KRContext));
    if (ptContext == NULL) {
        fprintf(stderr, "kr_calloc ptContext failed!");
        return NULL;
    }

    ptContext->ptEnv = ptEnv;
    ptContext->ptMap = kr_hashtable_new_full(
                    (KRHashFunc )kr_string_hash, 
                    (KREqualFunc )kr_string_equal, 
                    (KRDestroyNotify)kr_free,
                    (KRDestroyNotify)kr_bean_free);
    
    return ptContext;
}


void kr_context_destroy(T_KRContext *ptContext)
{
    if (ptContext) {
        kr_hashtable_destroy(ptContext->ptMap);
        kr_free(ptContext);
    }
}


int kr_context_add_func(T_KRContext *ptContext, char *name, 
    KRBeanConstructFunc pfConstruct, KRBeanDestructFunc pfDestruct)
{
    /*construct data first*/
    void *data = pfConstruct(ptContext->ptEnv);
    if (data == NULL) {
        fprintf(stderr, "construct bean:[%s] failed!", name);
        return -1;
    }
    
    T_KRBean *ptBean = kr_bean_new(name, data, pfConstruct, pfDestruct);
    if (ptBean == NULL) {
        fprintf(stderr, "kr_bean_new [%s] failed!", name);
        return -1;
    }

    kr_hashtable_replace(ptContext->ptMap, ptBean->name, ptBean);

    return 0;
}


int kr_context_add_data(T_KRContext *ptContext, char *name, void *data)
{
    T_KRBean *ptBean = kr_bean_new(name, data, NULL, NULL);
    if (ptBean == NULL) {
        fprintf(stderr, "kr_bean_new [%s] failed!", name);
        return -1;
    }
    
    kr_hashtable_replace(ptContext->ptMap, ptBean->name, ptBean);

    return 0;
}


void *kr_context_get_data(T_KRContext *ptContext, char *name)
{
    T_KRBean *ptBean = kr_hashtable_lookup(ptContext->ptMap, name);
    if (ptBean == NULL) {
        fprintf(stderr, "bean [%s] not found!", name);
        return NULL;
    }
    
    return ptBean->data;
}


void kr_context_del(T_KRContext *ptContext, char *name)
{
    kr_hashtable_remove(ptContext->ptMap, name);
}
