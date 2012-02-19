/* kr_regex is a simple wrapper of libc's regex library
 * there is no regerror() wrapper function provided.
 */

#include <stdio.h>
#include "kr_regex.h"
#include "zmalloc.h"

/*compile a regex from pattern string*/
KRRegex *kr_regex_compile(const char *pattern)
{
    int nRet = 0;
    KRRegex *krregex = (KRRegex *)zcalloc(sizeof(KRRegex));
    if (krregex == NULL) {
        fprintf(stderr, "zmalloc KRRegex failed!\n");
        return NULL;
    }
    
    krregex->pattern = zstrdup(pattern);
    nRet = regcomp(&krregex->regex, krregex->pattern, REG_EXTENDED|REG_NOSUB);
    if (nRet != 0) {
        fprintf(stderr, "regcomp [%s] failed!\n", krregex->pattern);
        zfree(krregex->pattern);
        zfree(krregex); krregex = NULL;
    }
        
    return krregex;
}


/* do a regex match
 * return KR_TRUE while matched, KR_FALSE for else
 */
int kr_regex_execute(const KRRegex *krregex, const char *str)
{
    int nRet = 0;
    nRet = regexec(&krregex->regex, str, 0, NULL, 0);
    if (nRet != REG_NOERROR) {
        fprintf(stdout, "regexec [%s] failed:[%d]!\n", str, nRet);
        return KR_FALSE;
    }
    return KR_TRUE;
}


/* free this krregex */
void kr_regex_free(KRRegex *krregex)
{
    if (krregex != NULL) {
        zfree(krregex->pattern);
        regfree(&krregex->regex);
        zfree(krregex); krregex=NULL;
    }
}