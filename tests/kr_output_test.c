#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krparam/kr_param_class_output.h"
#include "kroutput/kr_output.h"

#define STR(...) #__VA_ARGS__

static char *psParamOutput1 = STR({
        "id": 1,
        "name": "name_1",
        "desc": "desc_1",
        "fields": [
          {
            "id": 0,
            "name": "field_0",
            "desc": "field_0",
            "type": "I",
            "length": 8,
            "calc_format": "F", 
            "calc_string": "1+3;"
          },
          {
            "id": 1,
            "name": "field_1",
            "desc": "field_1",
            "type": "S",
            "length": 20,
            "calc_format": "F", 
            "calc_string": "C_1;"
          }
        ]
      });

static char *psModuleFile = "/home/tiger/krproject/lib/libkriface.so";

E_KRType my_get_type(char kind, int id, void *data)
{
    return KR_TYPE_STRING;
}

void *my_get_value(char kind, int id, void *data)
{
    return "Const String!";
}


int main(int argc,char *argv[])
{
    T_KRParam *ptParam = kr_param_create();
    kr_param_object_add(ptParam, KR_PARAM_OUTPUT, "1", psParamOutput1);
    kr_param_switch(ptParam);

    T_KROutput *ptOutput = kr_output_construct(ptParam, psModuleFile,
                        my_get_type, my_get_value);
    if (ptOutput == NULL) {
        fprintf(stderr, "kr_output_construct failed!");
        return -1;
    }

    //kr_output_check(ptOutput, ptParam);
    T_KRContext *ptContext = kr_context_create(NULL);
    int iOutputId = 1;
    char *psOutputFmt = "json";
    kr_context_add_data(ptContext, "output_id", &iOutputId);
    kr_context_add_data(ptContext, "output_fmt", psOutputFmt);

    T_KRResponse *ptResponse = kr_output_process(ptOutput, ptContext);
    if (ptResponse == NULL) {
        fprintf(stderr, "kr_output_process failed!");
        return -1;
    }

    fprintf(stderr, "output msglen:%zu msgbuf:%s \n", 
            ptResponse->msglen,
            ptResponse->msgbuf);

    kr_response_free(ptResponse);
    kr_context_destroy(ptContext);
    kr_output_destruct(ptOutput);
    kr_param_destroy(ptParam);

    return 0;
}
