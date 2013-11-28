#ifndef __KR_REGEX_H__
#define __KR_REGEX_H__

/* kr_regex is a simple wrapper of libc's regex library
 * there is no regerror() wrapper function provided.
 */

#include "kr_macros.h"
#include <regex.h>

typedef struct _kr_regex
{
    char     *pattern;
    regex_t  regex;
}T_KRRegex;

T_KRRegex *kr_regex_compile(const char *pattern);
kr_bool kr_regex_execute(const T_KRRegex *krregex, const char *str);
void kr_regex_free(T_KRRegex *krregex);

#endif /* __KR_REGEX_H__ */
