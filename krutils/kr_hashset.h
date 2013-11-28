#ifndef __KR_HASH_SET_H__
#define __KR_HASH_SET_H__

/* kr_set is a simple wrapper of hashtable functions */
#include "kr_macros.h"
#include "kr_hashtable.h"

typedef struct _kr_hash_set_t
{
    char            *name;
    char            type;
    T_KRHashTable   *set;
}T_KRHashSet;


T_KRHashSet *kr_hashset_create(char *name, char key_type);
kr_bool kr_hashset_search(T_KRHashSet *krset, const void *element);
int kr_hashset_add(T_KRHashSet *krset, void *element);
int kr_hashset_remove(T_KRHashSet *krset, const void *element);
void kr_hashset_destroy(T_KRHashSet *krset);

#endif /* __KR_HASH_SET_H__ */
