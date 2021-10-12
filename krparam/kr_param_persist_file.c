#include "kr_param_persist.h"
#include "krutils/kr_utils.h"

void *kr_param_persist_file_load_pre(T_KRParamPersistConfig *ptConfig)
{
    char *psFileName = ptConfig->value.file;
    FILE *fp=fopen(psFileName,"rb");
    if (fp == NULL) {
        KR_LOG(KR_LOGERROR, "fopen %s failed!", psFileName);
        return NULL;
    }
    fseek(fp, 0, SEEK_END); long len=ftell(fp); fseek(fp, 0, SEEK_SET);
    char *data=(char*)kr_malloc(len+1); int n=fread(data,1,len,fp); fclose(fp);
    cJSON *root=cJSON_Parse(data); kr_free(data);
    if (root == NULL) {
        KR_LOG(KR_LOGERROR, "json parse %s failed! %s", data, cJSON_GetErrorPtr());
        return NULL;
    }
    char *version = cJSON_GetString(root, "version");
    if (strcmp(ptConfig->version, version) != 0) {
        KR_LOG(KR_LOGERROR, "version mismatch expected:%s actual:%s", 
        ptConfig->version, version);
        return NULL;
    }
    return root;
}


void kr_param_persist_file_load_post(void *ptResource, T_KRParamPersistConfig *ptConfig)
{
    cJSON *root = (cJSON *)ptResource;
    cJSON_Delete(root);
}


int kr_param_persist_file_load(T_KRParam *ptParam, char *psParamVersion, char *psParamClassName, void *ptResource)
{
    int nRet = 0;
    cJSON *root = (cJSON *)ptResource;
    
    cJSON *children = cJSON_GetObjectItem(root, psParamClassName);
    if (children == NULL) {
        KR_LOG(KR_LOGDEBUG, "no param class %s found!", psParamClassName);
        return nRet;
    }
    for (int i=0; i<cJSON_GetArraySize(children); ++i) {
        cJSON *child = cJSON_GetArrayItem(children, i);
        
        char *psParamObjectKey = cJSON_GetString(child, "param_object_key");
        char *psParamObjectString = cJSON_GetString(child, "param_object_string");
        
        nRet = kr_param_object_add(ptParam, psParamClassName, 
                psParamObjectKey, psParamObjectString);
        if (nRet != 0) {
            KR_LOG(KR_LOGERROR, "kr_param_object_add %s failed!", psParamObjectKey);
            return -1;
        }
    }

    return nRet;
}


void *kr_param_persist_file_dump_pre(T_KRParamPersistConfig *ptConfig)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "version", ptConfig->version);
    return root;
}

void kr_param_persist_file_dump_post(void *ptResource, T_KRParamPersistConfig *ptConfig)
{
    cJSON *root = (cJSON *)ptResource;
    char *psFileName = ptConfig->value.file;
    
    char *data = cJSON_Print(root); cJSON_Delete(root);
    FILE *fp=fopen(psFileName, "w"); fprintf(fp, "%s", data); fflush(fp); fclose(fp);
    kr_free(data);
}

int kr_param_persist_file_dump(char *psParamVersion, char *psParamClassName, char *psParamObjectKey, char *psParamObjectString, void *ptResource)
{
    cJSON *root = (cJSON *)ptResource;
    
    cJSON *children = cJSON_GetObjectItem(root, psParamClassName);
    if (children == NULL) {
        children = cJSON_CreateArray();
        cJSON_AddItemToObject(root, psParamClassName, children);
    }
    
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "param_object_key", psParamObjectKey);
    cJSON_AddStringToObject(child, "param_object_string", psParamObjectString);
    cJSON_AddItemToArray(children, child);
    
    return 0;
}
