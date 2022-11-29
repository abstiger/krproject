#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "krutils/kr_string.h"

int main()
{
    kr_bool ret;
    char caNumeric[10]="12345678";
    char caDecimal[10]="-1.23456";
    char caAlphabet[10]="abcdexyz";
    ret = kr_string_isnumeric(caNumeric);
    assert(ret == TRUE);
    ret = kr_string_isdecimal(caDecimal);
    assert(ret == TRUE);
    ret = kr_string_isalphabet(caAlphabet);
    assert(ret == TRUE);

    ret = kr_string_isnumeric(caDecimal);
    assert(ret == FALSE);
    ret = kr_string_isdecimal(caAlphabet);
    assert(ret == FALSE);
    ret = kr_string_isalphabet(caNumeric);
    assert(ret == FALSE);

    char caRTrim[20]="rtrim       ";
    char caLTrim[20]="       ltrim";
    char caALLTrim[20]="    ltrim    ";

    printf("caRTrim=[%s], kr_string_rtrim(caRTrim)=[%s]\n", \
            caRTrim, kr_string_rtrim(caRTrim));
    printf("caLTrim=[%s], kr_string_ltrim(caLTrim)=[%s]\n", \
            caLTrim, kr_string_ltrim(caLTrim));
    printf("caALLTrim=[%s], kr_string_alltrim(caALLTrim)=[%s]\n", \
            caALLTrim, kr_string_alltrim(caALLTrim));


    char caToUpper[10]="Toupper";
    char caToLower[10]="ToLOWER";
    printf("caToUpper=[%s], ", caToUpper);
    printf("kr_string_toupper(caToUpper)=[%s]\n", kr_string_toupper(caToUpper));
    printf("caToLower=[%s], ", caToLower);
    printf("kr_string_tolower(caToLower)=[%s]\n", kr_string_tolower(caToLower));
    
    return 0;
}
