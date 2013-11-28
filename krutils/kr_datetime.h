#ifndef __KR_DATETIME_H__
#define __KR_DATETIME_H__

#include <time.h>
#include "kr_types.h"

#define KR_DATE_LEN 8
#define KR_TIME_LEN 14

/* Time operating functions */
extern kr_bool   kr_date_isvalid(const char *pcaDate);
extern kr_bool   kr_date_isleap(const char *pcaDate);
extern char *kr_date_getmonthend(char *pcaDate);
extern char *kr_date_addmonth(const char *pcaBegin, int iMonths, char *pcaEnd);
extern char *kr_date_addday(const char *pcaBegin, int iDays, char *pcaEnd);
extern char *kr_time_system(char *pcaSysTime);
extern char *kr_time_addsec(const char *pcaBegin, long iSecs, char *pcaEnd);
extern long  kr_time_interval(const char *pcaBegin, const char *pcaEnd);
extern time_t kr_time_to_ttime(char *pcaTime);
extern char *kr_ttime_to_date(time_t lTime, char *pcaDate);

#endif /*__KR_DATETIME_H__*/
