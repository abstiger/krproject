#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krinput/kr_input.h"

int main(int argc,char *argv[])
{
//    kr_log_set_path(".");
//    kr_log_set_name("param");
//    kr_log_set_level(KR_LOGDEBUG);
    
    T_KRParam *ptParam = kr_param_create();

    T_KRParamPersistConfig stLoadConfig = {
        .type = 'O',
        .version = "version1",
        .value.odbc.name = getenv("DBNAME"),
        .value.odbc.user = getenv("DBUSER"),
        .value.odbc.pass = getenv("DBPASS"),
    };
    kr_param_load(ptParam, &stLoadConfig);
    
    
    T_KRModule *ptIOModule = kr_module_open("/home/tiger/krproject/lib/libkriface.so", RTLD_LAZY);
    T_KRInput *ptInput = kr_input_construct(ptParam, ptIOModule);
    if (ptInput == NULL) {
        fprintf(stderr, "kr_input_construct failed!");
        return -1;
    }

    T_KRMessage stMessage = {
        .msgtype = 0,
        .msgid = "000001",
        .datasrc = 1,
        .msgfmt = "json",
        .msglen = 40,
        .msgbuf = "{\"field_0\": 10,\"field_1\":  \"haha\"}",
    };
    T_KRRecord *ptRecord = kr_input_process(ptInput, &stMessage);
    if (ptRecord == NULL) {
        fprintf(stderr, "kr_input_process failed!");
        return -1;
    }
    fprintf(stderr, "field_0:%d\n", *(int *)kr_record_get_field_value(ptRecord, 0));
    fprintf(stderr, "field_1:%s\n", (char *)kr_record_get_field_value(ptRecord, 1));

    kr_input_check(ptInput, ptParam);

    kr_input_destruct(ptInput);

    kr_param_destroy(ptParam);

    return 0;
}
