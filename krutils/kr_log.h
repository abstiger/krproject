#ifndef __KR_LOG_H__
#define __KR_LOG_H__

/* loglevel enumeration */
typedef enum {
    KR_LOGFATAL   = 1,
    KR_LOGERROR   = 2,
    KR_LOGWARNING = 3,
    KR_LOGINFO    = 4,
    KR_LOGDEBUG   = 5
}KRLogLevel;

/* log functions */
extern void kr_log(const char *file, int line, KRLogLevel level, const char *fmt, ...);
extern void kr_log_set_path(char *path);
extern void kr_log_set_name(char *name) ;
extern void kr_log_set_level(KRLogLevel level);

/*define for easy log*/
#define KR_LOG(level, fmt, ...)   kr_log(__FILE__, __LINE__, level, fmt, ##__VA_ARGS__)

#endif /*__KR_LOG_H__*/
