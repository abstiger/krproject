#include "kr_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int kr_queue_init(kr_queue_t **q, unsigned int hwm) 
{
    /*alloc queue memory*/
    *q = (kr_queue_t *)kr_calloc(sizeof(kr_queue_t));
    if(*q == NULL) {
        fprintf(stderr, "calloc kr_queue_t error!\n");
        return -1;
    }

    /*list initial*/
    (*q)->list = kr_list_new();
    (*q)->hwm = hwm;
        
    return 0;
}

int kr_queue_push(kr_queue_t *q, void *data)
{
    assert(q);
    
    /*check current queue length*/
    if (kr_list_length(q->list) >= q->hwm) {
        fprintf(stderr, "queue length [%u] >= high watermark[%u]\n", \
                kr_list_length(q->list), q->hwm);
        return -1;
    }

    /*add this node to queue's tail*/
    kr_list_add_tail(q->list, data);
        
    return 0;
}


void *kr_queue_pop(kr_queue_t *q) 
{
    assert(q);
    
    /*check queue's length*/
    if (kr_list_length(q->list) <= 0) {
        return NULL;
    }
    
    /*get the queue's head then deleted*/
    T_KRListNode *node = NULL;
    node = kr_list_first(q->list);
    if (node == NULL) {
        return NULL;
    }
    
    /*free node*/
    void *value = node->value;
    kr_list_delete(q->list, node);
    
    return value;
}


void kr_queue_fini(kr_queue_t *q)
{
    if (q) {
        kr_list_destroy(q->list);
        kr_free(q); q=NULL;
    }
}


unsigned int kr_queue_length(kr_queue_t *q)
{
    return kr_list_length(q->list);
}


int kr_queue_is_empty(kr_queue_t *q)
{
    return (kr_list_length(q->list) == 0);
}
