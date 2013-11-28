#include "kr_calc.h"
#include "kr_calc_tree.h"

extern int kr_calc_parse(T_KRCalc *krcalc);

T_KRCalc *kr_calc_construct(E_KRCalcFormat format, char *calcstr)
{
    T_KRCalc *krcalc = (T_KRCalc *)kr_calloc(sizeof(T_KRCalc));
    krcalc->calc_format = format;
    krcalc->calc_string = kr_strdup(calcstr);
    
    krcalc->get_type_cb = kr_data_get_type;
    krcalc->get_value_cb = kr_data_get_value;
    
    if (kr_calc_parse(krcalc) != 0) {
        fprintf(stderr, "kr_calc_parse failed[%s]!\n", krcalc->err_msg);
        return NULL;
    }
    kr_calc_dump(krcalc, stdout);
    
    return krcalc;
}

void kr_calc_destruct(T_KRCalc *krcalc)
{
    if (krcalc != NULL) {
        kr_free(krcalc->calc_string);
        kr_calctree_free(krcalc->calc_tree);
        kr_free(krcalc); krcalc = NULL;
    }
}

int kr_calc_check(T_KRCalc *krcalc)
{
    if (kr_calctree_check(krcalc->calc_tree, krcalc->err_msg) != 0) {
        fprintf(stderr, "kr_calctree_check failed [%s]!\n", krcalc->err_msg);
        return -1;
    }
    return 0;
}

/* evaluate the calc with current record as parameter */
int kr_calc_eval(T_KRCalc *krcalc, void *param)
{
    T_KRCalcTree *t=krcalc->calc_tree;
    krcalc->data = param;
    if (kr_calctree_eval(t, krcalc) != 0) {
        fprintf(stderr, "kr_calctree_eval %s failed\n", krcalc->calc_string);
        return -1;
    }
    
    krcalc->result_ind = t->ind;
    krcalc->result_type = t->type;
    memcpy(&krcalc->result_value, &t->attr.val, sizeof(U_KRValue));
    
    return 0;
}


/* print the calc to the printer */
void kr_calc_dump(T_KRCalc *krcalc, FILE *fp)
{
    fprintf(fp, "Dumping Calc:[%c][%s]:\n    ", \
            krcalc->calc_format, krcalc->calc_string);
    fprintf(fp, "cid_cnt:[%d],", krcalc->cid_cnt);
    fprintf(fp, "fid_cnt:[%d],", krcalc->fid_cnt);
    fprintf(fp, "sid_cnt:[%d],", krcalc->sid_cnt);
    fprintf(fp, "did_cnt:[%d],", krcalc->did_cnt);
    fprintf(fp, "hid_cnt:[%d],", krcalc->hid_cnt);
    fprintf(fp, "set_cnt:[%d],", krcalc->set_cnt);
    fprintf(fp, "multi_cnt:[%d],", krcalc->multi_cnt);
    fprintf(fp, "regex_cnt:[%d]\n", krcalc->regex_cnt);
    kr_calctree_dump(krcalc->calc_tree, fp);
}
