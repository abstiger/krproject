#include <stdio.h>
#include <stdlib.h>
#include "zmalloc.h"

int main(int argc,char *argv[])
{
    char *pzmalloc=(char *)zmalloc(10);
    strcpy(pzmalloc, "haha");
    printf("%s\n", pzmalloc);
    zfree(pzmalloc);
    return 0;
}