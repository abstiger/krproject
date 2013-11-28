#ifndef __KR_LIST_H__
#define __KR_LIST_H__

typedef void *(*KRListDupFunc)(void *data);
typedef void (*KRListFreeFunc)(void *data);
typedef int (*KRListMatchFunc)(const void *a, const void *b);
typedef int (*KRListCompareFunc)(const void *a, const void *b, void *data);
typedef void (*KRListForEachFunc)(void *value, void *data);

typedef struct _kr_list_node_t 
{
    struct _kr_list_node_t *prev;
    struct _kr_list_node_t *next;
    void *value;
}T_KRListNode;

typedef struct _kr_list_t 
{
    T_KRListNode *head;
    T_KRListNode *tail;
    KRListDupFunc dup;
    KRListFreeFunc free;
    KRListMatchFunc match;
    KRListCompareFunc compare;
    unsigned int len;
}T_KRList;

/* Functions implemented as macros */
#define kr_list_length(l) ((l)->len)
#define kr_list_first(l) ((l)->head)
#define kr_list_last(l) ((l)->tail)
#define kr_list_prev(n) ((n)->prev)
#define kr_list_next(n) ((n)->next)
#define kr_list_value(n) ((n)->value)

#define kr_list_set_dup(l,m) ((l)->dup = (m))
#define kr_list_set_free(l,m) ((l)->free = (m))
#define kr_list_set_match(l,m) ((l)->match = (m))
#define kr_list_set_compare(l,m) ((l)->compare = (m))

#define kr_list_get_dup(l) ((l)->dup)
#define kr_list_get_free(l) ((l)->free)
#define kr_list_get_match(l) ((l)->match)
#define kr_list_get_compare(l) ((l)->compare)

/* Prototypes */
T_KRList *kr_list_new(void);
void kr_list_destroy(T_KRList *list);
T_KRList *kr_list_add_head(T_KRList *list, void *value);
T_KRList *kr_list_add_tail(T_KRList *list, void *value);
T_KRList *kr_list_add_sorted(T_KRList *list, void *value, void *user_data);
void kr_list_delete(T_KRList *list, T_KRListNode *node);
T_KRList *kr_list_dup(T_KRList *orig);
void kr_list_foreach(T_KRList *list, KRListForEachFunc func, void *user_data);
T_KRListNode *kr_list_search(T_KRList *list, void *key);
T_KRListNode *kr_list_locate(T_KRList *list, int index);
void kr_list_remove(T_KRList *list, void *key);
void kr_list_remove_all(T_KRList *list, void *key);
void kr_list_move_head(T_KRList *list, T_KRListNode *node);

#endif /* __KR_LIST_H__ */
