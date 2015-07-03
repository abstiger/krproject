#include "kr_param.h"
#include "kr_param_class.h"
#include "krutils/kr_utils.h"

/*a read only parameter implemention*/
#define N_MAX_SEC 2
struct _kr_param_t
{
    short              nSecId;                   /*current section indicator*/
    char               *psParamVersion;          /*current version*/
    T_KRHashTable      *ptHashTable[N_MAX_SEC];  /*class value hashtable */
    time_t             tLastLoadTime;            /*set while param load*/
};

/*parameter hash slot definition*/
typedef struct _kr_param_slot_t
{
    T_KRParamClass     *ptParamClass;            /*definition of this class*/
    T_KRList           *ptParamObjectList;       /*objects of this class*/
}T_KRParamSlot;

/*parameter object definition*/
typedef struct _kr_param_object_t
{
    T_KRParamClass     *ptParamClass;
    char               *psParamObjectKey;        /*read only*/
    char               *psParamObjectString;     /*read only*/
    
    void               *ptParamObject;
}T_KRParamObject;



T_KRParamObject *kr_param_object_new(T_KRParamClass *ptParamClass, 
    char *psParamObjectKey, char *psParamObjectString)
{
    T_KRParamObject *ptParamObject = kr_calloc(sizeof(T_KRParamObject));
    if (ptParamObject == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptParamObject failed!");
        return NULL;
    }
    ptParamObject->ptParamClass = ptParamClass;
    ptParamObject->psParamObjectKey = kr_strdup(psParamObjectKey);
    ptParamObject->psParamObjectString = kr_strdup(psParamObjectString);
    
    ptParamObject->ptParamObject = \
        ptParamObject->ptParamClass->pfDeserialize(psParamObjectString);
    
    return ptParamObject;
}

void kr_param_object_free(T_KRParamObject *ptParamObject)
{
    if (ptParamObject) {
        if (ptParamObject->psParamObjectKey) {
            kr_free(ptParamObject->psParamObjectKey);
        }
        if (ptParamObject->psParamObjectString) {
            kr_free(ptParamObject->psParamObjectString);
        }
        if (ptParamObject->ptParamObject) {
            ptParamObject->ptParamClass->pfDeserializeFree(ptParamObject->ptParamObject);
        }
        kr_free(ptParamObject);
    }
}

static inline int kr_param_object_compare(void *a, void *b, void *data)
{
    T_KRParamObject *obj1 = (T_KRParamObject *)a;
    T_KRParamObject *obj2 = (T_KRParamObject *)b;
    return strcmp(obj1->psParamObjectKey, obj2->psParamObjectKey);
}

static inline int kr_param_object_match(void *ptr, void *key)
{
    T_KRParamObject *ptParamObject = (T_KRParamObject *)ptr; 
    return strcmp(ptParamObject->psParamObjectKey, (char *)key);
}    


T_KRParamSlot *kr_param_slot_new(T_KRParamClass *ptParamClass)
{
    T_KRParamSlot *ptParamSlot = kr_calloc(sizeof(T_KRParamSlot));
    if (ptParamSlot == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptParamClass failed!");
        return NULL;
    }
    ptParamSlot->ptParamClass = ptParamClass;
    ptParamSlot->ptParamObjectList = kr_list_new();
    kr_list_set_compare(ptParamSlot->ptParamObjectList, 
            (KRListCompareFunc )kr_param_object_compare);
    kr_list_set_match(ptParamSlot->ptParamObjectList, 
            (KRListMatchFunc )kr_param_object_match);
    kr_list_set_free(ptParamSlot->ptParamObjectList, 
            (KRListFreeFunc )kr_param_object_free);
    
    return ptParamSlot;
}


void kr_param_slot_free(T_KRParamSlot *ptParamSlot)
{
    if (ptParamSlot) {
        if (ptParamSlot->ptParamObjectList) {
            kr_list_destroy(ptParamSlot->ptParamObjectList);
        }
        kr_free(ptParamSlot);
    }
}


T_KRParam *kr_param_create(void)
{
    T_KRParam *ptParam = kr_calloc(sizeof(T_KRParam));
    if (ptParam == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptParam failed!");
        return NULL;
    }
    
    for (int i=0; i<N_MAX_SEC; ++i) {
        ptParam->ptHashTable[i] = kr_hashtable_new_full(
                    (KRHashFunc )kr_string_hash, 
                    (KREqualFunc )kr_string_equal, 
                    (KRDestroyNotify)NULL,
                    (KRDestroyNotify)kr_param_slot_free);
    }

    ptParam->psParamVersion = NULL;
    ptParam->nSecId = 0;

    return ptParam;
}


void kr_param_destroy(T_KRParam *ptParam)
{
    if (ptParam) {
        for (int i=0; i<N_MAX_SEC; ++i) {
            kr_hashtable_destroy(ptParam->ptHashTable[i]);
        }
        if (ptParam->psParamVersion) {
            kr_free(ptParam->psParamVersion);
        }
        kr_free(ptParam);
    }
}


short kr_param_switch(T_KRParam *ptParam)
{
    short nSecId = -1;
    
    nSecId = (ptParam->nSecId+1)%N_MAX_SEC;
    ptParam->nSecId = nSecId;
    
    ptParam->tLastLoadTime = time(NULL);
    
    return nSecId;
}


long kr_param_load_time(T_KRParam *ptParam)
{
    return (long )ptParam->tLastLoadTime;
}

char *kr_param_version_get(T_KRParam *ptParam)
{
    if (ptParam == NULL) {
        return NULL;
    }
    return ptParam->psParamVersion;
}

void kr_param_version_set(T_KRParam *ptParam, char *version)
{
    if (ptParam == NULL) {
        return;
    }
    if (ptParam->psParamVersion) {
        kr_free(ptParam->psParamVersion);
    }
    ptParam->psParamVersion = kr_strdup(version);
}

int kr_param_object_foreach(T_KRParam *ptParam, char *psParamClassName, KRParamObjectForEachFunc pfParamObjectForEach, void *data)
{
    short nCurrSecId = ptParam->nSecId;
    T_KRHashTable *ptCurrTable = ptParam->ptHashTable[nCurrSecId];
    
    T_KRParamSlot *ptParamSlot = kr_hashtable_lookup(ptCurrTable, psParamClassName);
    if (ptParamSlot == NULL) {
        KR_LOG(KR_LOGERROR, "param class name %s not found!", psParamClassName);
        return -1;
    }
    
    T_KRListNode *node = kr_list_first(ptParamSlot->ptParamObjectList);
    while(node)
    {
        T_KRParamObject *ptParamObject = kr_list_value(node);
        if (pfParamObjectForEach(psParamClassName, ptParamObject->psParamObjectKey, 
            ptParamObject->psParamObjectString, ptParamObject->ptParamObject, data) != 0) {
            KR_LOG(KR_LOGERROR, "call pfForEach %s failed!", ptParamObject->psParamObjectKey);
            return -1;
        }
        node = kr_list_next(node);
    }
    
    return 0;
}

void *kr_param_object_get(T_KRParam *ptParam, char *psParamClassName, char *psParamObjectKey)
{
    short nCurrSecId = ptParam->nSecId;
    T_KRHashTable *ptCurrTable = ptParam->ptHashTable[nCurrSecId];
    
    T_KRParamSlot *ptParamSlot = kr_hashtable_lookup(ptCurrTable, psParamClassName);
    if (ptParamSlot == NULL) {
        KR_LOG(KR_LOGERROR, "param class name %s not found!", psParamClassName);
        return NULL;
    }
    
    T_KRListNode *ptListNode = kr_list_search(ptParamSlot->ptParamObjectList, psParamObjectKey);
    if (ptListNode == NULL) {
        KR_LOG(KR_LOGERROR, "param object key %s not found!", psParamObjectKey);
        return NULL;
    }
    
    T_KRParamObject *ptParamObject = kr_list_value(ptListNode);
    return ptParamObject->ptParamObject;
}


int kr_param_object_add(T_KRParam *ptParam, char *psParamClassName, char *psParamObjectKey, char *ptParamObjectString)
{
    short nBackSecId = (ptParam->nSecId+1)%N_MAX_SEC;
    T_KRHashTable *ptBackTable = ptParam->ptHashTable[nBackSecId];
    
    /*first find class*/
    T_KRParamClass *ptParamClass = kr_param_class_get(psParamClassName);
    if (ptParamClass == NULL) {
        KR_LOG(KR_LOGERROR, "kr_param_class_get %s failed!", psParamClassName);
        return -1;
    }
    
    /*then find slot in table, create if not exists*/
    T_KRParamSlot *ptParamSlot = kr_hashtable_lookup(ptBackTable, psParamClassName);
    if (ptParamSlot == NULL) {
        ptParamSlot = kr_param_slot_new(ptParamClass);
        if (ptParamSlot == NULL) {
            KR_LOG(KR_LOGERROR, "kr_param_slot_new failed!");
            return -1;
        }
        kr_hashtable_insert(ptBackTable, ptParamClass->psParamClassName, ptParamSlot);
    }
    
    T_KRListNode *ptListNode = kr_list_search(ptParamSlot->ptParamObjectList, psParamObjectKey);
    if (ptListNode != NULL) {
        KR_LOG(KR_LOGERROR, "param object key %s already exists!", psParamObjectKey);
        return 1;
    }
    
    T_KRParamObject *ptParamObject = kr_param_object_new(
            ptParamSlot->ptParamClass, psParamObjectKey, ptParamObjectString);
    if (ptParamObject == NULL) {
        KR_LOG(KR_LOGERROR, "kr_param_object_new %s failed!", psParamObjectKey);
        return -1;
    }
    
    kr_list_add_sorted(ptParamSlot->ptParamObjectList, 
        ptParamObject, ptParamObject->psParamObjectKey);
}


void kr_param_object_del(T_KRParam *ptParam, char *psParamClassName, char *psParamObjectKey)
{
    short nBackSecId = (ptParam->nSecId+1)%N_MAX_SEC;
    T_KRHashTable *ptBackTable = ptParam->ptHashTable[nBackSecId];
    
    T_KRParamSlot *ptParamSlot = kr_hashtable_lookup(ptBackTable, psParamClassName);
    if (ptParamSlot != NULL) {
        kr_list_remove(ptParamSlot->ptParamObjectList, psParamObjectKey);
    }
}


