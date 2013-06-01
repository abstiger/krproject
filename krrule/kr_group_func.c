#include <Python.h>

#include "kr_group.h"
#include "kr_group_func.h"
#include "krutils/kr_json.h"

int weibo_post_status(void *data, char *text)
{
    PyObject *client;

    client = Py_BuildValue("O", data);
    PyObject_CallMethod(client, "post_status", "(s)", text);
    Py_DECREF(client);
    return 0;
}

int weibo_repost_status(void *data, double id, char *text)
{
    PyObject *client;

    client = Py_BuildValue("O", data);
    PyObject_CallMethod(client, "repost_status", "(Ls)", (long long )id, text);
    Py_DECREF(client);
    return 0;
}

int weibo_post_comment(void *data, double id, char *text)
{
    PyObject *client;

    client = Py_BuildValue("O", data);
    PyObject_CallMethod(client, "post_comment", "(Ls)", (long long )id, text);
    Py_DECREF(client);
    return 0;
}


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
    /*set context's extra data with json format*/
    if (krcontext->ptExtra == NULL) {
        printf("no rules fired!\n");
        return 0;
    }

    cJSON_AddNumberToObject(krcontext->ptExtra, "groupid", krgroup->lGroupId);
    char *jsonstr = cJSON_PrintUnformatted(krcontext->ptExtra);
    double id = *(double *)kr_get_field_value(krcontext->ptArg->ptCurrRec, 13);
    char repost[]="来自krproject:repost status test";
    char comment[]="来自krproject:post comment test";
    //weibo_post_status(krcontext->ptArg->pExtra, jsonstr);
    weibo_post_comment(krcontext->ptArg->pExtra, id, comment);
    //weibo_repost_status(krcontext->ptArg->pExtra, id, repost);
    printf("fired rules: %s\n", jsonstr);
    KR_LOG(KR_LOGDEBUG, "result json: %s ", jsonstr);
    kr_free(jsonstr);
    /*
    if (kr_event_file_create(krcontext->ptEnv->krEventLoop, 
                krcontext->ptArg->fd, KR_EVENT_WRITABLE, 
                kr_group_write_handler, jsonstr) == KR_NET_ERR) 
    {
        KR_LOG(KR_LOGERROR, "kr_event_file_create error!");
        kr_free(jsonstr);
        nret = -1;
    }
    */

    /*free extra data*/
    cJSON_Delete(krcontext->ptExtra);
    krcontext->ptExtra = NULL;
    return 0;
}
