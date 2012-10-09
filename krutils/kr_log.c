#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

#include "kr_macros.h"
#include "kr_log.h"

typedef struct _kr_log_t {
    char        logpath[KR_FILENAME_LEN];
    char        logname[KR_FILENAME_LEN];
    KRLogLevel  loglevel;  
}T_KRLog;

T_KRLog gstKRLog = {".", "trace", KR_LOGWARNING};


void kr_log_set_path(char *path) 
{
    strcpy(gstKRLog.logpath, path);
}

void kr_log_set_name(char *name) 
{
    strcpy(gstKRLog.logname, name);
}

void kr_log_set_level(KRLogLevel level)
{
    gstKRLog.loglevel = level;
}

void kr_log(const char *file, int line, KRLogLevel level, const char *fmt, ...)
{
    FILE *fp = NULL;
    va_list         vlVar;
    char            caTimeString[80];
    char            caLogFile[2*KR_FILENAME_LEN+1];

    if (level > gstKRLog.loglevel) {
        return;
    }
    
    snprintf(caLogFile, sizeof(caLogFile), "%s/%s.%d.%lu.log", \
        gstKRLog.logpath, gstKRLog.logname, \
        (int)getpid(), (unsigned long)pthread_self());
    
    if ((fp = fopen(caLogFile, "a+")) == NULL) {
        return;
    }
    
    fprintf(fp, "--------------------------------------------------------------------------------\n");
    
    switch (level)
    {
        case KR_LOGFATAL:
            fprintf(fp, "FATAL:");
            break;
        case KR_LOGERROR:
            fprintf(fp, "ERROR:");
            break;
        case KR_LOGWARNING:
            fprintf(fp, "WARN :");
            break;
        case KR_LOGINFO:
            fprintf(fp, "INFO :");
            break;
        case KR_LOGDEBUG:
            fprintf(fp, "DEBUG:");
            break;
        default:
            fprintf(fp, "-----:");
            break;
    }

    time_t tTime = time(NULL);
    struct tm *ptmNow = localtime(&tTime);
    strftime(caTimeString, sizeof(caTimeString), "%c", ptmNow);

    va_start(vlVar, fmt);
    fprintf(fp, "%s:[%d]:%s,%d:\n", \
            caTimeString, getpid(), file, line);
    vfprintf(fp, fmt, vlVar);
    fprintf(fp, "\n");
    va_end(vlVar);
    
    fflush(fp);
    
    fclose(fp);
}

