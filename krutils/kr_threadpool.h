#ifndef __KR_THREADPOOL_H__
#define __KR_THREADPOOL_H__

#include <pthread.h>
#include "kr_queue.h"

typedef void* (*KRThdInitFunc)(void *env);
typedef int (*KRThdWorkerFunc)(void *ctx, void *arg);
typedef void (*KRThdFiniFunc)(void *ctx);

typedef enum {
    TP_INITIALIZED = '0', 
    TP_SETED, 
    TP_RUNNING,
    TP_SUSPEND,
    TP_FINALIZED
}TP_STATUS;

typedef struct _kr_threadpool_t 
{
    kr_queue_t          *queue;      /*task queue*/
    void                *initenv;    /*environment for initializing*/
    KRThdInitFunc       initfunc;    /*function to initial the thread*/
    KRThdWorkerFunc     workerfunc;  /*function to with the task*/
    KRThdFiniFunc       finifunc;    /*function to finalize the thread*/
    unsigned int        nthreads;    /*thread number of this pool*/
    pthread_t           *threads;    /*an array stores all threads*/
    pthread_mutex_t     mutex;       /*mutex of this threadpool*/
    pthread_cond_t      cond;        /*manager tells workers got a new item!*/
    TP_STATUS           status;      /*status of this threadpool*/
}T_KRThreadPool;


/*create a threadpool with <n> threads and <func> thread process function*/
T_KRThreadPool *kr_threadpool_create(unsigned int nthreads, unsigned int hwm, void *env, 
    KRThdInitFunc init_func, KRThdWorkerFunc worker_func, KRThdFiniFunc fini_func);

/*run all of the threads*/
int kr_threadpool_run(T_KRThreadPool *tp);

/*add a task to the queue, main thread invoke this function*/
int kr_threadpool_add_task(T_KRThreadPool *tp, void *task);

/*get a task from the queue, workerthreads invoke this function*/
void *kr_threadpool_get_task(T_KRThreadPool *tp);

/*destroy this threadpool*/
void kr_threadpool_destroy(T_KRThreadPool *tp);

#endif /*__KR_THREADPOOL_H__*/
