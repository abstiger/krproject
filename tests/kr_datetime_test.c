#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "krutils/kr_macros.h"
#include "krutils/kr_datetime.h"

int main()
{
    kr_bool ret = FALSE;
    char caDate[KR_DATE_LEN+1];
    char caTempDate[KR_DATE_LEN+1];
    char caTime[KR_TIME_LEN+1];
    char caTempTime[KR_TIME_LEN+1];
    
    ret = kr_date_isvalid("00341132");
    assert(!ret);
    ret = kr_date_isvalid("20120329");
    assert(ret);
    
    ret = kr_date_isleap("20010101");
    assert(!ret);
    ret = kr_date_isleap("20080101");
    assert(ret);
    
    strcpy(caDate, "20120329");
    
    printf("kr_date_getmonthend 20120329 [%s]\n", kr_date_getmonthend(caDate));
    printf("kr_date_addmonth [%s] [%d]:[%s]\n", caDate, 1, kr_date_addmonth(caDate, 1, caTempDate));
    printf("kr_date_addday [%s] [%d]:[%s]\n", caDate, -30, kr_date_addday(caDate, -30, caTempDate));
    printf("kr_time_system [%s]\n", kr_time_system(caTime));
    printf("kr_time_addsec [%s] [%d]:[%s]\n", caTime, 50, kr_time_addsec(caTime, 50, caTempTime));
    printf("kr_time_interval[%s] [%s]:[%ld]\n", caTime, caTempTime, kr_time_interval(caTime, caTempTime));
    
    return 0;
}

