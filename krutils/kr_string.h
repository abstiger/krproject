#ifndef __KR_STRING_H__
#define __KR_STRING_H__

#include <string.h>
#include "kr_types.h"

/*simple wrapper of standard str functions*/
static inline size_t kr_string_length(const kr_string s)
{
    return strlen(s);
}

static inline int kr_string_compare(const kr_string s1, const kr_string s2)
{
    return strcmp(s1, s2);
}

/* String operating functions */
extern unsigned int kr_string_hash(const kr_string s);
extern kr_bool kr_string_equal(kr_string s1, kr_string s2);
extern kr_string kr_string_dup(kr_string s);
extern kr_string kr_string_dupenv(kr_string s);

extern kr_bool kr_string_isnumeric(const kr_string s);
extern kr_bool kr_string_isdecimal(const kr_string s);
extern kr_bool kr_string_isalphabet(const kr_string s);
extern kr_string kr_string_rtrim(kr_string s);
extern kr_string kr_string_ltrim(kr_string s);
extern kr_string kr_string_alltrim(kr_string s);
extern kr_string kr_string_toupper(kr_string s);
extern kr_string kr_string_tolower(kr_string s);

#endif /*__KR_STRING_H__*/
