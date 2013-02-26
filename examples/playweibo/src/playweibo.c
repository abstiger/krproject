#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "parson.h"


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

void *weibo_map_func_pre(void *msg)
{
    JSON_Value *jsonval = json_parse_string((char *)msg);
    return jsonval;
}


void weibo_map_func_post(void *data)
{
    json_value_free(data);
}


void weibo_map_func(void *fldval, int fldno, int fldlen, void *data)
{
    JSON_Object *jsonobj = json_value_get_object(data);
    memset(fldval, 0x00, fldlen);
    switch(fldno)
    {
        case 0:
            *(long *)fldval = (long )time(NULL);
            break;
        case 1:
        {
            char caDateTime[14+1] = {0};
            snprintf(caDateTime, 15, "%8s%6s", \
                    json_object_get_string(jsonobj, "txndate"), 
                    json_object_get_string(jsonobj, "txntime"));
            *(long *)fldval = kr_time_to_ttime(caDateTime);
            break;
        }
        case 2:
            printf("custno=[%s]\n", json_object_get_string(jsonobj, "custno"));
            memcpy(fldval, json_object_get_string(jsonobj, "custno"), fldlen);
            break;
        case 3:
            printf("txndate=[%s]\n",json_object_get_string(jsonobj, "txndate"));
            memcpy(fldval, json_object_get_string(jsonobj, "txndate"), fldlen);
            break;    
        case 4:
            printf("txntime=[%s]\n",json_object_get_string(jsonobj, "txntime"));
            memcpy(fldval, json_object_get_string(jsonobj, "txntime"), fldlen);
            break;
        case 5:
            printf("flowno=[%s]\n", json_object_get_string(jsonobj, "flowno"));
            memcpy(fldval, json_object_get_string(jsonobj, "flowno"), fldlen);
            break;
        case 6:
            printf("txntype=[%s]\n",json_object_get_string(jsonobj, "txntype"));
            memcpy(fldval, json_object_get_string(jsonobj, "txntype"), fldlen);
            break;
        case 7:
            printf("txnamt=[%lf]\n", json_object_get_number(jsonobj, "txnamt"));
            *(double *)fldval = json_object_get_number(jsonobj, "txnamt");
            break;
        case 8:
            printf("txnloc=[%s]\n", json_object_get_string(jsonobj, "txnloc"));
            memcpy(fldval, json_object_get_string(jsonobj, "txnloc"), fldlen);
            break;
        default:
            break;
    }
}

