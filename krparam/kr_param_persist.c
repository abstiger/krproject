#include "kr_param_persist.h"
#include "kr_param_class.h"
#include "krutils/kr_utils.h"

static T_KRParamPersistConfig *kr_param_persist_config_parse(char *psConfig);
static void kr_param_persist_config_free(T_KRParamPersistConfig *ptConfig);


T_KRParamPersistHandle gptParamPersistHandle[] = 
{
    {
        "odbc",
        kr_param_persist_odbc_load_pre,
        kr_param_persist_odbc_load,
        kr_param_persist_odbc_load_post,
        
        kr_param_persist_odbc_dump_pre,
        kr_param_persist_odbc_dump,
        kr_param_persist_odbc_dump_post
    },
    
    {
        "file",
        kr_param_persist_file_load_pre,
        kr_param_persist_file_load,
        kr_param_persist_file_load_post,
        
        kr_param_persist_file_dump_pre,
        kr_param_persist_file_dump,
        kr_param_persist_file_dump_post
    }
};


T_KRParamPersistHandle *kr_param_persist_handle_get(char *type)
{
    for (int i=0; i<sizeof(gptParamPersistHandle)/sizeof(T_KRParamPersistHandle); ++i) {
        T_KRParamPersistHandle *ptHandle = &gptParamPersistHandle[i];
        if (strcasecmp(ptHandle->type, type) == 0) {
            return ptHandle;
        }
    }
    
    return NULL;
}

T_KRParamPersistConfig *kr_param_persist_config_parse(char *psConfig)
{
    T_KRParamPersistConfig *ptConfig = kr_calloc(sizeof(*ptConfig));
    if (ptConfig == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptConfig failed!");
        return NULL;
    }

    char url[100+1];
    if (sscanf(psConfig, "%[^:]:%[^:]:%s", \
                ptConfig->type, ptConfig->version, url) != 3) {
        KR_LOG(KR_LOGERROR, "config format [%s] wrong!", psConfig);
        kr_param_persist_config_free(ptConfig);
        return NULL;
    }

    KR_LOG(KR_LOGDEBUG, "type:%s, version:%s, url:%s", ptConfig->type, ptConfig->version, url);

    if (strcasecmp(ptConfig->type, "odbc") == 0) {
        if (sscanf(url, "%[^/]/%[^@]@%s", \
                    ptConfig->value.odbc.user, 
                    ptConfig->value.odbc.pass, 
                    ptConfig->value.odbc.name) != 3) {
            KR_LOG(KR_LOGERROR, "config odbc format [%s] wrong!", url);
            kr_param_persist_config_free(ptConfig);
            return NULL;
        }
    } else if (strcasecmp(ptConfig->type, "file") == 0) {
        strcpy(ptConfig->value.file, url);
    } else {
        KR_LOG(KR_LOGERROR, "type [%s] wrong!", ptConfig->type);
        kr_param_persist_config_free(ptConfig);
        return NULL;
    }
    
    return ptConfig;
}


void kr_param_persist_config_free(T_KRParamPersistConfig *ptConfig)
{
    if (ptConfig) {
        kr_free(ptConfig);
    }
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
        KR_LOG(KR_LOGERROR, "pfLoad param version:%s class:%s failed!", 
                ptParamLoad->ptConfig->version ,
                ptParamClass->psParamClassName);
        return -1;
    }
    return 0;
}

short kr_param_load(T_KRParam *ptParam, char *psConfig)
{
    T_KRParamPersistConfig *ptConfig = kr_param_persist_config_parse(psConfig);
    if (ptConfig == NULL) {
        KR_LOG(KR_LOGERROR, "kr_param_persist_config_parse %s failed!", \
                psConfig);
        return -1;
    }
    
    /*get persist handle*/
    T_KRParamPersistHandle *ptHandle = kr_param_persist_handle_get(ptConfig->type);
    if (ptHandle == NULL) {
        KR_LOG(KR_LOGERROR, "kr_param_persist_handle_get %s failed!", \
                ptConfig->type);
        kr_param_persist_config_free(ptConfig);
        return -1;
    }

    /*open resource*/
    void *ptResource = ptHandle->pfLoadPre(ptConfig);
    if (ptResource == NULL) {
        KR_LOG(KR_LOGERROR, "call pfLoadPre failed!");
        kr_param_persist_config_free(ptConfig);
        return -1;
    }
    
    /*rebuild backup parameter hashtable */
    T_KRParamLoad stParamLoad = {ptParam, ptConfig, ptHandle, ptResource};
    if (kr_param_class_foreach(_kr_param_load_foreach, &stParamLoad) != 0) {
        KR_LOG(KR_LOGERROR, "kr_param_class_foreach load failed!");
        ptHandle->pfLoadPost(ptResource, ptConfig);
        kr_param_persist_config_free(ptConfig);
        return -1;
    }
    
    /*close resource*/
    ptHandle->pfLoadPost(ptResource, ptConfig);

    /*set parameter version*/
    kr_param_version_set(ptParam, ptConfig->version);
    
    /*free param persist config*/
    kr_param_persist_config_free(ptConfig);
    
    return kr_param_switch(ptParam);
}


typedef struct _kr_param_dump_t
{
    T_KRParam              *ptParam;
    T_KRParamPersistConfig *ptConfig;
    T_KRParamPersistHandle *ptHandle;
    void                   *ptResource;
}T_KRParamDump;

static int _kr_param_dump_object_foreach(char *psParamClassName, 
    char *psParamObjectKey, char *psParamObjectString, 
    void *ptParamObject, void *data)
{
    T_KRParamDump *ptParamDump = (T_KRParamDump *)data;
    
    if (ptParamDump->ptHandle->pfDump(
                ptParamDump->ptConfig->version,
                psParamClassName, 
                psParamObjectKey,
                psParamObjectString,
                ptParamDump->ptResource) != 0) {
        KR_LOG(KR_LOGERROR, "pfDump param version:%s class:%s key:%s failed!", 
                ptParamDump->ptConfig->version ,
                psParamClassName, psParamObjectKey);
        return -1;
    }

    return 0;
}

static int _kr_param_dump_foreach(T_KRParamClass *ptParamClass, void *data)
{
    T_KRParamDump *ptParamDump = (T_KRParamDump *)data;
    
    return kr_param_object_foreach(ptParamDump->ptParam, 
            ptParamClass->psParamClassName, 
            _kr_param_dump_object_foreach, 
            ptParamDump);
}

short kr_param_dump(T_KRParam *ptParam, char *psConfig)
{
    T_KRParamPersistConfig *ptConfig = kr_param_persist_config_parse(psConfig);
    if (ptConfig == NULL) {
        KR_LOG(KR_LOGERROR, "kr_param_persist_config_parse %s failed!", \
                psConfig);
        return -1;
    }

    /*get persist handle*/
    T_KRParamPersistHandle *ptHandle = kr_param_persist_handle_get(ptConfig->type);
    if (ptHandle == NULL) {
        KR_LOG(KR_LOGERROR, "kr_param_persist_handle_get %s failed!", \
                ptConfig->type);
        kr_param_persist_config_free(ptConfig);        
        return -1;
    }
        
    /*open resource*/
    void *ptResource = ptHandle->pfDumpPre(ptConfig);
    if (ptResource == NULL) {
        KR_LOG(KR_LOGERROR, "call pfDumpPre failed!");
        kr_param_persist_config_free(ptConfig);
        return -1;
    }
    
    /*traversal current parameter hashtable */
    T_KRParamDump stParamDump = {ptParam, ptConfig, ptHandle, ptResource};
    if (kr_param_class_foreach(_kr_param_dump_foreach, &stParamDump) != 0) {
        KR_LOG(KR_LOGERROR, "kr_param_class_foreach dump failed!");
        ptHandle->pfDumpPost(ptResource, ptConfig);
        kr_param_persist_config_free(ptConfig);
        return -1;
    }

    /*close resource*/
    ptHandle->pfDumpPost(ptResource, ptConfig);
    
    /*free param persist config*/
    kr_param_persist_config_free(ptConfig);
    
    return 0;
}
