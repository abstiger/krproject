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

#define KR_LOGFILE_LEN 1024

static void kr_hex_dump(FILE *fp, char *addr, int len);

typedef struct _kr_log_t {
    char        logpath[KR_LOGFILE_LEN];
    char        logname[KR_LOGFILE_LEN];
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
    char            caLogFile[2*KR_LOGFILE_LEN+1];

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


void kr_log_hex(const char *file, int line, KRLogLevel level, char *buf, int len)
{
    FILE *fp = NULL;
    va_list         vlVar;
    char            caTimeString[80];
    char            caLogFile[2*KR_LOGFILE_LEN+1];

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

    fprintf(fp, "%s:[%d]:%s,%d:\n", \
            caTimeString, getpid(), file, line);
    
    kr_hex_dump(fp, buf, len);
    fflush(fp);
    
    fclose(fp);
}


static void kr_hex_dump(FILE *fp, char *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                fprintf(fp, "  %s\n", buff);

            // Output the offset.
            fprintf(fp, "  %04x ", i);
        }

        // Now the hex code for the specific character.
        fprintf(fp, " %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        fprintf(fp, "   ");
        i++;
    }

    // And print the final ASCII bit.
    fprintf(fp, "  %s\n", buff);
}
