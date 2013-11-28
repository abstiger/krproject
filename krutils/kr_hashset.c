#include "kr_utils.h"
#include "kr_hashset.h"

/*create a new set*/
T_KRHashSet *kr_hashset_create(char *name, char key_type)
{
    T_KRHashSet *krset = (T_KRHashSet *)kr_calloc(sizeof(T_KRHashSet));
    krset->name = (char *)kr_strdup(name);
    krset->type = key_type;
    KRHashFunc hash_func = kr_get_hash_func(key_type);
    KREqualFunc equal_func = kr_get_equal_func(key_type);
    krset->set = kr_hashtable_new_full(hash_func, equal_func, kr_free, NULL);
    
    return krset;
}

/* search an element in a krset
 * return TURE while found, FALSE for else
 */
kr_bool kr_hashset_search(T_KRHashSet *krset, const void *element)
{
    void *entry = kr_hashtable_lookup(krset->set, element);
    if (entry != NULL) {
        return TRUE;
    }
    return FALSE;
}

/* add an element
 * return 0 while success, -1 for else
 */
int kr_hashset_add(T_KRHashSet *krset, void *element)
{
    KRDupFunc dup_func = kr_get_dup_func(krset->type);
    void *dup_key = dup_func(element);
    kr_hashtable_insert(krset->set, dup_key, dup_key);
    return 0;
}

/* kr_hashset_remove an element
 * return 0 while success, -1 for else
 */
int kr_hashset_remove(T_KRHashSet *krset, const void *element)
{
    kr_hashtable_remove(krset->set, element);
    return 0;
}

/* destroy this krset */
void kr_hashset_destroy(T_KRHashSet *krset)
{
    if (krset != NULL) {
        kr_hashtable_destroy(krset->set);
        kr_free(krset->name);
        kr_free(krset); krset=NULL;
    }
}
