#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "kr_cfgparser.h"

#define KR_CONFIG_MEMO      '#'
#define KR_CONFIG_STRING_BORDER  '"'
#define _IS_STRING_BORDER(x)     (x == KR_CONFIG_STRING_BORDER)
#define _IS_COMMENT(x)           (x == KR_CONFIG_MEMO)
#define _IS_SECTION_BEGIN(x)     (x == '[')
#define _IS_YES(x)               (x == 'Y' || x == 'y')

char gcaConfigFileName[KR_FILENAME_LEN + 1] = {0};

static int _tIsThisEntry(const char *vspLine, const char *vspEntry);
static void _vClearBlank(char *vspLine);
static void _vCutContent(char *vspLine, char *vspDest);


/*
 *  Function:  vClearBlank
 *
 *      To remove all comment and blank char from one line
 *
 *  Parameters:
 *
 *      vspLine IO- point of a string of a line
 *
 *  Return Value:
 *      none
 */
static void _vClearBlank(char *vspLine)
{
    int     i = 0, j, k;
    char    buf[KR_MAXINPUT_LEN];

    while (vspLine[i] != 0) {
        if (vspLine[i] == KR_CONFIG_MEMO || vspLine[i] == '\n') {
            vspLine[i] = 0;
            if (i == 0) break;
            j = i - 1;
            do {
                if (j < 0) break;
                if ((vspLine[j] != ' ') && (vspLine[j] != '\t')) break;
                vspLine[j--] = 0;
            } while(1);
            break;
        }
        i++;
    }

    i = 0;
    memset(buf, 0, KR_MAXINPUT_LEN);

    while ((vspLine[i] != '=') && (i < strlen(vspLine)))
        i++;
    if (i == strlen(vspLine)) return;

    for (j = i - 1; (vspLine[j] == ' ') || (vspLine[j] == '\t'); j--);
    for (k = i + 1; (vspLine[k] == ' ') || (vspLine[k] == '\t'); k++);

    memcpy(buf, vspLine, j + 1);
    buf[j + 1] = '=';
    strcat(buf + j + 2, vspLine + k);

    strcpy(vspLine, buf);
    return;
}

/*
 *  Function:  tIsSection
 *
 *      To confirm if a line is a section line
 *
 *  Parameters:
 *
 *      vspLine I - point of a string of a line
 *
 *  Return Value:
 *
 *       0 - is not a section line
 *       1 - is a section line
 */

int _tIsSection(char * vspLine)
{
    return (vspLine[0] == '[');
}


/*
 *  Function:  tIsThisSection
 *
 *      To confirm if this line is a spec section
 *
 *  Parameters:
 *
 *      vspLine    I - point of a string of a line
 *      vspSection I - name of section
 *
 *  Return Value:
 *
 *       0 - is not this section
 *       1 - is this section
 */

int _tIsThisSection(const char *vspLine, const char *vspSection)
{
    return (!memcmp(vspLine + 1, vspSection, strlen(vspSection)) &&
        vspLine[strlen(vspSection)+1] == ']');
}


/*
 *  Function:  tIsThisEntry
 *
 *      To confire if this line is a spec entry
 *
 *  Parameters:
 *
 *      vspLine  I - point of a string of a line
 *      vspEntry I - name of entry
 *
 *  Return Value:
 *
 *       0 - is not this entry
 *       1 - is this entry
 */

static int _tIsThisEntry(const char *vspLine, const char *vspEntry)
{
    return (!memcmp(vspLine, vspEntry, strlen(vspEntry)) &&
            vspLine[strlen(vspEntry)] == '=') ;
}


/*
 *  Function:  vCutContent
 *
 *      To get the value from a line
 *
 *  Parameters:
 *
 *      vspLine  I - point of a string of a line
 *      vspDest  O - value of entry to put
 *
 *  Return Value:
 *      none
 */

static void _vCutContent(char *vspLine, char *vspDest)
{
    int     i = 0;

    while (vspLine[i++] != '=');

    strcpy(vspDest, vspLine + i);
    return;
}

/*
 *  Function:  kr_config_setfile
 *
 *      set global variable for where is the config file 
 *
 *  Parameters:
 *
 *      vspConfigFile I- point of the config file name
 *
 *  Return Value:
 *      0 - success
 *      -1 - failure
 */

int kr_config_setfile(const char *vspConfigFile)
{
    FILE *fp;
    
    if ((fp=fopen(vspConfigFile, "r")) == NULL) {
        return -1;
    }
    fclose(fp);
    
    strcpy(gcaConfigFileName, vspConfigFile);
    return 0;
}

/*
 *  Function:  kr_config_getstring
 *
 *      To get a string value from profile
 *
 *  Parameters:
 *
 *      vspSection  I - section name
 *      vspEntry    I - entry name
 *      vspValue    O - pointer of string which value is returned
 *
 *  Return Value:
 *
 *      0 - success
 *      -1 - failure
 */

int kr_config_getstring(const char *vspSection, const char *vspEntry, char *vspValue)
{
    int     cbNum = FALSE;
    int     tInSection = FALSE;
    FILE    *fp;
    char    saLine[KR_MAXINPUT_LEN+1];

    if ((fp = fopen(gcaConfigFileName, "r")) == NULL) {
        return -1;
    }

    while (NULL != fgets(saLine, KR_MAXINPUT_LEN, fp)) {
        _vClearBlank(saLine);

        if (_tIsSection(saLine)) {
            tInSection = _tIsThisSection(saLine, vspSection);
            continue;
        }

        if (tInSection == FALSE) continue;

        if (_tIsThisEntry(saLine, vspEntry)) {
            _vCutContent(saLine, vspValue);
            cbNum = TRUE;
            break;
        }
    }

    fclose(fp);

    if (cbNum == FALSE) {
        return -1;
    }

    return 0;
}


/*
 *  Function:  kr_config_getint
 *
 *      To get a int value from profile
 *
 *  Parameters:
 *
 *      vspSection  I - section name
 *      vspEntry    I - entry name
 *      vtpValue    O - pointer of int which value is returned
 *
 *  Return Value:
 *
 *      0 - success
 *      -1 - failure
 */

int kr_config_getint(const char *vspSection, const char *vspEntry, int *vtpValue)
{
    char buf[KR_MAXINPUT_LEN];

    if (kr_config_getstring(vspSection, vspEntry, buf) != 0) {
        *vtpValue = 0;
        return -1;
    }
    else {
        *vtpValue = atoi(buf);
    }
    return 0;
}

/*
 *  Function:  kr_config_getshort
 *
 *      To get a int value from profile
 *
 *  Parameters:
 *
 *      vspSection  I - section name
 *      vspEntry    I - entry name
 *      vtpValue    O - pointer of int which value is returned
 *
 *  Return Value:
 *
 *      0 - success
 *      -1 - failure
 */

int kr_config_getshort(const char *vspSection, const char *vspEntry, short *vnpValue)
{
    char buf[KR_MAXINPUT_LEN];

    if (kr_config_getstring(vspSection, vspEntry, buf) != 0) {
        *vnpValue = 0;
        return -1;
    }
    else {
        *vnpValue = atoi(buf);
    }
    return 0;
}

/*
 *  Function: kr_config_getlong
 *
 *      To get a long value from profile
 *
 *  Parameters:
 *
 *      vspSection  I - section name
 *      vspEntry    I - entry name
 *      vlpValue    O - pointer of long which value is returned
 *
 *  Return Value:
 *
 *      0 - success
 *      -1 - failure
 */
int kr_config_getlong(const char *vspSection, const char *vspEntry, long *vlpValue)
{
    char buf[KR_MAXINPUT_LEN];

    if (kr_config_getstring(vspSection, vspEntry, buf) != 0) {
        *vlpValue = 0L;
        return -1;
    }
    else {
        *vlpValue = atol(buf);
    }
    return 0;
}

/*
 *  Function: kr_config_getdouble
 *
 *      To get a double value from profile
 *
 *  Parameters:
 *
 *      vspSection  I - section name
 *      vspEntry    I - entry name
 *      vdpValue    O - pointer of double which value is returned
 *
 *  Return Value:
 *
 *      0 - success
 *      -1 - failure
 */
int kr_config_getdouble(const char *vspSection, const char *vspEntry, double *vdpValue)
{
    char buf[KR_MAXINPUT_LEN];

    if (kr_config_getstring(vspSection, vspEntry, buf) != 0) {
        *vdpValue = 0L;
        return -1;
    }
    else {
        *vdpValue = atof(buf);
    }
    return 0;
}

int tLTrim(char *vspString)
{
    register int    i, j, tLen;

    tLen = strlen(vspString);
    j = 0;
    if (vspString[j] != ' ' && vspString[j] != '\t')
        return tLen;

    while (j < tLen && (vspString[j] == ' ' || vspString[j] == '\t') )
        j++;
    for(i = 0; i < tLen - j; i++)
        vspString[i] = vspString[i+j];
    vspString[i] = 0;
    return (i);
}

int tRTrim(char *vspString)
{
    register int    i;

    i = strlen(vspString)-1;

    while(i >= 0 && (vspString[i] == ' '||vspString[i] == '\t'||vspString[i] == '\n')) {
        vspString[i] = 0;
        i--;
    }
    return (i+1);
}

int tAllTrim(char *vspString)
{
    tRTrim(vspString);
    return(tLTrim(vspString));
}

int _tTrimComment(char *vspString)
{
    register int i, tLen;

    tLen = strlen(vspString);
    i = 0;
    while (i < tLen) {
        if (_IS_COMMENT(vspString[i])) {
            vspString[i] = 0;
        }
        i++;
    }
    return (i);
}

int _tTrimExtraSpace(char *vspString)
{
    register int i, j, tLen;
    int      tInString, tInSpace;

    tLen = strlen(vspString);
    i = 0, j = 0;
    tInString = FALSE;
    tInSpace = TRUE;

    while (j < tLen) {
        if (_IS_STRING_BORDER(vspString[j])) {
            tInString = !tInString;
            vspString[i] = vspString[j];
            tInSpace = FALSE;
            i++;
        }
        else {
            if (!tInString) {
                if (vspString[j] == '=') {
                    if (tInSpace&&i>0)  //elimite the front space of  equalsign
                        i--;
                    vspString[i] = vspString[j];
                    tInSpace = TRUE;    //elimite the back space of  equalsign
                    i++;
                }
                else if (vspString[j] == ' ' || vspString[j] == '\t') {
                    if (!tInSpace) {
                        tInSpace = TRUE;
                        vspString[i] = vspString[j];
                        i++;
                    }
                }
                else {
                    vspString[i] = vspString[j];
                    tInSpace = FALSE;
                    i++;
                }
            }
            else {
                vspString[i] = vspString[j];
                i++;
            }
        }
        j++;
    }
    vspString[i] = 0;
    return (i);
}
