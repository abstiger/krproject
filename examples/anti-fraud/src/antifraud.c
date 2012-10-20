#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _kr_tradflow_t{
    char     caOutCustNo[20+1];
    char     caOutTransDate[8+1];
    char     caOutTransTime[6+1];
    char     caOutFlowNo[15+1];
    char     caOutTransType[2+1];
    double   dOutTransAmt;
    char     caOutTransLoc[100+1];
} T_KRTradFlow_1;


static time_t kr_time_to_ttime(char *pcaTime)
{
    char caYear[5], caMonth[3], caDay[3], caHour[3], caMin[3], caSec[3];
	struct tm   tTime;

	memset(caYear, 0, sizeof(caYear));
	memset(caMonth, 0, sizeof(caMonth));
	memset(caDay, 0, sizeof(caDay));
	memset(caHour, 0, sizeof(caHour));
	memset(caMin, 0, sizeof(caMin));
	memset(caSec, 0, sizeof(caSec));

	/* 构造date */
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

void map_func_1(void *fldval, int fldno, int fldlen, void *data)
{
    T_KRTradFlow_1 *ptReqDef = (T_KRTradFlow_1 *)data;
	char caTransDateTime[14+1] = {0};
    memset(fldval, 0x00, fldlen);
    switch(fldno)
    {
        case 0:
            *(long *)fldval = (long )time(NULL);
            break;
        case 1:
            snprintf(caTransDateTime, 15, "%8s%6s", \
                    ptReqDef->caOutTransDate, 
                    ptReqDef->caOutTransTime);
            *(long *)fldval = kr_time_to_ttime(caTransDateTime);
            break;
        case 2:
            memcpy(fldval, ptReqDef->caOutCustNo, fldlen);
            break;
        case 3:
            memcpy(fldval, ptReqDef->caOutTransDate, fldlen);
            break;    
        case 4:
            memcpy(fldval, ptReqDef->caOutTransTime, fldlen);
            break;
        case 5:
            memcpy(fldval, ptReqDef->caOutFlowNo, fldlen);
            break;
        case 6:
            memcpy(fldval, ptReqDef->caOutTransType, fldlen);
            break;
        case 7:
            *(double *)fldval = ptReqDef->dOutTransAmt;
            break;
        case 8:
            memcpy(fldval, ptReqDef->caOutTransLoc, fldlen);
            break;
        default:
            break;
    }
}
