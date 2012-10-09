#include "krutils/kr_utils.h"
#include "krutils/kr_threadpool.h"
#include "krshm/kr_shm.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"
#include "krrule/kr_rule.h"
#include "dbs/dbs_basopr.h"
#include "kr_engine.h"

void *kr_engine_thread_init(void *env)
{
    /* Connect db */
    /*TODO:oracle mutli thread
	if (dbsDbConnect() != 0) {
	    KR_LOG(KR_LOGERROR, "dbsDbConnect failed!\n");
        return NULL;
	}
	*/
	
	/* intialize rule detecting context */
    T_KRContextEnv *krctxenv = (T_KRContextEnv *)env;
    T_KRContext *krcontext = kr_context_init(krctxenv);
    if (krcontext == NULL) {
        KR_LOG(KR_LOGERROR, "kr_context_init failed!\n");
        return NULL;
    }
    
    return krcontext;
}


int kr_engine_thread_worker(void *ctx, void *arg)
{
    T_KRContext *krcontext = (T_KRContext *)ctx;
    T_KRRecord *krcurrrec = (T_KRRecord *)arg;
    
    /* set context with arg */
    if (kr_context_set(krcontext, krcurrrec) != 0) {
        KR_LOG(KR_LOGERROR, "kr_context_set failed!\n");
        return -1;
    }

    /*rule detect*/
    if (kr_rule_group_detect(krcontext) != 0) {
        KR_LOG(KR_LOGERROR, "kr_rule_group_detect failed!\n");
        return -1;
    }
    
    /*TODO:write reponse*/
    
    return 0;
}


void kr_engine_thread_fini(void *ctx)
{
    /* finalize rule detecting context */
    T_KRContext *krcontext = (T_KRContext *)ctx;
    kr_context_fini(krcontext);
    
    /* disconnect db */
	/*
	dbsDbDisconnect();
	*/
}
