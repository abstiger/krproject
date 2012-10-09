#include "krcalc/kr_calc.h"
#include <assert.h>

E_KRFieldType MyGetTypeFunc(char *id, void *param)
{
    switch(id[0]) {
    case 'A':
    case 'F':
    case 'S':
    case 'D': 
    default:    
        return KR_FIELDTYPE_STRING;
    }
    
}

void *MyGetValueFunc(char *id, void *param)
{
    switch(id[0]) {
    case 'A':
    {
        T_KRSet *set = kr_set_create(id, KR_FIELDTYPE_STRING);
        kr_set_add(set, id);
        kr_set_add(set, "D_1");
        kr_set_add(set, "D_2");
        return (void *)set;
    }
    case 'F':
    case 'S':
    case 'D': 
    default:
        return id;
    }
}

int main(int argc, char *argv[])
{
    char calcstr[1024] = {0};
    T_KRCalc *mycalc = NULL;
    
    if (argv[1] == NULL)
        strcpy(calcstr, "((4%3+4*27/(10-2)) > 12.99)&&(F_1 !@ {'shanghai','beijing','guangzhou',})&&(S_3 ## [^S_*])||(D_1 @@ A_1);");
//        strcpy(calcstr, "(F_1 !@ {'shanghai','beijing','guangzhou',});");
    else
        strcpy(calcstr, argv[1]);
    
    mycalc = kr_calc_construct(KR_CALCBEHOOF_DATA, calcstr, MyGetTypeFunc, MyGetValueFunc);
    assert(mycalc != NULL);
    
    kr_calc_dump(mycalc, stdout);
    
    if (kr_calc_check(mycalc) != 0) {
        fprintf(stderr, "kr_calc_check failed!\n");
        return -1;
    }
    
    if (kr_calc_eval(mycalc, NULL) != 0) {
        fprintf(stderr, "kr_calc_eval failed!\n");
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
