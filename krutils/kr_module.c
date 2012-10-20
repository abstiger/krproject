#include "kr_utils.h"
#include "kr_module.h"

T_KRModule *kr_module_open(const char *filename, int flag)
{
    T_KRModule *krmodule = (T_KRModule *)kr_calloc(sizeof(T_KRModule));
    if (krmodule == NULL) {
        fprintf(stderr, "kr_calloc krmodule failed!\n");
        return NULL;
    }
    krmodule->filename = kr_strdup(filename);
    krmodule->flag = flag;
    krmodule->handle = dlopen(filename, flag);
    if (krmodule->handle == NULL) {
        fprintf(stderr, "dlopen %s with %d failed [%s]!\n", \
                filename, flag, dlerror());
        kr_free(krmodule->filename);
        kr_free(krmodule);
        return NULL;
    }

    return krmodule;
}


void *kr_module_symbol(T_KRModule *krmodule, const char *symbolname)
{
    void *pfunc = dlsym(krmodule->handle, symbolname);
    if (pfunc == NULL) {
        fprintf(stderr, "dlsym %s failed! [%s]\n", \
                symbolname, dlerror());
        return NULL;
    }

    return pfunc;
}


int kr_module_close(T_KRModule *krmodule)
{
    int ret = 0;
    if (krmodule) {
        ret = dlclose(krmodule->handle);
        if (ret != 0) {
            fprintf(stderr, "dlclose failed [%s]\n", dlerror());
        }
        kr_free(krmodule->filename);
        kr_free(krmodule);
    }

    return ret;
}
