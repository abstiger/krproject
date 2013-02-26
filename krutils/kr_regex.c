#include "kr_utils.h"
#include "kr_regex.h"

/*compile a regex from pattern string*/
T_KRRegex *kr_regex_compile(const char *pattern)
{
    int nRet = 0;
    T_KRRegex *krregex = (T_KRRegex *)kr_calloc(sizeof(T_KRRegex));
    if (krregex == NULL) {
        fprintf(stderr, "kr_malloc T_KRRegex failed!\n");
        return NULL;
    }
    
    krregex->pattern = kr_strdup(pattern);
    nRet = regcomp(&krregex->regex, krregex->pattern, REG_EXTENDED|REG_NOSUB);
    if (nRet != 0) {
        fprintf(stderr, "regcomp [%s] failed!\n", krregex->pattern);
        kr_free(krregex->pattern);
        kr_free(krregex); krregex = NULL;
    }
    return krregex;
}


/* do a regex match
 * return KR_TRUE while matched, KR_FALSE for else
 */
boolean kr_regex_execute(const T_KRRegex *krregex, const char *str)
{
    int nRet = 0;
    nRet = regexec(&krregex->regex, str, 0, NULL, 0);
    if (nRet == REG_NOERROR) {
        return TRUE;
    } else if (nRet != REG_NOMATCH) {
        fprintf(stderr, "regexec [%s] with error:[%d]!\n", str, nRet);
    }
    return FALSE;
}


/* free this krregex */
void kr_regex_free(T_KRRegex *krregex)
{
    if (krregex != NULL) {
        kr_free(krregex->pattern);
        regfree(&krregex->regex);
        kr_free(krregex); krregex=NULL;
    }
}