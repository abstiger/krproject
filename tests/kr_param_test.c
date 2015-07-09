#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"

int main(int argc,char *argv[])
{
    kr_log_set_path(".");
    kr_log_set_name("param");
    kr_log_set_level(KR_LOGDEBUG);
    
    T_KRParam *ptParam = kr_param_create();

    /*
    T_KRParamPersistConfig stLoadConfig = {
        .type = 'F',
        .version = "version1",
        .value.file = "param.json",
    };
    */
    T_KRParamPersistConfig stLoadConfig = {
        .type = 'O',
        .version = "version1",
        .value.odbc.name = getenv("DBNAME"),
        .value.odbc.user = getenv("DBUSER"),
        .value.odbc.pass = getenv("DBPASS"),
    };
    
    kr_param_load(ptParam, &stLoadConfig);
    
    printf("load time:%ld, version:%s \n", \
        kr_param_load_time(ptParam),
        kr_param_version_get(ptParam));
    
    /*
    T_KRParamPersistConfig stDumpConfig = {
        .type = 'F',
        .version = "version1",
        .value.file = "param.json",
    };
    */
    T_KRParamPersistConfig stDumpConfig = {
        .type = 'O',
        .version = "version2",
        .value.odbc.name = getenv("DBNAME"),
        .value.odbc.user = getenv("DBUSER"),
        .value.odbc.pass = getenv("DBPASS"),
    };
    
    kr_param_dump(ptParam, &stDumpConfig);

    kr_param_destroy(ptParam);

    return 0;
}
