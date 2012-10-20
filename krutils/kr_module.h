#ifndef __KR_MODULE_H__
#define __KR_MODULE_H__

#include <dlfcn.h>

typedef struct _kr_module_t
{
    char *filename;
    int  flag;
    void *handle;
}T_KRModule;




T_KRModule *kr_module_open(const char *filename, int flag);
void *kr_module_symbol(T_KRModule *krmodule, const char *symbolname);
int kr_module_close(T_KRModule *krmodule);

#endif /* __KR_MODULE_H__ */

