#include "kr_calc.h"
#include "kr_calc_tree.h"
#include "kr_calc_parser_flex.h"
#include "kr_calc_parser_json.h"
#include "kr_calc_parser_gwt.h"


T_KRSet *kr_make_set_from_multi(E_KRCalcKind kind, char *string)
{
    E_KRFieldType type;
    switch(kind)
    {
        case KR_CALCKIND_MINT: type = KR_FIELDTYPE_INT; break;
        case KR_CALCKIND_MFLOAT: type = KR_FIELDTYPE_DOUBLE; break;
        case KR_CALCKIND_MSTRING: type = KR_FIELDTYPE_STRING; break;
        default: return NULL;
    }

    T_KRSet *krset = kr_set_create(string, type);
    char *multi_str = kr_strdup(string);
    char *save_str = NULL;
    char *p = strtok_r(multi_str, ",", &save_str);
    U_KRFieldVal val;
    while(p)
    {
        switch(krset->type) 
        {
            case KR_FIELDTYPE_INT:
                val.i=atoi(p); 
                kr_set_add(krset, &val.i);
                break;
            case KR_FIELDTYPE_DOUBLE:
                val.d=atof(p);
                kr_set_add(krset, &val.d);
                break;    
            case KR_FIELDTYPE_STRING:
                val.s=p;
                kr_set_add(krset, val.s);
                break;
        }
        p=strtok_r(NULL, ",", &save_str);
    }

    kr_free(multi_str);
    return krset;
}


int kr_calc_parse(T_KRCalc *krcalc)
{
    if (krcalc == NULL) return -1;
    int ret = 0;
    switch(krcalc->calc_format)
    {
        case KR_CALCFORMAT_FLEX:
            ret = kr_calc_parse_flex(krcalc);
            break;
        case KR_CALCFORMAT_JSON:
            ret = kr_calc_parse_json(krcalc);
            break;
        case KR_CALCFORMAT_GWT:
            ret = kr_calc_parse_gwt(krcalc);
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupport calc format:[%c]", 
                    krcalc->calc_format);
            ret = -1;
            break;
    }

    return ret;
}
