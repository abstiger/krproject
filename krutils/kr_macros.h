#ifndef __KR_MACROS_H__
#define __KR_MACROS_H__

/* We include stddef.h to get the system's definition of NULL
 */
#include <stddef.h>

/* Guard C code in headers, while including them from C++ */
#ifdef  __cplusplus
# define KR_BEGIN_DECLS  extern "C" {
# define KR_END_DECLS    }
#else
# define KR_BEGIN_DECLS
# define KR_END_DECLS
#endif

/* Provide definitions for some commonly used macros.
 *  Some of them are only provided if they haven't already
 *  been defined. It is assumed that if they are already
 *  defined then the current definition is correct.
 */
#ifndef NULL
#  ifdef __cplusplus
#    define NULL        (0L)
#  else /* !__cplusplus */
#    define NULL        ((void*) 0)
#  endif /* !__cplusplus */
#endif

typedef int   boolean;

#ifndef	FALSE
#define	FALSE	(0)
#endif

#ifndef	TRUE
#define	TRUE	(!FALSE)
#endif

#undef	MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

#undef	MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

#undef	ABS
#define ABS(a)	   (((a) < 0) ? -(a) : (a))


#undef	SUCCESS
#define	SUCCESS	0

#undef	FAILURE
#define	FAILURE	-1

/*macros for character handling*/
#define ISSPACE(c)              ((c) == ' ' || (c) == '\f' || (c) == '\n' || \
                                 (c) == '\r' || (c) == '\t' || (c) == '\v')
#define ISUPPER(c)              ((c) >= 'A' && (c) <= 'Z')
#define ISLOWER(c)              ((c) >= 'a' && (c) <= 'z')
#define ISALPHA(c)              (ISUPPER (c) || ISLOWER (c))
#define TOUPPER(c)              (ISLOWER (c) ? (c) - 'a' + 'A' : (c))
#define TOLOWER(c)              (ISUPPER (c) ? (c) - 'A' + 'a' : (c))

/* 相关长度宏定义 */
#define KR_FILENAME_LEN      1024
#define KR_MAXINPUT_LEN      4096

#endif /* __KR_MACROS_H__ */
