#ifndef __KR_CONTEXT_H__
#define __KR_CONTEXT_H__

#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"
#include "krparam/kr_param.h"
#include "krdb/kr_db.h"
#include "kr_hdi_cache.h"

/* static environment, set while init */
typedef struct _kr_context_env_t
{
    T_KRModule       *krdbModule;  /* module for krdb */
    T_KRModule       *dataModule;  /* module for data */
    T_KRModule       *ruleModule;  /* module for rule */
    T_DbsEnv         *ptDbs;       /* db connection */
    T_KRShareMem     *ptShm;       /* share memory, read only in thread */
    T_KRDB           *ptKRDB;      /* krdb, read only in thread */
    T_KRCache        *ptHDICache;  /* hdi cache, need lock while mutli thread */
    T_KRFuncTable    *ptFuncTable; /* function table */
    void             *data;        /* engine startup extra data */
}T_KRContextEnv;

typedef struct _kr_context_t
{
    T_KRContextEnv   *ptEnv;      /* pointer to environment */
    T_DbsEnv         *ptDbs;      /* db connection, one per thread */
    T_KRDynamicMem   *ptDym;      /* pointer to dynamic memory */

    void             *ptArg;      /* argument of krengine run */

    T_KRRecord       *ptCurrRec;  /* used for C_* */
    T_KRGroup        *ptGroup;    /* routed group of current record */
    E_KRType          eKeyType;
    void             *pKeyValue;
    T_KRList         *ptRecList;
    T_KRRecord       *ptRecord;   /* used for F_* */
}T_KRContext;

typedef enum {
    KR_RETRIEVE_DATASRC     = '0',  /*retrieve from datasrc*/
    KR_RETRIEVE_SELFDEF     = '1'   /*retrieve by self-define function*/
}E_KRRetrieveType;


T_KRContext *kr_context_init(T_KRContextEnv *ptEnv);
int kr_context_set(T_KRContext *ptContext, void *ptArg);
void kr_context_clean(T_KRContext *ptContext);
void kr_context_fini(T_KRContext *ptContext);
cJSON *kr_context_dump_json(T_KRContext *ptContext);

#endif /* __KR_CONTEXT_H__ */
