#include "kr_calc.h"
#include "kr_calc_tree.h"
#include "kr_calc_dumper_flex.h"


/* print operator op, refer to the lexer file's definition
 */
static char *_kr_calc_dump_op(E_KRCalcOp op, char *str)
{ 
    switch (op)
    {
        case KR_CALCOP_AND: str += sprintf(str, " && "); break;
        case KR_CALCOP_OR: str += sprintf(str, " || "); break;
        case KR_CALCOP_NOT: str += sprintf(str, " ! "); break;

        case KR_CALCOP_PLUS: str += sprintf(str, " + "); break;
        case KR_CALCOP_SUB: str += sprintf(str, " - "); break;
        case KR_CALCOP_MUT: str += sprintf(str, " * "); break;
        case KR_CALCOP_DIV: str += sprintf(str, " / "); break;
        case KR_CALCOP_MOD: str += sprintf(str, " %% "); break;

        case KR_CALCOP_LT: str += sprintf(str, " < "); break;
        case KR_CALCOP_LE: str += sprintf(str, " <= "); break;
        case KR_CALCOP_GT: str += sprintf(str, " > "); break;
        case KR_CALCOP_GE: str += sprintf(str, " >= "); break;
        case KR_CALCOP_EQ: str += sprintf(str, " == "); break;
        case KR_CALCOP_NEQ: str += sprintf(str, " != "); break;
        case KR_CALCOP_BL: str += sprintf(str, " @@ "); break;
        case KR_CALCOP_NBL: str += sprintf(str, " !@ "); break;
        case KR_CALCOP_MATCH: str += sprintf(str, " ## "); break;

        default: return NULL;
    }

    return str;
}

/* preprocess of printing calctree node  */
static char *_kr_calc_dump_flex(T_KRCalcTree *t, char *str)
{ 
    switch (t->kind) 
    {
        case KR_CALCKIND_REL:
        case KR_CALCKIND_ARITH:
        case KR_CALCKIND_LOGIC:    
        {
            str += sprintf(str, "(");
            for (int i; i<t->childnum; ++i) {
                str = _kr_calc_dump_flex(t->children[i], str);
                if (i != t->childnum-1) {
                    str = _kr_calc_dump_op(t->op, str);
                }
            }
            str += sprintf(str, ")");
            break;
        }
        case KR_CALCKIND_INT:
            str += sprintf(str, "%d", t->value.i);
            break;
        case KR_CALCKIND_FLOAT:
            str += sprintf(str, "%g", t->value.d);
            break;
        case KR_CALCKIND_STRING:
            str += sprintf(str, "%s", t->value.s);
            break;
        case KR_CALCKIND_MINT:    
        case KR_CALCKIND_MFLOAT:    
        case KR_CALCKIND_MSTRING:    
        {
            T_KRHashSet *set = (T_KRHashSet *)t->value.p;
            str += sprintf(str, "{");
            str += sprintf(str,"%s", set->name);
            str += sprintf(str, "}");
            break;
        }
        case KR_CALCKIND_REGEX:
        {
            T_KRRegex *regex = (T_KRRegex *)t->value.p;
            str += sprintf(str, "[");
            str += sprintf(str,"%s", regex->pattern);
            str += sprintf(str, "]");
            break;    
        }
        case KR_CALCKIND_SET:
            str += sprintf(str, "A_%d", t->id);
            break;
        case KR_CALCKIND_CID:
            str += sprintf(str, "C_%d", t->id);
            break;
        case KR_CALCKIND_FID:
            str += sprintf(str, "F_%d", t->id);
            break;
        case KR_CALCKIND_SID:
            str += sprintf(str, "S_%d", t->id);
            break;
        case KR_CALCKIND_DID:
            str += sprintf(str, "D_%d", t->id);
            break;    
        case KR_CALCKIND_HID:
            str += sprintf(str, "H_%d", t->id);
            break;    
        default:
            return NULL;
    }

    return str;
}

/* prints a calctree to the 
 *  * printer file using indentation to indicate subtrees
 *   */
char *kr_calc_dump_flex(T_KRCalc *krcalc)
{
    char *dump_str = NULL;

    //FIXME:length can't be fixed
    dump_str = kr_calloc(4096);
    char *str = _kr_calc_dump_flex(krcalc->calc_tree, dump_str);
    sprintf(str, ";");

    return dump_str;
}


