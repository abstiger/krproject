#include <stdio.h>
#include <stdlib.h>
#include "krutils/kr_log.h"

int main()
{
    KR_LOG(KR_LOGFATAL, "%s", "this is a fatal log");
    KR_LOG(KR_LOGERROR, "%s", "this is a error log");
    KR_LOG(KR_LOGWARNING, "%s", "this is a warning log");
    KR_LOG(KR_LOGINFO, "%s", "this is a info log");
    KR_LOG(KR_LOGDEBUG, "%s", "this is a debug log");
    
    kr_log_set_path(".");
    kr_log_set_level(KR_LOGERROR);

    KR_LOG(KR_LOGFATAL, "%s", "this is a fatal log");
    KR_LOG(KR_LOGERROR, "%s", "this is a error log");
    KR_LOG(KR_LOGWARNING, "%s", "this is a warning log");
    KR_LOG(KR_LOGINFO, "%s", "this is a info log");
    KR_LOG(KR_LOGDEBUG, "%s", "this is a debug log");
    
    return 0;
}
