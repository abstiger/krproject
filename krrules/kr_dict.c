/* kr_dict is a simple wrapper of redis's dict functions
 * check README for the Copyright
 */
#include <stdio.h>
#include "kr_dict.h"

/* dict functions for type of integer*/
static inline unsigned int kr_int_hash (const void *key)
{
    return *(const int*) key;
}

static inline int kr_int_compare(void *privdata, \
                               const void *key1, const void *key2)
{
    return *((const int*) key1) == *((const int*) key2);
}

static inline void *kr_int_dup(void *privdata, const void *key)
{
    int *d = (int *)zmalloc(sizeof(int));
    *d = *(int *)key;
    return d;
}

static inline void kr_int_destruct(void *privdata, void *key)
{
    zfree(key);
}

/* dict functions for type of double*/
static inline unsigned int kr_double_hash (const void *key)
{
    return (unsigned int) *(const double*) key;
}

static inline int kr_double_compare(void *privdata, \
                                const void *key1, const void *key2)
{
    return *((const double*) key1) == *((const double*) key2);
}

static inline void *kr_double_dup(void *privdata, const void *key)
{
    double *d = (double *)zmalloc(sizeof(double));
    *d = *(double *)key;
    return d;
}

static inline void kr_double_destruct(void *privdata, void *key)
{
    zfree(key);
}

/* dict functions for type of string*/
static inline unsigned int kr_string_hash(const void *key)
{
    const signed char *p;
    unsigned int h = 5381;
    for (p = key; *p != '\0'; p++)
        h = (h << 5) + h + *p;
    return h;
}

static inline int kr_string_compare(void *privdata, \
                                const void *key1, const void *key2)
{
    const char *str1 = key1;
    const char *str2 = key2;
    return (strcmp(str1, str2) == 0);
}                                

static inline void *kr_string_dup(void *privdata, const void *key)
{
    return (void *)zstrdup(key);
}

static inline void kr_string_destruct(void *privdata, void *key)
{
    zfree(key);
}

/*declaration for dict which key's type is Integer*/
dictType krdictTypeInteger = {
    kr_int_hash,                   /* hash function */
    kr_int_dup,                    /* key dup */
    NULL,                          /* val dup */
    kr_int_compare,                /* key compare */
    kr_int_destruct,               /* key destructor */
    NULL                           /* val destructor */
};

/*declaration for dict which key's type is Double*/
dictType krdictTypeDouble = {
    kr_double_hash,                /* hash function */
    kr_double_dup,                 /* key dup */
    NULL,                          /* val dup */
    kr_double_compare,             /* key compare */
    kr_double_destruct,            /* key destructor */
    NULL                           /* val destructor */
};

/*declaration for dict which key's type is String*/
dictType krdictTypeString = {
    kr_string_hash,                /* hash function */
    kr_string_dup,                 /* key dup */
    NULL,                          /* val dup */
    kr_string_compare,             /* key compare */
    kr_string_destruct,            /* key destructor */
    NULL                           /* val destructor */
};


/*create a new dict*/
KRDict *kr_dict_create(char *name, KRType keytype)
{
    KRDict *krdict = (KRDict *)zcalloc(sizeof(KRDict));
    krdict->name = (char *)zstrdup(name);
    krdict->type = keytype;
    switch(krdict->type)
    {
        case KR_INTEGER:
            krdict->dict = dictCreate(&krdictTypeInteger, NULL);
            break;
        case KR_DOUBLE:
            krdict->dict = dictCreate(&krdictTypeDouble, NULL);
            break;
        case KR_STRING:
            krdict->dict = dictCreate(&krdictTypeString, NULL);
            break;
        default:
            fprintf(stderr, "Unknown dict Type[%d]\n", krdict->type);
            zfree(krdict->name); zfree(krdict); krdict=NULL;
            break;
    }
    return krdict;
}

/* search an element in a krdict
 * return KR_TURE while founded, KR_FALSE for else
 */
int kr_dict_search(KRDict *krdict, const void *key)
{
    dictEntry *entry = dictFind(krdict->dict, key);
    if (entry != NULL) {
        switch(krdict->type)
        {
            case KR_INTEGER:
                fprintf(stdout, "Found Entry Integer[%d]\n", *(int *)entry->key);
                break;
            case KR_DOUBLE:
                fprintf(stdout, "Found Entry Double[%f]\n", *(double *)entry->key);
                break;
            case KR_STRING:
                fprintf(stdout, "Found Entry String[%s]\n", (char *)entry->key);
                break;
        }
        return KR_TRUE;
    }
    return KR_FALSE;
}

/* add an element
 * return KR_SUCCESS while success, KR_FAILURE for else
 */
int kr_dict_add(KRDict *krdict, void *key, void *val)
{
    if (dictAdd(krdict->dict, key, val) != DICT_OK) {
        return KR_FAILURE;
    }
    return KR_SUCCESS;
}

/* delete an element
 * return KR_SUCCESS while success, KR_FAILURE for else
 */
int kr_dict_delete(KRDict *krdict, const void *key)
{
    if (dictDelete(krdict->dict, key) != DICT_OK) {
        return KR_FAILURE;
    }
    return KR_SUCCESS;
}

/* release this krdict */
void kr_dict_release(KRDict *krdict)
{
    if (krdict != NULL) {
        dictRelease(krdict->dict);
        zfree(krdict->name);
        zfree(krdict); krdict=NULL;
    }
}