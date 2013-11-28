#include <string.h>
#include <ctype.h>
#include "kr_string.h"


/*TODO:repalce this with a better hash*/
unsigned int kr_string_hash(const kr_string s)
{
    const signed char *p;
    unsigned int h = 5381;

    for (p = s; *p != '\0'; p++)
        h = (h << 5) + h + *p;

    return h;
}


kr_bool kr_string_equal(kr_string s1, kr_string s2)
{
    return kr_string_compare(s1, s2)==0?TRUE:FALSE;
}


kr_string kr_string_dup(kr_string s)
{
    size_t l = kr_string_length(s)+1;
    kr_string p = kr_calloc(l);

    memcpy(p,s,l);
    return p;
}


kr_bool kr_string_isnumeric(const kr_string s)
{
    int i;
    for (i = 0; i < strlen(s); i++) {
        if ((s[i] < '0') || (s[i] > '9')) {
            return FALSE;
        }
    }
    return TRUE;
}


kr_bool kr_string_isdecimal(const kr_string s)
{
    int i;
    for (i = 0; i < strlen(s); i++) {
        if (((s[i] <  '0') || (s[i] >  '9')) &&
            (s[i] != '.') && (s[i] != '-') && (s[i] != '+')) {
            return FALSE;
        }
    }
    return TRUE;
}


kr_bool kr_string_isalphabet(const kr_string s)
{
    int i;
    for (i = 0; i < strlen(s); i++) {
        if ((s[i] < 'A') ||
            ((s[i] > 'Z') && (s[i] < 'a')) ||
            (s[i] > 'z')) {
            return FALSE;
        }
    }
    return TRUE;
}


kr_string kr_string_rtrim(kr_string s)
{
    register int l;
    for(l=(int)strlen(s); l>0 && isspace((unsigned char)s[l-1]); l--) 
        s[l-1]='\0';
    return s;
}


kr_string kr_string_ltrim(kr_string s)
{
    register char *p;
    for( p=s; isspace((unsigned char)*p); p++ );
        if( p!=s )  strcpy(s, p);
    return s;
}


kr_string kr_string_alltrim(kr_string s)
{
    kr_string_rtrim(s);
    kr_string_ltrim(s);
    return s;
}


kr_string kr_string_toupper(kr_string s)
{
    int i;
    for (i = 0; i < strlen(s); i++) {
        if (((unsigned char)s[i] >= 0x81) && 
            ((unsigned char)s[i] <= 0xfe)) {
            i++;
        } else {
            s[i] = toupper(s[i]);
        }
    }
    return s;
}


kr_string kr_string_tolower(kr_string s)
{
    int i;
    for (i = 0; i < strlen(s); i++) {
        if (((unsigned char)s[i] >= 0x81) && 
            ((unsigned char)s[i] <= 0xfe)) {
            i++;
        } else {
            s[i] = tolower(s[i]);
        }
    }
    return s;
}
