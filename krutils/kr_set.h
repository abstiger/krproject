#ifndef __KR_SET_H__
#define __KR_SET_H__

/* kr_set is a simple wrapper of hashtable functions */
#include "kr_macros.h"
#include "kr_hashtable.h"

typedef struct _kr_set_t
{
    char            *name;
    char            type;
    T_KRHashTable   *set;
}T_KRSet;


T_KRSet *kr_set_create(char *name, char key_type);
int kr_set_search(T_KRSet *krset, const void *element);
int kr_set_add(T_KRSet *krset, void *element);
int kr_set_remove(T_KRSet *krset, const void *element);
void kr_set_destroy(T_KRSet *krset);

#endif /* __KR_SET_H__ */
