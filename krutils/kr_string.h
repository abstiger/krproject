#ifndef __KR_STRING_H__
#define __KR_STRING_H__

#include "kr_macros.h"

/* String operating functions */
extern boolean kr_string_isnumeric(const char *psBuf);
extern boolean kr_string_isdecimal(const char *psBuf);
extern boolean kr_string_isalphabet(const char *psBuf);
extern char *kr_string_rtrim( char *pcaBuf );
extern char *kr_string_ltrim( char *pcaBuf );
extern char *kr_string_alltrim( char *pcaBuf );
extern char *kr_string_toupper(char *pcaBuf);
extern char *kr_string_tolower(char *pcaBuf);

#endif /*__KR_STRING_H__*/
