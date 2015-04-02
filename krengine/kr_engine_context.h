#ifndef __KR_CONTEXT_H__
#define __KR_CONTEXT_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"
#include "krdata/kr_data.h"
#include "krflow/kr_flow.h"
#include "kriface/kr_iface.h"

/* static environment, set while init */
typedef struct _kr_context_env_t
{
    T_KRModule       *krdbModule;  /* module for krdb */
    T_KRModule       *dataModule;  /* module for data */
    T_KRModule       *ruleModule;  /* module for rule */
    T_DbsEnv         *ptDbsEnv;    /* db connection */
    T_KRParam        *ptParam;     /* parameter, read only in thread */
    T_KRIface        *ptIface;     /* interface module */
    T_KRDB           *ptDB;        /* krdb, read only in thread */
    T_KRCache        *ptHDICache;  /* hdi cache, need lock while mutli thread */
    T_KRFuncTable    *ptFuncTable; /* function table */
    void             *extra;       /* engine startup extra data */
}T_KRContextEnv;

struct _kr_context_t
{
    T_KRContextEnv   *ptEnv;      /* pointer to environment */
    T_DbsEnv         *ptDbsEnv;   /* db connection, one per thread */
    T_KRData         *ptData;     /* pointer to dynamic memory */
    T_KRFlow         *ptFlow;     /* pointer to dynamic memory */

    void             *ptArg;      /* argument of krengine run */
    T_KRRecord       *ptCurrRec;  /* used for C_* */
};

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
