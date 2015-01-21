#ifndef __KR_HASH_SET_H__
#define __KR_HASH_SET_H__

/* kr_set is a simple wrapper of hashtable functions */
#include "kr_macros.h"
#include "kr_hashtable.h"

typedef struct _kr_hash_set_t
{
    char            *name;
    E_KRType         type;
    T_KRHashTable   *set;
}T_KRHashSet;


T_KRHashSet *kr_hashset_create(char *name, E_KRType key_type);
kr_bool kr_hashset_search(T_KRHashSet *krset, const void *element);
int kr_hashset_add(T_KRHashSet *krset, void *element);
int kr_hashset_remove(T_KRHashSet *krset, const void *element);
void kr_hashset_destroy(T_KRHashSet *krset);
void kr_hashset_dump(T_KRHashSet *krset, FILE *fp);
T_KRHashSet *kr_hashset_make(E_KRType key_type, char *multi_string);

#endif /* __KR_HASH_SET_H__ */
