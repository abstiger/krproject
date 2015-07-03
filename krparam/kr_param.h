#ifndef __KR_PARAM_H__
#define __KR_PARAM_H__

typedef struct _kr_param_t T_KRParam;

typedef struct _kr_param_persist_config_t 
{
    char    type;   /* 'O' for odbc, 'F' for file */
    char   *version;            
    union {
        struct {
            char *name;
            char *user;
            char *pass;
        }odbc;
        
        char *file;
    }value;
}T_KRParamPersistConfig;


/*param function declaration*/
T_KRParam *kr_param_create(void);
void kr_param_destroy(T_KRParam *ptParam);
long kr_param_load_time(T_KRParam *ptParam);
char *kr_param_version_get(T_KRParam *ptParam);
void kr_param_version_set(T_KRParam *ptParam, char *version);
short kr_param_switch(T_KRParam *ptParam);

/*param persist function declaration*/
short kr_param_load(T_KRParam *ptParam, T_KRParamPersistConfig *ptConfig);
short kr_param_dump(T_KRParam *ptParam, T_KRParamPersistConfig *ptConfig);

/*param object function declaration*/
typedef int (*KRParamObjectForEachFunc)(char *psParamClassName, char *psParamObjectKey, char *psParamObjectString, void *ptParamObject, void *data);
int kr_param_object_foreach(T_KRParam *ptParam, char *psParamClassName, KRParamObjectForEachFunc pfParamObjectForEach, void *data);
void *kr_param_object_get(T_KRParam *ptParam, char *psParamClassName, char *psParamObjectKey);
int kr_param_object_add(T_KRParam *ptParam, char *psParamClassName, char *psParamObjectKey, char *ptParamObjectString);
void kr_param_object_del(T_KRParam *ptParam, char *psParamClassName, char *psParamObjectKey);

#endif  /*__KR_PARAM_H__*/
