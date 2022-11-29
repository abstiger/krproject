#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "krutils/kr_alloc.h"

int main()
{
    void *pMalloc=NULL;
    size_t malloc_size = 100;
    
    pMalloc = kr_malloc(malloc_size);
    assert(pMalloc != NULL);
    
    pMalloc = kr_realloc(pMalloc, malloc_size*2);
    assert(pMalloc != NULL);
    
    kr_free(pMalloc);
    
    char *pStrDup = kr_strdup("Tiger");
    assert(pStrDup != NULL);
    printf("pStrDup=[%s]\n", pStrDup);
    kr_free(pStrDup);
    
    int i=0;
    char *paMalloc[100];
    for(i=0; i<100; i++) {
        paMalloc[i] = (char *)kr_malloc(malloc_size*i);
        assert(paMalloc[i] != NULL);
    }
    
    printf("used memory:[%u], fragmentation_ratio:[%f], rss:[%u]\n", \
           kr_malloc_used_memory(), 
           kr_malloc_get_fragmentation_ratio(), 
           kr_malloc_get_rss());
    
    for(i=0; i<100; i++) {
        kr_free(paMalloc[i]);
    }
    
    return 0;
}

