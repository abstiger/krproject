#include "kr_types.h"
#include "kr_threadpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static T_KRThreadPoolTask *kr_threadpool_dup_task(T_KRThreadPoolTask *task)
{
    T_KRThreadPoolTask *dup_task = kr_malloc(sizeof(*dup_task));
    if (dup_task == NULL) {
        fprintf(stderr, "kr_malloc dup_task failed!\n");
        return NULL;
    }
    memcpy(dup_task, task, sizeof(*dup_task));
    if (task->arg && task->arg_size > 0) {
        dup_task->arg = kr_malloc(task->arg_size);
        if (dup_task->arg == NULL) {
            fprintf(stderr, "kr_malloc dup_task->arg failed!\n");
            kr_free(dup_task);
            return NULL;
        }
        memcpy(dup_task->arg, task->arg, task->arg_size);
    }
    return dup_task;
}

static void kr_threadpool_free_task(T_KRThreadPoolTask *task)
{
    if (task->arg && task->arg_size > 0) {
        kr_free(task->arg);
    }
    kr_free(task);
}

static void *thread_func(void *arg) 
{
    int ret = 0;
    void *context = NULL;
    T_KRThreadPool *tp = (T_KRThreadPool *)arg;
    
    if (tp->initfunc != NULL) context = tp->initfunc(tp->initenv);
    while(1) 
    {
        /*get task first*/
        T_KRThreadPoolTask *task = kr_threadpool_get_task(tp);
        if (task == NULL) {
            fprintf(stderr, "kr_threadpool_get_task failed!\n");
            ret = -1;
        }
        
        /*then do the task*/
        if (task->task_func != NULL) {
            if (task->task_func(context, task->arg) != 0) {
                fprintf(stderr, "run task_func failed!\n");
                ret = -1;
            }
        }

        /*free task final*/
        kr_threadpool_free_task(task);
    }
    if (tp->finifunc != NULL) tp->finifunc(context);
    
    return (void *)ret;
}


T_KRThreadPool *kr_threadpool_create(unsigned int nthreads, unsigned int hwm, 
        void *env, KRThdInitFunc init_func, KRThdFiniFunc fini_func)
{
    assert(nthreads>0);
    assert(hwm>0);
    
    T_KRThreadPool *tp = NULL;
    tp = (T_KRThreadPool *)kr_calloc(sizeof(T_KRThreadPool));
    if(tp == NULL) {
        fprintf(stderr, "calloc T_KRThreadPool error!\n");    
        return NULL;
    }
    
    /*initialize task queue of this thread pool*/
    if(kr_queue_init(&tp->queue, hwm) != 0) {
        fprintf(stderr, "kr_queue_init tp->queue error!\n");    
        return NULL;
    }
    
    /*set initalize environment*/
    tp->initenv = env;
    /*set initalize function*/
    tp->initfunc = init_func;
    /*set finalize function*/
    tp->finifunc = fini_func;
    
    /*set thread count of this threadpool*/
    tp->nthreads = nthreads;
    /*allocate memory for those threads*/
    tp->threads = (pthread_t *)kr_calloc(nthreads*sizeof(pthread_t));
    if(tp->threads == NULL) {
        fprintf(stderr, "calloc [%u] pthread_t error!\n", nthreads);
        kr_queue_fini(tp->queue); kr_free(tp);
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
    
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    for (int i = 0; i < tp->nthreads; i++) {
        pthread_create(&tp->threads[i], &attr, thread_func, (void *)tp);
    }
    
    pthread_attr_destroy(&attr);
    tp->status = TP_RUNNING;
    
    return 0;
}

int kr_threadpool_broadcast_task(T_KRThreadPool *tp, T_KRThreadPoolTask *task) 
{
    /*queue is busy, wait util the tide goes out*/
    while (kr_queue_length(tp->queue) >= tp->queue->hwm) {
       /*do nothing but wait*/ 
    }

    pthread_mutex_lock(&tp->mutex);
    /*push threads task to the queue of this threadpool*/
    for (int i=0; i<tp->nthreads; i++) {
        T_KRThreadPoolTask *task_copy = kr_threadpool_dup_task(task);
        if (task_copy == NULL) {
            pthread_mutex_unlock(&tp->mutex);
            return -1;
        }

        if (kr_queue_push(tp->queue, task_copy) != 0) {
            pthread_mutex_unlock(&tp->mutex);
            return -1;
        }
    }
    /*broadcast all threads to pop data*/
    pthread_cond_broadcast(&tp->cond);
    pthread_mutex_unlock(&tp->mutex);

    return 0;
}

int kr_threadpool_add_task(T_KRThreadPool *tp, T_KRThreadPoolTask *task) 
{
    /*queue is busy, wait util the tide goes out*/
    while (kr_queue_length(tp->queue) >= tp->queue->hwm) {
       /*do nothing but wait*/ 
    }

    pthread_mutex_lock(&tp->mutex);
    /*copy task first*/
    T_KRThreadPoolTask *task_copy = kr_threadpool_dup_task(task);
    if (task_copy == NULL) {
        pthread_mutex_unlock(&tp->mutex);
        return -1;
    }

    /*push it to queue*/
    if (kr_queue_push(tp->queue, task_copy) != 0) {
        pthread_mutex_unlock(&tp->mutex);
        return -1;
    }
    /*signal a workerthread to pop data*/
    pthread_cond_signal(&tp->cond);
    pthread_mutex_unlock(&tp->mutex);

    return 0;
}

T_KRThreadPoolTask *kr_threadpool_get_task(T_KRThreadPool *tp) 
{
    pthread_mutex_lock(&tp->mutex);
    /*wait signal, need "while loop" to check the predicate*/
    while (kr_queue_length(tp->queue) == 0) {
        pthread_cond_wait(&tp->cond, &tp->mutex);
    }
    /*get a task from the queue of this threadpool*/
    T_KRThreadPoolTask *task = kr_queue_pop(tp->queue);
    if (task == NULL) {
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
    
