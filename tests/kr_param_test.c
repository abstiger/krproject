#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"

int main(int argc,char *argv[])
{
    T_KRParam *ptParam = kr_param_create();
    
    T_KRParamPersistConfig stLoadConfig = {
        .type = 'F',
        .version = "version1",
        .value.file = "param_load.json"
    };
    kr_param_load(ptParam, &stLoadConfig);

    T_KRParamPersistConfig stDumpConfig = {
        .type = 'F',
        .version = "version1",
        .value.file = "param_dump.json"
    };
    kr_param_dump(ptParam, &stDumpConfig);

    kr_param_destroy(ptParam);

    return 0;
}
