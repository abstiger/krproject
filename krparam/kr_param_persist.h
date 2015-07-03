#ifndef __KR_PARAM_PERSIST_H__
#define __KR_PARAM_PERSIST_H__

#include "kr_param.h"

typedef struct _kr_param_persist_handle_t 
{
    char    type; /* 'O' for odbc, 'F' for file */
    
    void*   (*pfLoadPre)(T_KRParamPersistConfig *ptConfig);
    int     (*pfLoad)(T_KRParam *ptParam, char *psParamVersion, char *psParamClassName, void *ptResource);
    void    (*pfLoadPost)(void *ptResource, T_KRParamPersistConfig *ptConfig);

    void*   (*pfDumpPre)(T_KRParamPersistConfig *ptConfig);
    int     (*pfDump)(char *psParamVersion, char *psParamClassName, char *psParamObjectKey, char *psParamObjectString, void *ptResource);
    void    (*pfDumpPost)(void *ptResource, T_KRParamPersistConfig *ptConfig);
}T_KRParamPersistHandle;


/*odbc persistence*/
void *kr_param_persist_odbc_load_pre(T_KRParamPersistConfig *ptConfig);
void kr_param_persist_odbc_load_post(void *ptResource, T_KRParamPersistConfig *ptConfig);
int kr_param_persist_odbc_load(T_KRParam *ptParam, char *psParamVersion, char *psParamClassName, void *ptResource);

void *kr_param_persist_odbc_dump_pre(T_KRParamPersistConfig *ptConfig);
void kr_param_persist_odbc_dump_post(void *ptResource, T_KRParamPersistConfig *ptConfig);
int kr_param_persist_odbc_dump(char *psParamVersion, char *psParamClassName, char *psParamObjectKey, char *psParamObjectString, void *ptResource);

/*file persistence*/
void *kr_param_persist_file_load_pre(T_KRParamPersistConfig *ptConfig);
void kr_param_persist_file_load_post(void *ptResource, T_KRParamPersistConfig *ptConfig);
int kr_param_persist_file_load(T_KRParam *ptParam, char *psParamVersion, char *psParamClassName, void *ptResource);

void *kr_param_persist_file_dump_pre(T_KRParamPersistConfig *ptConfig);
void kr_param_persist_file_dump_post(void *ptResource, T_KRParamPersistConfig *ptConfig);
int kr_param_persist_file_dump(char *psParamVersion, char *psParamClassName, char *psParamObjectKey, char *psParamObjectString, void *ptResource);




#endif  /*__KR_PARAM_PERSIST_H__*/
