#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krparam/kr_param_class_input.h"
#include "krinput/kr_input.h"

#define STR(...) #__VA_ARGS__

static char *psParamInput1 = STR({
        "id": 1,
        "name": "name_1",
        "desc": "desc_1",
        "keep_mode": "0",
        "keep_value": 100,
        "fields": [
          {
            "id": 0,
            "name": "field_0",
            "desc": "field_0",
            "type": "I",
            "length": 8
          },
          {
            "id": 1,
            "name": "field_1",
            "desc": "field_1",
            "type": "S",
            "length": 10
          }
        ]
      });

static T_KRMessage stInputMessage = {
        .msgtype = 0,
        .msgid = "000001",
        .datasrc = 1,
        .msgfmt = "json",
        .msglen = 40,
        .msgbuf = STR({"field_0": 10,"field_1": "haha"}),
    };

static char *psModuleFile = "/home/tiger/krproject/lib/libkriface.so";

int main(int argc,char *argv[])
{
//    kr_log_set_path(".");
//    kr_log_set_name("param");
//    kr_log_set_level(KR_LOGDEBUG);
    
    T_KRParam *ptParam = kr_param_create();
    kr_param_object_add(ptParam, KR_PARAM_INPUT, "1", psParamInput1);
    kr_param_switch(ptParam);
    
    T_KRInput *ptInput = kr_input_construct(ptParam, psModuleFile);
    if (ptInput == NULL) {
        fprintf(stderr, "kr_input_construct failed!");
        return -1;
    }

    T_KRRecord *ptRecord = kr_input_process(ptInput, &stInputMessage);
    if (ptRecord == NULL) {
        fprintf(stderr, "kr_input_process failed!");
        return -1;
    }
    
    fprintf(stderr, "field_0:%d\n", *(int *)kr_record_get_field_value(ptRecord, 0));
    fprintf(stderr, "field_1:%s\n", (char *)kr_record_get_field_value(ptRecord, 1));

    //kr_input_check(ptInput, ptParam);

    kr_input_destruct(ptInput);

    kr_param_destroy(ptParam);

    return 0;
}
