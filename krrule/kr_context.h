#ifndef __KR_CONTEXT_H__
#define __KR_CONTEXT_H__

#include "krutils/kr_utils.h"
#include "krshm/kr_shm.h"
#include "krdb/kr_db.h"
#include "kr_hdi_cache.h"
#include "kr_dynamicmem.h"


/* static environment, set while init */
typedef struct _kr_context_env_t
{
    T_KRShareMem     *ptShm;      /* pointer to share memory */
    T_KRDB           *ptKRDB;     /* pointer to krdb */
    T_KRCache        *ptHDICache; /* pointer to hdi cache */
}T_KRContextEnv;

typedef struct _kr_context_t
{
    T_KRContextEnv   *ptEnv;      /* pointer to environment */

    T_KRDynamicMem   *ptDym;      /* pointer to dynamic memory */
    T_KRRecord       *ptCurrRec;  /* pointer to current record */
    void             *ptExtra;    /* pointer to user's extra data */
    
    E_KRFieldType    eKeyType;
    void             *pKeyValue;
    T_KRList         *ptRecList;
    T_KRRecord       *ptRecord;
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


E_KRFieldType kr_rule_get_type(char *id, void *param);
void *kr_rule_get_value(char *id, void *param);


T_KRContext *kr_context_init(T_KRContextEnv *ptEnv);
int kr_context_set(T_KRContext *ptContext, T_KRRecord *ptCurrRec);
void kr_context_free(T_KRContext *ptContext);



#endif /* __KR_CONTEXT_H__ */
