#ifndef __KR_ALLOC_H__
#define __KR_ALLOC_H__

/* Double expansion needed for stringification of macro values. */
#define __xstr(s) __str(s)
#define __str(s) #s

#if defined(USE_TCMALLOC)
#define KRMALLOC_LIB ("tcmalloc-" __xstr(TC_VERSION_MAJOR) "." __xstr(TC_VERSION_MINOR))
#include <google/tcmalloc.h>
#if TC_VERSION_MAJOR >= 1 && TC_VERSION_MINOR >= 6
#define HAVE_MALLOC_SIZE 1
#define kr_malloc_size(p) tc_malloc_size(p)
#else
#error "Newer version of tcmalloc required"
#endif

#elif defined(USE_JEMALLOC)
#define KRMALLOC_LIB ("jemalloc-" __xstr(JEMALLOC_VERSION_MAJOR) "." __xstr(JEMALLOC_VERSION_MINOR) "." __xstr(JEMALLOC_VERSION_BUGFIX))
#define JEMALLOC_MANGLE
#include <jemalloc/jemalloc.h>
#if JEMALLOC_VERSION_MAJOR >= 2 && JEMALLOC_VERSION_MINOR >= 1
#define HAVE_MALLOC_SIZE 1
#define kr_malloc_size(p) JEMALLOC_P(malloc_usable_size)(p)
#else
#error "Newer version of jemalloc required"
#endif

#elif defined(__APPLE__)
#include <malloc/malloc.h>
#define HAVE_MALLOC_SIZE 1
#define kr_malloc_size(p) malloc_size(p)
#endif

#ifndef KRMALLOC_LIB
#define KRMALLOC_LIB "libc"
#endif

/*add by Tiger 20120328*/
#define HAVE_PROCFS 1

void *kr_malloc(size_t size);
void *kr_calloc(size_t size);
void *kr_realloc(void *ptr, size_t size);
void kr_free(void *ptr);
char *kr_strdup(const char *s);
size_t kr_malloc_used_memory(void);
void kr_malloc_enable_thread_safeness(void);
float kr_malloc_get_fragmentation_ratio(void);
size_t kr_malloc_get_rss(void);

#endif /* __KR_ALLOC_H__ */
