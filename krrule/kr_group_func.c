#include "kr_group.h"
#include "kr_group_func.h"
#include "krutils/kr_json.h"


void kr_group_write_handler(T_KREventLoop *el, int fd, void *privdata, int mask)
{
    /* this is quite important for event driven programming */
    if (privdata == NULL) return; 
    char *jsonstr = (char *)privdata;

    KR_LOG(KR_LOGDEBUG, "fd=[%d], jsonstr: %s ", fd, jsonstr);

    /* send jsonstr back to client */
    int nwrite = kr_net_write(fd, jsonstr, strlen(jsonstr));
    if (nwrite != strlen(jsonstr)) {
        KR_LOG(KR_LOGERROR, "kr_net_write[%d] jsonstr[%s] error[%d]!", \
                fd, jsonstr, nwrite);
    }
    
    /* free json string */
    kr_event_file_delete(el, fd, AE_WRITABLE);
    kr_free(privdata);
    privdata=NULL;
}


int kr_group_func(T_KRGroup *krgroup, T_KRContext *krcontext)
{
    int nret = 0;

    /*set context's extra data with json format*/
    if (krcontext->ptExtra == NULL) {
        krcontext->ptExtra = cJSON_CreateArray();
    }
    cJSON_AddNumberToObject(krcontext->ptExtra, "groupid", krgroup->lGroupId);

    char *jsonstr = cJSON_PrintUnformatted(krcontext->ptExtra);
    if (kr_event_file_create(krcontext->ptEnv->krEventLoop, 
                krcontext->ptArg->fd, KR_EVENT_WRITABLE, 
                kr_group_write_handler, jsonstr) == KR_NET_ERR) 
    {
        KR_LOG(KR_LOGERROR, "kr_event_file_create error!");
        kr_free(jsonstr);
        nret = -1;
    }

    /*free extra data*/
    cJSON_Delete(krcontext->ptExtra);
    krcontext->ptExtra = NULL;
    return 0;
}
