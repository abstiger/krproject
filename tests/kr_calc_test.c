#include "krcalc/kr_calc.h"
#include <assert.h>

E_KRType get_type(char kind, int id, void *param)
{
    return KR_TYPE_STRING;
}

void *get_value(char kind, int id, void *param)
{
    return &id;
}

int main(int argc, char *argv[])
{
    char calcfmt;
    char calcstr[1024] = {0};
    T_KRCalc *mycalc = NULL;

    if (argc == 1) {
        calcfmt = 'F';    
        strcpy(calcstr, "((((((4 % 3) + ((4 * 27) / (10 - 2))) > 12.99) && (F_1 !@ {'shanghai','beijing','guangzhou',})) && (S_3 ## [^S_*])) || (D_1 < D_2));");
        //strcpy(calcstr, "((4%3+4*27/(10-2)) > 12.99)&&(F_1 !@ {'shanghai','beijing','guangzhou',})&&(S_3 ## [^S_*])||(D_1 < D_2);");
        //strcpy(calcstr, "(F_1 !@ {'shanghai','beijing','guangzhou',});");
        //strcpy(calcstr, "{\"op\":1,\"child\":[{\"kind\":3,\"value\":3}, {\"kind\":3,\"value\":5}]}");
        //strcpy(calcstr, "{\"op\":16,\"child\":[{\"op\":12,\"child\":[{\"kind\":12,\"value\":2},{\"kind\":8,\"value\":\"00,01,02,\"}]},{\"op\":9,\"child\":[{\"kind\":14,\"value\":1},{\"op\":3,\"child\":[{\"kind\":13,\"value\":2},{\"kind\":15,\"value\":1}]}]},{\"op\":17,\"child\":[{\"op\":11,\"child\":[{\"kind\":12,\"value\":1},{\"kind\":5,\"value\":\"Tiger\"}]},{\"op\":8,\"child\":[{\"kind\":11,\"value\":2},{\"op\":1,\"child\":[{\"kind\":12,\"value\":2},{\"kind\":3,\"value\":25}]}]}]}]}");
    } else {
        calcfmt = argv[1][0];
        strcpy(calcstr, argv[2]);
    }

    mycalc = kr_calc_construct(calcfmt, calcstr, get_type, get_value);
    assert(mycalc != NULL);

    char *flexstr = kr_calc_dump(mycalc, KR_CALCFORMAT_FLEX);
    fprintf(stdout, "dump calc flex: %s \n", flexstr);
    kr_free(flexstr);

    char *jsonstr = kr_calc_dump(mycalc, KR_CALCFORMAT_JSON);
    fprintf(stdout, "dump calc json: %s \n", jsonstr);
    kr_free(jsonstr);


    if (kr_calc_check(mycalc) != 0) {
        fprintf(stderr, "kr_calc_check failed!\n");
        return -1;
    }

    if (kr_calc_eval(mycalc, NULL) != 0) {
        fprintf(stderr, "kr_calc_eval failed! %s\n", kr_calc_errmsg(mycalc));
        return -1;
    }

    switch(kr_calc_type(mycalc)) {
        case KR_TYPE_BOOL:   
            printf("result_type:[%c] result_value:[%d]\n", 
                    kr_calc_type(mycalc), kr_calc_value(mycalc)->b); 
            break;
        case KR_TYPE_INT:   
            printf("result_type:[%c] result_value:[%d]\n", 
                    kr_calc_type(mycalc), kr_calc_value(mycalc)->i); 
            break;
        case KR_TYPE_DOUBLE:   
            printf("result_type:[%c] result_value:[%lf]\n", 
                    kr_calc_type(mycalc), kr_calc_value(mycalc)->d); 
            break;
        case KR_TYPE_STRING:   
            printf("result_type:[%c] result_value:[%s]\n", 
                    kr_calc_type(mycalc), kr_calc_value(mycalc)->s); 
            break;     
        default:
            printf("result_type:[%c] unknown!\n", kr_calc_type(mycalc)); 
            break; 
    }

    kr_calc_destruct(mycalc);

    exit(0);
}
