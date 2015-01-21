#include "kr_calc.h"
#include "kr_calc_tree.h"
#include "kr_calc_parser.h"
#include "kr_calc_dumper.h"


T_KRCalc *kr_calc_construct(E_KRCalcFormat format, char *calcstr, 
        KRGetTypeFunc get_type_func, KRGetValueFunc get_value_func)
{
    T_KRCalc *krcalc = (T_KRCalc *)kr_calloc(sizeof(*krcalc));
    krcalc->calc_format = format;
    krcalc->calc_string = kr_strdup(calcstr);
    
    krcalc->get_type_cb = get_type_func;
    krcalc->get_value_cb = get_value_func;
    
    /*parse it*/
    if (kr_calc_parse(krcalc) != 0) {
        KR_LOG(KR_LOGERROR, "kr_calc_parse failed [%s]!", 
                krcalc->calc_errmsg);
        kr_free(krcalc);
        return NULL;
    }
    
    return krcalc;
}

void kr_calc_destruct(T_KRCalc *krcalc)
{
    if (krcalc != NULL) {
        kr_free(krcalc->calc_string);
        kr_calc_tree_free(krcalc->calc_tree);
        kr_free(krcalc); krcalc = NULL;
    }
}

int kr_calc_check(T_KRCalc *krcalc)
{
    if (kr_calc_tree_check(krcalc->calc_tree, krcalc) != 0) {
        KR_LOG(KR_LOGERROR, "kr_calc_tree_check failed [%s]!", 
                krcalc->calc_errmsg);
        return -1;
    }
    return 0;
}

/* evaluate the calc with current record as parameter */
int kr_calc_eval(T_KRCalc *krcalc, void *param)
{
    krcalc->calc_param = param;

    if (kr_calc_tree_eval(krcalc->calc_tree, krcalc) != 0) {
        KR_LOG(KR_LOGERROR, "kr_calc_tree_eval %s failed", 
                krcalc->calc_string);
        return -1;
    }
    
    return 0;
}


int kr_calc_status(T_KRCalc *krcalc)
{
    return krcalc->calc_status;
}

char *kr_calc_errmsg(T_KRCalc *krcalc)
{
    return krcalc->calc_errmsg;
}

E_KRType kr_calc_type(T_KRCalc *krcalc)
{
    return krcalc->calc_tree->type;
}

E_KRValueInd kr_calc_ind(T_KRCalc *krcalc)
{
    return krcalc->calc_tree->ind;
}

U_KRValue *kr_calc_value(T_KRCalc *krcalc)
{
    return &krcalc->calc_tree->value;
}

