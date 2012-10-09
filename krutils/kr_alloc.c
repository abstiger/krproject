#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "kr_macros.h"
#include "kr_alloc.h"

#ifdef HAVE_MALLOC_SIZE
#define PREFIX_SIZE (0)
#else
#if defined(__sun)
#define PREFIX_SIZE (sizeof(long long))
#else
#define PREFIX_SIZE (sizeof(size_t))
#endif
#endif

/* Explicitly override malloc/free etc when using tcmalloc. */
#if defined(USE_TCMALLOC)
#define malloc(size) tc_malloc(size)
#define calloc(count,size) tc_calloc(count,size)
#define realloc(ptr,size) tc_realloc(ptr,size)
#define free(ptr) tc_free(ptr)
#elif defined(USE_JEMALLOC)
#define malloc(size) je_malloc(size)
#define calloc(count,size) je_calloc(count,size)
#define realloc(ptr,size) je_realloc(ptr,size)
#define free(ptr) je_free(ptr)
#endif

#define update_kr_malloc_stat_alloc(__n,__size) do { \
    size_t _n = (__n); \
    if (_n&(sizeof(long)-1)) _n += sizeof(long)-(_n&(sizeof(long)-1)); \
    if (kr_malloc_thread_safe) { \
        pthread_mutex_lock(&used_memory_mutex);  \
        used_memory += _n; \
        pthread_mutex_unlock(&used_memory_mutex); \
    } else { \
        used_memory += _n; \
    } \
} while(0)

#define update_kr_malloc_stat_free(__n) do { \
    size_t _n = (__n); \
    if (_n&(sizeof(long)-1)) _n += sizeof(long)-(_n&(sizeof(long)-1)); \
    if (kr_malloc_thread_safe) { \
        pthread_mutex_lock(&used_memory_mutex);  \
        used_memory -= _n; \
        pthread_mutex_unlock(&used_memory_mutex); \
    } else { \
        used_memory -= _n; \
    } \
} while(0)

static size_t used_memory = 0;
static int kr_malloc_thread_safe = 0;
pthread_mutex_t used_memory_mutex = PTHREAD_MUTEX_INITIALIZER;

static void kr_malloc_oom(size_t size) {
    fprintf(stderr, "kr_malloc: Out of memory trying to allocate %zu bytes\n",
        size);
    fflush(stderr);
    abort();
}

void *kr_malloc(size_t size) {
    void *ptr = malloc(size+PREFIX_SIZE);

    if (!ptr) kr_malloc_oom(size);
#ifdef HAVE_MALLOC_SIZE
    update_kr_malloc_stat_alloc(kr_malloc_size(ptr),size);
    return ptr;
#else
    *((size_t*)ptr) = size;
    update_kr_malloc_stat_alloc(size+PREFIX_SIZE,size);
    return (char*)ptr+PREFIX_SIZE;
#endif
}

void *kr_calloc(size_t size) {
    void *ptr = calloc(1, size+PREFIX_SIZE);

    if (!ptr) kr_malloc_oom(size);
#ifdef HAVE_MALLOC_SIZE
    update_kr_malloc_stat_alloc(kr_malloc_size(ptr),size);
    return ptr;
#else
    *((size_t*)ptr) = size;
    update_kr_malloc_stat_alloc(size+PREFIX_SIZE,size);
    return (char*)ptr+PREFIX_SIZE;
#endif
}

void *kr_realloc(void *ptr, size_t size) {
#ifndef HAVE_MALLOC_SIZE
    void *realptr;
#endif
    size_t oldsize;
    void *newptr;

    if (ptr == NULL) return kr_malloc(size);
#ifdef HAVE_MALLOC_SIZE
    oldsize = kr_malloc_size(ptr);
    newptr = realloc(ptr,size);
    if (!newptr) kr_malloc_oom(size);

    update_kr_malloc_stat_free(oldsize);
    update_kr_malloc_stat_alloc(kr_malloc_size(newptr),size);
    return newptr;
#else
    realptr = (char*)ptr-PREFIX_SIZE;
    oldsize = *((size_t*)realptr);
    newptr = realloc(realptr,size+PREFIX_SIZE);
    if (!newptr) kr_malloc_oom(size);

    *((size_t*)newptr) = size;
    update_kr_malloc_stat_free(oldsize);
    update_kr_malloc_stat_alloc(size,size);
    return (char*)newptr+PREFIX_SIZE;
#endif
}

void kr_free(void *ptr) {
#ifndef HAVE_MALLOC_SIZE
    void *realptr;
    size_t oldsize;
#endif

    if (ptr == NULL) return;
#ifdef HAVE_MALLOC_SIZE
    update_kr_malloc_stat_free(kr_malloc_size(ptr));
    free(ptr);
#else
    realptr = (char*)ptr-PREFIX_SIZE;
    oldsize = *((size_t*)realptr);
    update_kr_malloc_stat_free(oldsize+PREFIX_SIZE);
    free(realptr);
#endif
}

char *kr_strdup(const char *s) {
    size_t l = strlen(s)+1;
    char *p = kr_malloc(l);

    memcpy(p,s,l);
    return p;
}

size_t kr_malloc_used_memory(void) {
    size_t um;

    if (kr_malloc_thread_safe) pthread_mutex_lock(&used_memory_mutex);
    um = used_memory;
    if (kr_malloc_thread_safe) pthread_mutex_unlock(&used_memory_mutex);
    return um;
}

void kr_malloc_enable_thread_safeness(void) {
    kr_malloc_thread_safe = 1;
}

/* Get the RSS information in an OS-specific way.
 *
 * WARNING: the function kr_malloc_get_rss() is not designed to be fast
 * and may not be called in the busy loops where Redis tries to release
 * memory expiring or swapping out objects.
 *
 * For this kind of "fast RSS reporting" usages use instead the
 * function RedisEstimateRSS() that is a much faster (and less precise)
 * version of the funciton. */

#if defined(HAVE_PROCFS)
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

size_t kr_malloc_get_rss(void) {
    int page = sysconf(_SC_PAGESIZE);
    size_t rss;
    char buf[4096];
    char filename[256];
    int fd, count;
    char *p, *x;

    snprintf(filename,256,"/proc/%d/stat",getpid());
    if ((fd = open(filename,O_RDONLY)) == -1) return 0;
    if (read(fd,buf,4096) <= 0) {
        close(fd);
        return 0;
    }
    close(fd);

    p = buf;
    count = 23; /* RSS is the 24th field in /proc/<pid>/stat */
    while(p && count--) {
        p = strchr(p,' ');
        if (p) p++;
    }
    if (!p) return 0;
    x = strchr(p,' ');
    if (!x) return 0;
    *x = '\0';

    rss = strtoll(p,NULL,10);
    rss *= page;
    return rss;
}
#elif defined(HAVE_TASKINFO)
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/task.h>
#include <mach/mach_init.h>

size_t kr_malloc_get_rss(void) {
    task_t task = MACH_PORT_NULL;
    struct task_basic_info t_info;
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

    if (task_for_pid(current_task(), getpid(), &task) != KERN_SUCCESS)
        return 0;
    task_info(task, TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count);

    return t_info.resident_size;
}
#else
size_t kr_malloc_get_rss(void) {
    /* If we can't get the RSS in an OS-specific way for this system just
     * return the memory usage we estimated in kr_malloc()..
     *
     * Fragmentation will appear to be always 1 (no fragmentation)
     * of course... */
    return kr_malloc_used_memory();
}
#endif

/* Fragmentation = RSS / allocated-bytes */
float kr_malloc_get_fragmentation_ratio(void) {
    return (float)kr_malloc_get_rss()/kr_malloc_used_memory();
}
