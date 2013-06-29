#include "krcalc/kr_calc.h"
#include <assert.h>

E_KRFieldType MyGetTypeFunc(char kind, int id, void *param)
{
    return KR_FIELDTYPE_INT;
}

void *MyGetValueFunc(char kind, int id, void *param)
{
    return &id;
}

int main(int argc, char *argv[])
{
    char calcstr[1024] = {0};
    T_KRCalc *mycalc = NULL;
    
    if (argc == 1)
        //strcpy(calcstr, "((4%3+4*27/(10-2)) > 12.99)&&(F_1 !@ {'shanghai','beijing','guangzhou',})&&(S_3 ## [^S_*])||(D_1 @@ A_1);");
        //strcpy(calcstr, "(F_1 !@ {'shanghai','beijing','guangzhou',});");
        strcpy(calcstr, "{\"op\":1,\"child\":[{\"kind\":3,\"value\":3}, {\"kind\":3,\"value\":5}]}");
    else
        strcpy(calcstr, argv[1]);
    
    mycalc = kr_calc_construct('J', calcstr, MyGetTypeFunc, MyGetValueFunc);
    assert(mycalc != NULL);
    
    kr_calc_dump(mycalc, stdout);
    
    if (kr_calc_check(mycalc) != 0) {
        fprintf(stderr, "kr_calc_check failed!\n");
        return -1;
    }
    
    if (kr_calc_eval(mycalc, NULL) != 0) {
        fprintf(stderr, "kr_calc_eval failed! %s\n", mycalc->err_msg);
        return -1;
    }
    
    switch(mycalc->result_type) {
    case KR_CALCTYPE_BOOLEAN:   
        printf("result_type:[%c] result_value:[%d]\n", mycalc->result_type, mycalc->result_value.b); 
        break;
    case KR_CALCTYPE_INTEGER:   
        printf("result_type:[%c] result_value:[%d]\n", mycalc->result_type, mycalc->result_value.i); 
        break;
    case KR_CALCTYPE_DOUBLE:   
        printf("result_type:[%c] result_value:[%lf]\n", mycalc->result_type, mycalc->result_value.d); 
        break;
    case KR_CALCTYPE_STRING:   
        printf("result_type:[%c] result_value:[%s]\n", mycalc->result_type, mycalc->result_value.s);    
        break;     
    default:
        printf("result_type:[%c] unknown!\n", mycalc->result_type, mycalc->result_value.s);   
        break; 
    }
    
    kr_calc_destruct(mycalc);
    
    exit(0);
}
