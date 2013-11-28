#include <stdlib.h>

#include "kr_alloc.h"
#include "kr_list.h"

/* Create a new list. The created list can be freed with
 * AlFreeList(), but private value of every node need to be freed
 * by the user before to call AlFreeList().
 *
 * On error, NULL is returned. Otherwise the pointer to the new list. */
T_KRList *kr_list_new(void)
{
    T_KRList *list;
    
    list = (T_KRList *)kr_malloc(sizeof(*list));
    if (list == NULL)
        return NULL;
    list->head = list->tail = NULL;
    list->len = 0;
    list->dup = NULL;
    list->free = NULL;
    list->match = NULL;
    list->compare = NULL;
    return list;
}

/* Free the whole list.
 *
 * This function can't fail. */
void kr_list_destroy(T_KRList *list)
{
    unsigned int len;
    T_KRListNode *current, *next;

    current = list->head;
    len = list->len;
    while(len--) {
        next = current->next;
        if (list->free) list->free(current->value);
        kr_free(current);
        current = next;
    }
    kr_free(list);
}

/* Add a new node to the list, to head, contaning the specified 'value'
 * pointer as value.
 *
 * On error, NULL is returned and no operation is performed (i.e. the
 * list remains unaltered).
 * On success the 'list' pointer you pass to the function is returned. */
T_KRList *kr_list_add_head(T_KRList *list, void *value)
{
    T_KRListNode *node;

    if ((node = kr_malloc(sizeof(*node))) == NULL)
        return NULL;
    node->value = value;
    if (list->len == 0) {
        list->head = list->tail = node;
        node->prev = node->next = NULL;
    } else {
        node->prev = NULL;
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }
    list->len++;
    return list;
}

/* Add a new node to the list, to tail, contaning the specified 'value'
 * pointer as value.
 *
 * On error, NULL is returned and no operation is performed (i.e. the
 * list remains unaltered).
 * On success the 'list' pointer you pass to the function is returned. */
T_KRList *kr_list_add_tail(T_KRList *list, void *value)
{
    T_KRListNode *node;

    if ((node = kr_malloc(sizeof(*node))) == NULL)
        return NULL;
    node->value = value;
    if (list->len == 0) {
        list->head = list->tail = node;
        node->prev = node->next = NULL;
    } else {
        node->prev = list->tail;
        node->next = NULL;
        list->tail->next = node;
        list->tail = node;
    }
    list->len++;
    return list;
}


/* Add a new node to the list, with sorted, contaning the specified 'value'
 * pointer as value.
 * we start to add the new node from the tail of list.
 *
 * On error, NULL is returned and no operation is performed (i.e. the
 * list remains unaltered).
 * On success the 'list' pointer you pass to the function is returned. */
T_KRList *kr_list_add_sorted(T_KRList *list, void *value, void *user_data)
{
    T_KRListNode *node;

    if ((node = kr_malloc(sizeof(*node))) == NULL)
        return NULL;
    node->value = value;
    if (list->len == 0) {
        list->head = list->tail = node;
        node->prev = node->next = NULL;
    } else {
        T_KRListNode *curr = list->tail;
        T_KRListNode *next = curr->next;
        
        while(curr != NULL)
        {
            if (list->compare && 
                list->compare(node->value, curr->value, user_data) >=0) 
            {
                break;
            }
            next = curr; curr = curr->prev;
        }
        
        node->prev = curr;
        node->next = next;
        
        if (node->prev != NULL) {
            node->prev->next = node;
        } else {
            list->head = node;
        }
        
        if (node->next != NULL) {
            node->next->prev = node;
        } else {
            list->tail = node;
        }
    }
    list->len++;
    return list;
}


/* Remove the specified node from the specified list.
 * It's up to the caller to free the private value of the node.
 *
 * This function can't fail. */
void kr_list_delete(T_KRList *list, T_KRListNode *node)
{
    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;
    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;
    if (list->free) list->free(node->value);
    kr_free(node);
    list->len--;
}

/* Duplicate the whole list. On out of memory NULL is returned.
 * On success a copy of the original list is returned.
 *
 * The 'Dup' method set with kr_list_set_dup() function is used
 * to copy the node value. Otherwise the same pointer value of
 * the original node is used as value of the copied node.
 *
 * The original list both on success or error is never modified. */
T_KRList *kr_list_dup(T_KRList *orig)
{
    T_KRList *copy;
    T_KRListNode *node = orig->head;

    if ((copy = kr_list_new()) == NULL)
        return NULL;
    copy->dup = orig->dup;
    copy->free = orig->free;
    copy->match = orig->match;
    copy->compare = orig->compare;
    
    while(node) 
    {
        void *value;

        if (copy->dup) {
            value = copy->dup(node->value);
            if (value == NULL) {
                kr_list_destroy(copy);
                return NULL;
            }
        } else
            value = node->value;
        if (kr_list_add_tail(copy, value) == NULL) {
            kr_list_destroy(copy);
            return NULL;
        }
        node = node->next;
    }
    return copy;
}

/* Traversal the list,dealing value with the function specified */
void kr_list_foreach(T_KRList *list, KRListForEachFunc func, void *user_data)
{
    T_KRListNode *node, *next;
    
    node = list->head;
    while(node)
    {
        next = node->next;
        if (func) {
            func(node->value, user_data);
        }
        node = next;
    }
}

/* Search the list for a node matching a given key.
 * The match is performed using the 'match' method
 * set with kr_list_set_match(). If no 'match' method
 * is set, the 'value' pointer of every node is directly
 * compared with the 'key' pointer.
 *
 * On success the first matching node pointer is returned
 * (search starts from head). If no matching node exists
 * NULL is returned. */
T_KRListNode *kr_list_search(T_KRList *list, void *key)
{
    T_KRListNode *node = list->head;

    while(node) 
    {
        if (list->match) {
            if (list->match(node->value, key)) {
                return node;
            }
        } else {
            if (key == node->value) {
                return node;
            }
        }
        node = node->next;
    }
    return NULL;
}

/* Return the element at the specified zero-based index
 * where 0 is the head, 1 is the element next to head
 * and so on. Negative integers are used in order to count
 * from the tail, -1 is the last element, -2 the penultimante
 * and so on. If the index is out of range NULL is returned. */
T_KRListNode *kr_list_locate(T_KRList *list, int index) 
{
    T_KRListNode *n;

    if (index < 0) {
        index = (-index)-1;
        n = list->tail;
        while(index-- && n) n = n->prev;
    } else {
        n = list->head;
        while(index-- && n) n = n->next;
    }
    return n;
}

/* Remove the first node with key from the specified list.
 * It's up to the caller to free the private value of the node.
 */
void kr_list_remove(T_KRList *list, void *key)
{
    T_KRListNode *node = list->head;

    while(node) 
    {
        if (list->match) {
            if (list->match(node->value, key)) {
                kr_list_delete(list, node);
                return;
            }
        } else {
            if (key == node->value) {
                kr_list_delete(list, node);
                return;
            }
        }
        node = node->next;
    }
}

/* Remove all nodes with key from the specified list.
 * It's up to the caller to free the private value of the node.
 */
void kr_list_remove_all(T_KRList *list, void *key)
{
    T_KRListNode *node = list->head;

    while(node) 
    {
        if (list->match) {
            if (list->match(node->value, key)) {
                kr_list_delete(list, node);
            }
        } else {
            if (key == node->value) {
                kr_list_delete(list, node);
            }
        }
        node = node->next;
    }
}


/* Move the specified node to the head of list.
 *
 * This function can't fail. */
void kr_list_move_head(T_KRList *list, T_KRListNode *node)
{
    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;
    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;
    
    node->prev = NULL;
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
}
