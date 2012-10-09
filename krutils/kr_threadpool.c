#include "kr_threadpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void *thread_func(void *arg) 
{
    int ret = 0;
    void *context = NULL;
    T_KRThreadPool *tp = (T_KRThreadPool *)arg;
    
    if (tp->initfunc != NULL) context = tp->initfunc(tp->initenv);
    while(1) 
    {
        /*get task first*/
        void *task=kr_threadpool_get_task(tp);
        if (task == NULL) {
            fprintf(stderr, "kr_threadpool_get_task failed!\n");
            ret = -1;
        }
        
        /*then do it*/
        if (tp->workerfunc != NULL) {
            if (tp->workerfunc(context, task) != 0) {
                fprintf(stderr, "run workerfunc failed!\n");
                ret = -1;
            }
        }
    }
    if (tp->finifunc != NULL) tp->finifunc(context);
    
    return (void *)ret;
}


T_KRThreadPool *kr_threadpool_create(unsigned int nthreads, void *env, 
    KRThdInitFunc init_func, KRThdWorkerFunc worker_func, KRThdFiniFunc fini_func)
{
    assert(nthreads>0);
    
    T_KRThreadPool *tp = NULL;
    tp = (T_KRThreadPool *)kr_calloc(sizeof(T_KRThreadPool));
    if(tp == NULL) {
        fprintf(stderr, "calloc T_KRThreadPool error!\n");    
        return NULL;
    }
    
    /*initialize task queue of this thread pool*/
    if(kr_queue_init(&tp->queue, 1000) != 0) {
        fprintf(stderr, "kr_queue_init tp->queue error!\n");    
        return NULL;
    }
    
    /*set initalize environment*/
    tp->initenv = env;
    /*set initalize function*/
    tp->initfunc = init_func;
    /*set worker function*/
    tp->workerfunc = worker_func;
    /*set finalize function*/
    tp->finifunc = fini_func;
    
    /*set thread count of this threadpool*/
    tp->nthreads = nthreads;
    
    /*allocate memory for those threads*/
    tp->threads = (pthread_t *)kr_calloc(nthreads*sizeof(pthread_t));
    if(tp->threads == NULL) {
        fprintf(stderr, "calloc [%u] pthread_t error!\n", nthreads);
        kr_queue_fini(tp->queue); free(tp);
        return NULL;
    }
    
    /*mutex initial*/
    pthread_mutex_init(&tp->mutex, NULL);
    
    /*condition initial*/
    pthread_cond_init(&tp->cond, NULL);
    
    /*set threadpool status */
    tp->status = TP_INITIALIZED;
    
    return tp;
}


int kr_threadpool_run(T_KRThreadPool *tp) 
{
    assert(tp);
    
    int i;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    for (i = 0; i < tp->nthreads; i++) {
        pthread_create(&tp->threads[i], &attr, thread_func, (void *)tp);
    }
    
    pthread_attr_destroy(&attr);
    
    tp->status = TP_RUNNING;
    
    return 0;
}


int kr_threadpool_add_task(T_KRThreadPool *tp, void *task) 
{
    pthread_mutex_lock(&tp->mutex);
    
    /*push a task to the queue of this threadpool*/
    if (kr_queue_push(tp->queue, task) != 0)
    {
        fprintf(stderr, "kr_queue_push task failed!\n");
        pthread_mutex_unlock(&tp->mutex);
        return -1;
    }
    
    /*signal a workerthread to pop data*/
    pthread_cond_signal(&tp->cond);
    
    pthread_mutex_unlock(&tp->mutex);
    return 0;
}

void *kr_threadpool_get_task(T_KRThreadPool *tp) 
{
    pthread_mutex_lock(&tp->mutex);
    
    /*wait signal, need "while loop" to check the predicate*/
    while (kr_queue_length(tp->queue) == 0) {
        pthread_cond_wait(&tp->cond, &tp->mutex);
    }

    /*get a task from the queue of this threadpool*/
    void *task = kr_queue_pop(tp->queue);
    if (task == NULL) {
        fprintf(stderr, "kr_queue_pop task failed!\n");
        pthread_mutex_unlock(&tp->mutex);
        return NULL;
    }
    
    pthread_mutex_unlock(&tp->mutex);
    return task;
}


void kr_threadpool_destroy(T_KRThreadPool *tp) 
{
    pthread_mutex_lock(&tp->mutex);
    kr_free(tp->threads);
    kr_queue_fini(tp->queue);
    pthread_mutex_unlock(&tp->mutex);
    
    pthread_cond_destroy(&tp->cond);
    pthread_mutex_destroy(&tp->mutex);
    kr_free(tp);
}
    
