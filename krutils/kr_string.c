#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "kr_macros.h"
#include "kr_string.h"

/*****************************************************************************/
/* FUNC:   boolean kr_string_isnumeric(const char *psBuf)                    */
/* PARAMS: psBuf - 输入串 (I)                                                */
/* RETURN: 0 - 不是                                                          */
/*         1 - 是                                                            */
/* DESC:   判断是否为全数字串                                                */
/*****************************************************************************/
boolean kr_string_isnumeric(const char *psBuf)
{
    int i;
    for (i = 0; i < strlen(psBuf); i++) 
    {
        if ((psBuf[i] < '0') || (psBuf[i] > '9')) 
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*****************************************************************************/
/* FUNC:   boolean kr_string_isdecimal(const char *psBuf)                    */
/* PARAMS: psBuf - 输入串 (I)                                                */
/* RETURN: 0 - 不是                                                          */
/*         1 - 是                                                            */
/* DESC:   判断是否为全数串                                                  */
/*****************************************************************************/
boolean kr_string_isdecimal(const char *psBuf)
{
    int i;
    for (i = 0; i < strlen(psBuf); i++) 
    {
        if (((psBuf[i] <  '0') || (psBuf[i] >  '9')) &&
            (psBuf[i] != '.') && (psBuf[i] != '-') && (psBuf[i] != '+')) 
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*****************************************************************************/
/* FUNC:   boolean kr_string_isalphabet(const char *psBuf)                   */
/* PARAMS: psBuf - 输入串 (I)                                                */
/* RETURN: 0 - 不是                                                          */
/*         1 - 是                                                            */
/* DESC:   判断是否为全字母串                                                */
/*****************************************************************************/
boolean kr_string_isalphabet(const char *psBuf)
{
    int i;
    for (i = 0; i < strlen(psBuf); i++) 
    {
        if ((psBuf[i] < 'A') ||
            ((psBuf[i] > 'Z') && (psBuf[i] < 'a')) ||
            (psBuf[i] > 'z'))
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*****************************************************************************/
/* FUNC:   char *kr_string_rtrim(char *pcaBuf)                               */
/* PARAMS: pcaBuf  - 转换串 (I/O)                                            */
/* RETURN: 转换结果串                                                        */
/* DESC:   去除右边空格                                                      */
/*****************************************************************************/
char *kr_string_rtrim(char *pcaBuf)
{
    register int l;
    for(l=(int)strlen(pcaBuf); l>0 && isspace((unsigned char)pcaBuf[l-1]); l--) 
        pcaBuf[l-1]='\0';
    return pcaBuf;
}

/*****************************************************************************/
/* FUNC:   char *kr_string_ltrim(char *pcaBuf)                               */
/* PARAMS: pcaBuf  - 传入串 (I)                                              */
/* RETURN: 转换结果串                                                        */
/* DESC:   去除左边空格                                                      */
/*****************************************************************************/
char *kr_string_ltrim( char *pcaBuf )
{
    register char *p;
    for( p=pcaBuf; isspace((unsigned char)*p); p++ );
        if( p!=pcaBuf )  strcpy(pcaBuf, p);
    return pcaBuf;
}

/*****************************************************************************/
/* FUNC:   char *kr_string_alltrim(char *pcaBuf)                             */
/* PARAMS: pcaBuf  - 传入串 (I)                                              */
/* RETURN: 转换结果串                                                        */
/* DESC:   去除两边空格                                                      */
/*****************************************************************************/
char *kr_string_alltrim( char *pcaBuf )
{
    kr_string_rtrim(pcaBuf);
    kr_string_ltrim(pcaBuf);
    return pcaBuf;
}

/*****************************************************************************/
/* FUNC:   char *kr_string_toupper(char *pcaBuf)                             */
/* PARAMS: psBuf - 转换串 (I/O)                                              */
/* RETURN: 返回串                                                            */
/*         1 - 是                                                            */
/* DESC:   转换为全大写                                                      */
/*****************************************************************************/
char *kr_string_toupper(char *pcaBuf)
{
    int i;
    for (i = 0; i < strlen(pcaBuf); i++)
    {
        if (((unsigned char)pcaBuf[i] >= 0x81) && 
            ((unsigned char)pcaBuf[i] <= 0xfe))
        {
            /* 如果是汉字的前半个字符，跳过整个汉字不做转换 */
            i++;
        }
        else
        {
            pcaBuf[i] = toupper(pcaBuf[i]);
        }
    }
    return pcaBuf;
}

/*****************************************************************************/
/* FUNC:   char *kr_string_tolower(char *pcaBuf)                             */
/* PARAMS: psBuf - 转换串 (I/O)                                              */
/* RETURN: 返回串                                                            */
/*         1 - 是                                                            */
/* DESC:   转换为全小写                                                      */
/*****************************************************************************/
char *kr_string_tolower(char *pcaBuf)
{
    int i;
    for (i = 0; i < strlen(pcaBuf); i++)
    {
        if (((unsigned char)pcaBuf[i] >= 0x81) && 
            ((unsigned char)pcaBuf[i] <= 0xfe))
        {
            /* 如果是汉字的前半个字符，跳过整个汉字不做转换 */
            i++;
        }
        else
        {
            pcaBuf[i] = tolower(pcaBuf[i]);
        }
    }
    return pcaBuf;
}