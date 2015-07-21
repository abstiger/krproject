#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"

int main(int argc,char *argv[])
{
    //kr_log_set_path(".");
    //kr_log_set_name("param");
    //kr_log_set_level(KR_LOGDEBUG);
    
    T_KRParam *ptParam = kr_param_create();

    kr_param_load(ptParam, "odbc:version1:tiger/haha@sqlite3");
    
    printf("load time:%ld, version:%s \n", \
            kr_param_load_time(ptParam),
            kr_param_version_get(ptParam));
    
    kr_param_dump(ptParam, "file:version1:param.json");

    kr_param_destroy(ptParam);

    return 0;
}
