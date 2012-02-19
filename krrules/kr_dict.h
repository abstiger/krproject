#ifndef __KR_DICT_H__
#define __KR_DICT_H__

#include "utils.h"
#include "dict.h"

typedef struct _kr_dict
{
    char     *name;
    KRType   type;
    dict     *dict;
}KRDict;


KRDict *kr_dict_create(char *name, KRType keytype);
int kr_dict_add(KRDict *krdict, void *key, void *val);
int kr_dict_search(KRDict *krdict, const void *key);
int kr_dict_delete(KRDict *krdict, const void *key);
void kr_dict_release(KRDict *d);

#endif /* __KR_DICT_H__ */
