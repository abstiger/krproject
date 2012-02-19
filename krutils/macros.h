#ifndef __KR_MACROS_H__
#define __KR_MACROS_H__

/* We include stddef.h to get the system's definition of NULL
 */
#include <stddef.h>

#define KR_MAJOR_VERSION 1
#define KR_MINOR_VERSION 0
#define KR_MICRO_VERSION 0
#define KR_VERSION KR_MAJOR_VERSION.KR_MINOR_VERSION.KR_MICRO_VERSION

/* Guard C code in headers, while including them from C++ */
#ifdef  __cplusplus
# define KR_BEGIN_DECLS  extern "C" {
# define KR_END_DECLS    }
#else
# define KR_BEGIN_DECLS
# define KR_END_DECLS
#endif


#ifndef NULL
#  ifdef __cplusplus
#    define NULL        (0L)
#  else /* !__cplusplus */
#    define NULL        ((void*) 0)
#  endif /* !__cplusplus */
#endif

typedef int boolean;

#ifndef	FALSE
#define	FALSE	(0)
#endif

#ifndef	TRUE
#define	TRUE	(!FALSE)
#endif

#ifndef	MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef	MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif

#ifndef	ABS
#define ABS(a)	   (((a) < 0) ? -(a) : (a))
#endif

#endif /* __KR_MACROS_H__ */
