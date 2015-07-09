#include "kr_param_persist.h"
#include "kr_param_class.h"
#include "krutils/kr_utils.h"


T_KRParamPersistHandle gptParamPersistHandle[] = 
{
    {
        'O',
        kr_param_persist_odbc_load_pre,
        kr_param_persist_odbc_load,
        kr_param_persist_odbc_load_post,
        
        kr_param_persist_odbc_dump_pre,
        kr_param_persist_odbc_dump,
        kr_param_persist_odbc_dump_post
    },
    
    {
        'F',
        kr_param_persist_file_load_pre,
        kr_param_persist_file_load,
        kr_param_persist_file_load_post,
        
        kr_param_persist_file_dump_pre,
        kr_param_persist_file_dump,
        kr_param_persist_file_dump_post
    }
};


T_KRParamPersistHandle *kr_param_persist_handle_get(char type)
{
    for (int i=0; i<sizeof(gptParamPersistHandle)/sizeof(T_KRParamPersistHandle); ++i) {
        T_KRParamPersistHandle *ptHandle = &gptParamPersistHandle[i];
        if (ptHandle->type == type) {
            return ptHandle;
        }
    }
    
    return NULL;
}


typedef struct _kr_param_load_t
{
    T_KRParam              *ptParam;
    T_KRParamPersistConfig *ptConfig;
    T_KRParamPersistHandle *ptHandle;
    void                   *ptResource;
}T_KRParamLoad;

static int _kr_param_load_foreach(T_KRParamClass *ptParamClass, void *data)
{
    T_KRParamLoad *ptParamLoad = (T_KRParamLoad *)data;
    
    if (ptParamLoad->ptHandle->pfLoad(ptParamLoad->ptParam, 
        ptParamLoad->ptConfig->version, 
        ptParamClass->psParamClassName, 
        ptParamLoad->ptResource) != 0) {
        KR_LOG(KR_LOGERROR, "pfLoad psParamVersion:%s psParamClassName:%s failed!", 
            ptParamLoad->ptConfig->version , ptParamClass->psParamClassName);
        return -1;
    }
}

short kr_param_load(T_KRParam *ptParam, T_KRParamPersistConfig *ptConfig)
{
    /*get persist handle*/
    T_KRParamPersistHandle *ptHandle = kr_param_persist_handle_get(ptConfig->type);
    if (ptHandle == NULL) {
        KR_LOG(KR_LOGERROR, "kr_param_persist_handle_get %c failed!", ptConfig->type);
        return -1;
    }

    /*open resource*/
    void *ptResource = ptHandle->pfLoadPre(ptConfig);
    if (ptResource == NULL) {
        KR_LOG(KR_LOGERROR, "call pfLoadPre failed!");
        return -1;
    }
    
    /*rebuild backup parameter hashtable */
    T_KRParamLoad stParamLoad = {ptParam, ptConfig, ptHandle, ptResource};
    if (kr_param_class_foreach(_kr_param_load_foreach, &stParamLoad) != 0) {
        KR_LOG(KR_LOGERROR, "kr_param_class_foreach load failed!");
        ptHandle->pfLoadPost(ptResource, ptConfig);
        return -1;
    }
    
    /*close resource*/
    ptHandle->pfLoadPost(ptResource, ptConfig);

    /*set parameter version*/
    kr_param_version_set(ptParam, ptConfig->version);
    
    return kr_param_switch(ptParam);
}


typedef struct _kr_param_dump_t
{
    T_KRParam              *ptParam;
    T_KRParamPersistConfig *ptConfig;
    T_KRParamPersistHandle *ptHandle;
    void                   *ptResource;
}T_KRParamDump;

static int _kr_param_dump_object_foreach(char *psParamClassName, char *psParamObjectKey, char *psParamObjectString, void *ptParamObject, void *data)
{
    T_KRParamDump *ptParamDump = (T_KRParamDump *)data;
    
    return ptParamDump->ptHandle->pfDump(
                ptParamDump->ptConfig->version,
                psParamClassName, 
                psParamObjectKey,
                psParamObjectString,
                ptParamDump->ptResource);
}

static int _kr_param_dump_foreach(T_KRParamClass *ptParamClass, void *data)
{
    T_KRParamDump *ptParamDump = (T_KRParamDump *)data;
    
    return kr_param_object_foreach(ptParamDump->ptParam, 
        ptParamClass->psParamClassName, _kr_param_dump_object_foreach, ptParamDump);
}

short kr_param_dump(T_KRParam *ptParam, T_KRParamPersistConfig *ptConfig)
{
    /*get persist handle*/
    T_KRParamPersistHandle *ptHandle = kr_param_persist_handle_get(ptConfig->type);
    if (ptHandle == NULL) {
        KR_LOG(KR_LOGERROR, "kr_param_persist_handle_get %c failed!", ptConfig->type);
        return -1;
    }
        
    /*open resource*/
    void *ptResource = ptHandle->pfDumpPre(ptConfig);
    if (ptResource == NULL) {
        KR_LOG(KR_LOGERROR, "call pfDumpPre failed!");
        return -1;
    }
    
    /*traversal current parameter hashtable */
    T_KRParamDump stParamDump = {ptParam, ptConfig, ptHandle, ptResource};
    if (kr_param_class_foreach(_kr_param_dump_foreach, &stParamDump) != 0) {
        KR_LOG(KR_LOGERROR, "kr_param_class_foreach dump failed!");
        ptHandle->pfDumpPost(ptResource, ptConfig);
        return -1;
    }

    /*close resource*/
    ptHandle->pfDumpPost(ptResource, ptConfig);
    
    return 0;
}
