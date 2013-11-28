#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kr_datetime.h"

char *kr_time_system(char *pcaSysTime)
{
    struct tm *ptTime;
    time_t tTimeVal;

    if ((tTimeVal = time(NULL)) == -1) {
        return NULL;
    }
    if ((ptTime = localtime(&tTimeVal)) == NULL) {
        return NULL;
    }

    sprintf(pcaSysTime, "%04d%02d%02d%02d%02d%02d",
               ptTime->tm_year + 1900,
               ptTime->tm_mon + 1,
               ptTime->tm_mday,
               ptTime->tm_hour,
               ptTime->tm_min,
               ptTime->tm_sec);

    return pcaSysTime;
}


kr_bool kr_date_isvalid(const char *pcaDate)
{
    char caYear[5], caMonth[3], caDay[3];
    struct tm   tTime;

    memset(caYear, 0, sizeof(caYear));
    memset(caMonth, 0, sizeof(caMonth));
    memset(caDay, 0, sizeof(caDay));

    memcpy(caYear, pcaDate, 4);
    memcpy(caMonth, pcaDate + 4, 2);
    memcpy(caDay, pcaDate + 6, 2);

    tTime.tm_year  = atoi(caYear) - 1900;
    tTime.tm_mon   = atoi(caMonth) - 1;
    tTime.tm_mday  = atoi(caDay);
    tTime.tm_hour  = 0;
    tTime.tm_min   = 0;
    tTime.tm_sec   = 1;
    tTime.tm_isdst = -1;

    if (mktime(&tTime) == -1) {
        return FALSE;
    }

    return TRUE;
}


kr_bool kr_date_isleap(const char *pcaDate)
{
    char caYear[4+1] = {0};
    int iYear = 0;
    
    memcpy(caYear, pcaDate, 4);
    iYear = atoi(caYear);
    if ((iYear % 4 == 0 && iYear % 100 != 0 ) || iYear % 400 == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}


char *kr_date_getmonthend(char *pcaDate)
{
    char caYear[5];
    char caMonth[3];

    memset(caYear, 0, sizeof(caYear));
    memset(caMonth, 0, sizeof(caMonth));

    memcpy(caYear, pcaDate, 4);
    memcpy(caMonth, pcaDate + 4, 2);

    switch (atoi(caMonth))
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            memcpy(pcaDate + 6, "31", 2);
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            memcpy(pcaDate + 6, "30", 2);
            break;
        case 2:
            if (kr_date_isleap(caYear)) {
                memcpy(pcaDate + 6, "29", 2);
            } else {
                memcpy(pcaDate + 6, "28", 2);
            }
            break;
        default:
            return NULL;
    }
    return pcaDate;
}


char *kr_date_addmonth(const char *pcaBegin, int iMonths, char *pcaEnd)
{
    char caYear[5], caMonth[3], caDay[3];
    struct tm tTime;
    time_t tTimeVal;
    long lValue;

    memset(caYear, 0, sizeof(caYear));
    memset(caMonth, 0, sizeof(caMonth));
    memset(caDay, 0, sizeof(caDay));

    memcpy(caYear, pcaBegin, 4);
    memcpy(caMonth, pcaBegin + 4, 2);
    if (pcaBegin[6] == 0) {
        strcpy(caDay, "01");
    } else {
        memcpy(caDay, pcaBegin + 6, 2);
    }

    lValue = atol(caYear) * 12 + atol(caMonth) + iMonths - 1;

    sprintf(caYear, "%04d", (int)((lValue) / 12));
    sprintf(caMonth, "%02d", (int)((lValue) % 12 + 1));

    while (1)
    {
        struct tm *ptTm;
        char caBufTmp[8 + 1];

        tTime.tm_year  = atoi(caYear) - 1900;
        tTime.tm_mon   = atoi(caMonth) - 1;
        tTime.tm_mday  = atoi(caDay);
        tTime.tm_hour  = 0;
        tTime.tm_min   = 0;
        tTime.tm_sec   = 1;
        tTime.tm_isdst = -1;

        sprintf(pcaEnd, "%s%s%s", caYear, caMonth, caDay);

        if ((tTimeVal = mktime(&tTime)) == -1) {
            return NULL;
        }
        if ((ptTm = localtime(&tTimeVal)) == NULL) {
            return NULL;
        }
        sprintf(caBufTmp, "%04d%02d%02d", \
                ptTm->tm_year + 1900, ptTm->tm_mon + 1, ptTm->tm_mday);
        if (memcmp(pcaEnd, caBufTmp, sizeof(caBufTmp) - 1) == 0) {
            break;
        }
        sprintf(caDay, "%02d", (int)(atol(caDay) - 1));
    }

    if (pcaBegin[6] == 0) {
        pcaEnd[6] = 0;
    }

    return pcaEnd;
}


char *kr_date_addday(const char *pcaBegin, int iDays, char *pcaEnd)
{
    struct tm tTime, *ptTime;
    time_t    tTimeNow;
    char caYear[5], caMonth[3], caDay[3];

    memset(caYear, 0, sizeof(caYear));
    memset(caMonth, 0, sizeof(caMonth));
    memset(caDay, 0, sizeof(caDay));

    memcpy(caYear, pcaBegin, 4);
    memcpy(caMonth, pcaBegin + 4, 2);
    memcpy(caDay, pcaBegin + 6, 2);

    tTime.tm_year  = atoi(caYear) - 1900;
    tTime.tm_mon   = atoi(caMonth) - 1;
    tTime.tm_mday  = atoi(caDay);
    tTime.tm_hour  = 0;
    tTime.tm_min   = 0;
    tTime.tm_sec   = 1;
    tTime.tm_isdst = -1;

    if ((tTimeNow = mktime(&tTime)) == -1) {
        return NULL;
    }
    tTimeNow += iDays * (24 * 60 * 60);

    ptTime = localtime(&tTimeNow);

    sprintf(pcaEnd, "%04d%02d%02d",
            ptTime->tm_year + 1900,
            ptTime->tm_mon + 1,
            ptTime->tm_mday);

    return pcaEnd;
}


char *kr_time_addsec(const char *pcaBegin, long iSecs, char *pcaEnd)
{
    struct tm tTime, *pTime;
    time_t    lTime;
    char      year[5], month[3], day[3], hour[3], minute[3], second[3];

    memset(year, 0, sizeof(year));
    memset(month, 0, sizeof(month));
    memset(day, 0, sizeof(day));
    memset(hour, 0, sizeof(hour));
    memset(minute, 0, sizeof(minute));
    memset(second, 0, sizeof(second));

    /* make up from time */
    memcpy(year, pcaBegin, 4);
    memcpy(month, pcaBegin + 4, 2);
    memcpy(day, pcaBegin + 6, 2);
    memcpy(hour, pcaBegin + 8, 2);
    memcpy(minute, pcaBegin + 10, 2);
    memcpy(second, pcaBegin + 12, 2);

    tTime.tm_year  = atoi(year) - 1900;
    tTime.tm_mon   = atoi(month) - 1;
    tTime.tm_mday  = atoi(day);
    tTime.tm_hour  = atoi(hour);
    tTime.tm_min   = atoi(minute);
    tTime.tm_sec   = atoi(second);
    tTime.tm_isdst = -1;

    if ((lTime = mktime(&tTime)) == -1) {
        return NULL;
    }
    lTime += iSecs;

    pTime = localtime(&lTime);

    sprintf(pcaEnd, "%04d%02d%02d%02d%02d%02d",
        pTime->tm_year + 1900,
        pTime->tm_mon + 1,
        pTime->tm_mday,
        pTime->tm_hour,
        pTime->tm_min,
        pTime->tm_sec);
    
    return pcaEnd;
}


long kr_time_interval(const char *pcaBegin, const char *pcaEnd)
{
    time_t  lTimeBegin, lTimeEnd;
    struct tm tTime;
    char    year[5], month[3], day[3], hour[3], minute[3], second[3];

    memset(year, 0, sizeof(year));
    memset(month, 0, sizeof(month));
    memset(day, 0, sizeof(day));
    memset(hour, 0, sizeof(hour));
    memset(minute, 0, sizeof(minute));
    memset(second, 0, sizeof(second));

    /* make up begin time */
    memcpy(year, pcaBegin, 4);
    memcpy(month, pcaBegin + 4, 2);
    memcpy(day, pcaBegin + 6, 2);
    memcpy(hour, pcaBegin + 8, 2);
    memcpy(minute, pcaBegin + 10, 2);
    memcpy(second, pcaBegin + 12, 2);

    tTime.tm_year  = atoi(year) - 1900;
    tTime.tm_mon   = atoi(month) - 1;
    tTime.tm_mday  = atoi(day);
    tTime.tm_hour  = atoi(hour);
    tTime.tm_min   = atoi(minute);
    tTime.tm_sec   = atoi(second);
    tTime.tm_isdst = -1;

    lTimeBegin = mktime(&tTime);
    
    /* make up end time */
    memcpy(year, pcaEnd, 4);
    memcpy(month, pcaEnd + 4, 2);
    memcpy(day, pcaEnd + 6, 2);
    memcpy(hour, pcaEnd + 8, 2);
    memcpy(minute, pcaEnd + 10, 2);
    memcpy(second, pcaEnd + 12, 2);

    tTime.tm_year  = atoi(year) - 1900;
    tTime.tm_mon   = atoi(month) - 1;
    tTime.tm_mday  = atoi(day);
    tTime.tm_hour  = atoi(hour);
    tTime.tm_min   = atoi(minute);
    tTime.tm_sec   = atoi(second);
    tTime.tm_isdst = -1;

    lTimeEnd = mktime(&tTime);
    
    return (long )(lTimeEnd - lTimeBegin);
}


time_t kr_time_to_ttime(char *pcaTime)
{
    char caYear[5], caMonth[3], caDay[3], caHour[3], caMin[3], caSec[3];
    struct tm   tTime;

    memset(caYear, 0, sizeof(caYear));
    memset(caMonth, 0, sizeof(caMonth));
    memset(caDay, 0, sizeof(caDay));
    memset(caHour, 0, sizeof(caHour));
    memset(caMin, 0, sizeof(caMin));
    memset(caSec, 0, sizeof(caSec));

    /* ¹¹Ôìdate */
    memcpy(caYear, pcaTime, 4);
    memcpy(caMonth, pcaTime + 4, 2);
    memcpy(caDay, pcaTime + 6, 2);
    memcpy(caHour, pcaTime + 8, 2);
    memcpy(caMin, pcaTime + 10, 2);
    memcpy(caSec, pcaTime + 12, 2);

    tTime.tm_year  = atoi(caYear) - 1900;
    tTime.tm_mon   = atoi(caMonth) - 1;
    tTime.tm_mday  = atoi(caDay);
    tTime.tm_hour  = atoi(caHour);
    tTime.tm_min   = atoi(caMin);
    tTime.tm_sec   = atoi(caSec);
    tTime.tm_isdst = -1;

    return mktime(&tTime);
}


char *kr_ttime_to_date(time_t lTime, char *pcaDate)
{
    struct tm *ptTime;
    ptTime = localtime(&lTime);
    sprintf(pcaDate, "%04d%02d%02d",
            ptTime->tm_year + 1900,
            ptTime->tm_mon + 1,
            ptTime->tm_mday);
            
    return pcaDate;
}

/* End of this file */
