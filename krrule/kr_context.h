#ifndef __KR_CONTEXT_H__
#define __KR_CONTEXT_H__

#include "krutils/kr_utils.h"
#include "krutils/kr_event.h"
#include "dbs/dbs_basopr.h"
#include "krshm/kr_shm.h"
#include "krdb/kr_db.h"
#include "kr_hdi_cache.h"
#include "kr_dynamicmem.h"


/* static environment, set while init */
struct _kr_context_env_t
{
    T_KREventLoop    *krEventLoop; /* event loop */
    T_KRModule       *krdbModule;  /* module for krdb */
    T_KRModule       *dataModule;  /* module for data */
    T_KRModule       *ruleModule;  /* module for rule */
    T_DbsEnv         *ptDbs;       /* db connection, one per thread */
    T_KRShareMem     *ptShm;       /* share memory, read only in thread */
    T_KRDB           *ptKRDB;      /* krdb, read only in thread */
    T_KRCache        *ptHDICache;  /* hdi cache, need lock while mutli thread */
};

typedef struct _kr_context_arg_t
{
    int              fd;          /* scoket fd of this connection */
    T_KRRecord       *ptCurrRec;  /* pointer to current record */
    void             *pData;      /* pointer to user's argument data */
    KRFreeFunc       DataFreeFunc;/* free function of user's data */
}T_KRContextArg;

typedef struct _kr_context_t
{
    T_KRContextEnv   *ptEnv;      /* pointer to environment */
    T_KRDynamicMem   *ptDym;      /* pointer to dynamic memory */
    T_KRContextArg   *ptArg;      /* pointer to arguments */
    
    T_KRRecord       *ptCurrRec;  /* the same as ptArg->ptCurrRec */
    E_KRFieldType    eKeyType;
    void             *pKeyValue;
    T_KRList         *ptRecList;
    T_KRRecord       *ptRecord;
    void             *ptExtra;    /* pointer to context's extra data */
}T_KRContext;

typedef enum {
    KR_RETRIEVE_DATASRC     = '0',  /*retrieve from datasrc*/
    KR_RETRIEVE_SELFDEF     = '1'   /*retrieve by self-define function*/
}E_KRRetrieveType;

/*field value union*/
typedef union {
    int         i;
    long        l;
    double      d;
    char        s[200];
}T_KRDataValue;

static inline time_t kr_get_transtime(T_KRRecord *ptRecord)
{
    time_t tTransTime;
    tTransTime = (time_t )*(long *)kr_get_field_value(\
                 ptRecord, KR_FIELDNO_TRANSTIME);
    return tTransTime;
}


E_KRFieldType kr_rule_get_type(char kind, int id, void *param);
void *kr_rule_get_value(char kind, int id, void *param);


T_KRContext *kr_context_init(T_KRContextEnv *ptEnv);
int kr_context_set(T_KRContext *ptContext, T_KRContextArg *ptContextArg);
void kr_context_free(T_KRContext *ptContext);



#endif /* __KR_CONTEXT_H__ */
