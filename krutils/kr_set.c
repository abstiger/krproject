#include "kr_utils.h"
#include "kr_set.h"

/*create a new set*/
T_KRSet *kr_set_create(char *name, char key_type)
{
    T_KRSet *krset = (T_KRSet *)kr_calloc(sizeof(T_KRSet));
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
boolean kr_set_search(T_KRSet *krset, const void *element)
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
int kr_set_add(T_KRSet *krset, void *element)
{
    KRDupFunc dup_func = kr_get_dup_func(krset->type);
    void *dup_key = dup_func(element);
    kr_hashtable_insert(krset->set, dup_key, dup_key);
    return 0;
}

/* kr_set_remove an element
 * return 0 while success, -1 for else
 */
int kr_set_remove(T_KRSet *krset, const void *element)
{
    kr_hashtable_remove(krset->set, element);
    return 0;
}

/* destroy this krset */
void kr_set_destroy(T_KRSet *krset)
{
    if (krset != NULL) {
        kr_hashtable_destroy(krset->set);
        kr_free(krset->name);
        kr_free(krset); krset=NULL;
    }
}