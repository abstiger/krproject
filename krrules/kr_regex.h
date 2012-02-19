#ifndef __KR_REGEX_H__
#define __KR_REGEX_H__

#include "utils.h"
#include <regex.h>

typedef struct _kr_regex
{
    char     *pattern;
    regex_t  regex;
}KRRegex;

KRRegex *kr_regex_compile(const char *pattern);
int kr_regex_execute(const KRRegex *krregex, const char *str);
void kr_regex_free(KRRegex *krregex);

#endif /* __KR_REGEX_H__ */