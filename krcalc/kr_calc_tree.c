#include "kr_calc_tree.h"
#include "krutils/kr_utils.h"

/* calctree traversal function definition */
typedef int (*traverse_func)(T_KRCalcTree *node, void *data);


/* create a new calctree node */
T_KRCalcTree *kr_calc_tree_new(E_KRCalcKind kind)
{ 
    T_KRCalcTree *t = (T_KRCalcTree *)kr_calloc(sizeof(*t));
    t->kind = kind;
    t->type = KR_TYPE_UNKNOWN;
    t->ind  = KR_VALUE_UNSET;
    return t;
}

/* append a calctree child */
void kr_calc_tree_append(T_KRCalcTree *t, T_KRCalcTree *child)
{ 
    t->children = kr_realloc(t->children, sizeof(*child)*(t->childnum+1));
    t->children[t->childnum] = child;
    t->childnum++;
}

/* procedure kr_calc_tree_traverse is a generic recursive
 * syntax tree traversal routine:
 * it applies pre_proc in preorder and post_proc in postorder
 * to tree pointed to by t
 */
static int kr_calc_tree_traverse(T_KRCalcTree *t, void *data,
                     traverse_func pre_proc, traverse_func post_proc)
{ 
    if (t != NULL) { 
        if (pre_proc != NULL) {
            if (pre_proc(t, data) != 0) return -1;
        }
        
        for (int i=0; i < t->childnum; i++) {
            if (kr_calc_tree_traverse(t->children[i], data, 
                        pre_proc, post_proc) != 0) {
                return -1;
            }
        }
        
        if (post_proc != NULL) {
            if (post_proc(t, data) != 0) return -1;
        }
    }
    return 0;
}

/* procedure checkNode performs
 * type checking at a single tree node
 */
static int _kr_calc_tree_check(T_KRCalcTree *t, T_KRCalc *krcalc)
{ 
    switch (t->kind)
    { 
        case KR_CALCKIND_ARITH:
        {
            if (t->childnum != 2) {
                krcalc->calc_status = -1;
                snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                        "Arith Op[%d] need two childes!", t->op);
                return -1;
            }
            T_KRCalcTree *left = t->children[0];
            T_KRCalcTree *right = t->children[1];
            if (left->type == KR_TYPE_UNKNOWN || 
                    right->type == KR_TYPE_UNKNOWN) {
                return 0;
            }
            
            switch(t->op) 
            {
                case KR_CALCOP_PLUS: case KR_CALCOP_SUB: 
                case KR_CALCOP_MUT: case KR_CALCOP_DIV: 
                case KR_CALCOP_MOD:
                    if ((left->type != KR_TYPE_INT && 
                         left->type != KR_TYPE_LONG &&
                         left->type != KR_TYPE_DOUBLE) ||
                        (right->type != KR_TYPE_INT && 
                         right->type != KR_TYPE_LONG && 
                         right->type != KR_TYPE_DOUBLE)) 
                    {
                        krcalc->calc_status = -1;
                        snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                                    "Arithmetic Op[%d] Type Error[%c][%c].",
                                    t->op, left->type, right->type);
                        return -1;
                    }
                    break;
                default:
                    krcalc->calc_status = -1;
                    snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                               "[%d] not an Arithmetic Op.\n", t->op);
                    return -1;
            }
            break;
        }
        case KR_CALCKIND_LOGIC:
        {
            if (t->childnum != 2) {
                krcalc->calc_status = -1;
                snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                        "Logic Op[%d] need two childes!", t->op);
                return -1;
            }
            T_KRCalcTree *left = t->children[0];
            T_KRCalcTree *right = t->children[1];
            if (left->type == KR_TYPE_UNKNOWN || 
                    right->type == KR_TYPE_UNKNOWN) {
                return 0;
            }

            switch(t->op) {
                case KR_CALCOP_LT: case KR_CALCOP_LE: 
                case KR_CALCOP_GT: case KR_CALCOP_GE: 
                case KR_CALCOP_EQ: case KR_CALCOP_NEQ:
                    if ((left->type == KR_TYPE_STRING && 
                         right->type != KR_TYPE_STRING) ||
                        (right->type == KR_TYPE_STRING && 
                         left->type != KR_TYPE_STRING)) 
                    {
                        krcalc->calc_status = -1;
                        snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                                "Logic Op[%d] Type Error[%c][%c].", \
                                t->op, left->type, right->type);
                        return -1;
                    }
                    break;
                case KR_CALCOP_BL: case KR_CALCOP_NBL:
                    if ((right->kind != KR_CALCKIND_MINT && 
                         right->kind != KR_CALCKIND_MFLOAT && 
                         right->kind != KR_CALCKIND_MSTRING && 
                         right->kind != KR_CALCKIND_SET) ||
                         left->type != right->type) 
                    {
                        krcalc->calc_status = -1;
                        snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                                "Logic Op[%d] Type Error[%c][%c][%d].",
                                t->op, left->type, right->type, 
                                right->kind);
                        return -1;        
                    }
                    break;
                case KR_CALCOP_MATCH:
                    if (left->type != KR_TYPE_STRING ||
                        right->kind != KR_CALCKIND_REGEX) 
                    {
                        krcalc->calc_status = -1;
                        snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                                "Logic Op[%d] Type Error[%c][%c][%d].",
                                t->op, left->type, right->type,
                                right->kind);
                        return -1;
                    }
                    break;
                default:
                    krcalc->calc_status = -1;
                    snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                            "[%d] is not an Logical Op.\n", t->op);
                    return -1;
            }
            break;
        }
        case KR_CALCKIND_REL:
        {
            switch(t->op) {
                case KR_CALCOP_AND: case KR_CALCOP_OR: case KR_CALCOP_NOT:
                {
                    for (int i=0; i < t->childnum; i++) {
                        T_KRCalcTree *child = t->children[i];
                        if (child->type == KR_TYPE_UNKNOWN) {
                            return 0;
                        }
                        if (child->type != KR_TYPE_BOOL && 
                                child->type != KR_TYPE_INT) {
                            krcalc->calc_status = -1;
                            snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                                    "Rel Op[%d] Type Error[%c].", \
                                    t->op, child->type);
                            return -1;
                        }

                    }
                    break;
                }
                default:
                {
                    krcalc->calc_status = -1;
                    snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                            "[%d] is not an Relation Op.\n", t->op);
                    return -1;
                }
            }
            break;
        }
    }
    return 0;
}

/* procedure typeCheck performs type checking 
 *  * by a postorder syntax tree traversal
 *   */
int kr_calc_tree_check(T_KRCalcTree *root, T_KRCalc *krcalc)
{
    return kr_calc_tree_traverse(root, krcalc, 
            NULL, (traverse_func )_kr_calc_tree_check);
}

/*do the relation evaluation*/
static int _kr_calc_tree_eval_rel(T_KRCalcTree *t, T_KRCalc *krcalc)
{
    t->ind = KR_VALUE_UNSET;

    kr_bool b;
    switch(t->op) {
        case KR_CALCOP_AND: 
            {
                b = TRUE;
                for (int i=0; i < t->childnum; i++) {
                    T_KRCalcTree *child = t->children[i];
                    if (child->ind != KR_VALUE_SETED) {
                        return -1;
                    }
                    
                    if (child->type == KR_TYPE_BOOL) {
                        if (!child->value.b) {
                            b = FALSE;
                            break;
                        }
                    } else if (child->type == KR_TYPE_INT) {
                        if (!child->value.i) {
                            b = FALSE;
                            break;
                        }
                    } else {
                        krcalc->calc_status = -1;
                        snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                                "op[%d] type [%c] not valided!", 
                                t->op, child->type);
                        return -1;
                    }
                }
                break;
            }
        case KR_CALCOP_OR:  
            {
                b = FALSE;
                for (int i=0; i < t->childnum; i++) {
                    T_KRCalcTree *child = t->children[i];
                    if (child->ind != KR_VALUE_SETED) {
                        return -1;
                    }
                    
                    if (child->type == KR_TYPE_BOOL) {
                        if (child->value.b) {
                            b = TRUE;
                            break;
                        }
                    } else if (child->type == KR_TYPE_INT) {
                        if (child->value.i) {
                            b = TRUE;
                            break;
                        }
                    } else {
                        krcalc->calc_status = -1;
                        snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                                "op[%d] type [%c]  not valided!", 
                                t->op, child->type);
                        return -1;
                    }
                }
                break;
            }
        case KR_CALCOP_NOT: 
            {
                b = TRUE;
                for (int i=0; i < t->childnum; i++) {
                    T_KRCalcTree *child = t->children[i];
                    if (child->ind != KR_VALUE_SETED) {
                        return -1;
                    }
                    
                    if (child->type == KR_TYPE_BOOL) {
                        if (child->value.b) {
                            b = FALSE;
                            break;
                        }
                    } else if (child->type == KR_TYPE_INT) {
                        if (child->value.i) {
                            b = FALSE;
                            break;
                        }
                    } else {
                        krcalc->calc_status = -1;
                        snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                                "op[%d] type [%c]  not valided!", 
                                t->op, child->type);
                        return -1;
                    }
                }
                break;
            }
        default:
        {
            krcalc->calc_status = -1;
            snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                    "unsupport op [%d]!", t->op);
            return -1;
        }
    }

    /*set result value*/
    t->value.b = b;
    t->ind = KR_VALUE_SETED;

    return 0;
}

/*do the arithmetic evaluation*/
static int _kr_calc_tree_eval_arith(T_KRCalcTree *t, T_KRCalc *krcalc)
{
    t->ind = KR_VALUE_UNSET;

    T_KRCalcTree *left = t->children[0];
    T_KRCalcTree *right = t->children[1];
    if (left->ind != KR_VALUE_SETED || right->ind != KR_VALUE_SETED) {
        return -1;
    }

    
    /*promote left value to double*/
    double d0=0.0;
    switch(left->type) 
    {
        case KR_TYPE_INT: 
            d0 = (double )left->value.i; 
            break;
        case KR_TYPE_LONG: 
            d0 = (double )left->value.l; 
            break;
        case KR_TYPE_DOUBLE: 
            d0 = (double )left->value.d; 
            break;
        default: 
            krcalc->calc_status = -1;
            snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                    "left type should be numeric [%c]!", left->type);
            return -1;       
    }
    
    /*promote right value to double*/
    double d1=0.0;
    switch(right->type) 
    {
        case KR_TYPE_INT: 
            d1 = (double )right->value.i; 
            break;
        case KR_TYPE_LONG: 
            d1 = (double )right->value.l; 
            break;
        case KR_TYPE_DOUBLE: 
            d1 = (double )right->value.d; 
            break;
        default: 
            krcalc->calc_status = -1;
            snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                    "right type should be numeric [%c]!", right->type);
            return -1;       
    }
    
    /*do the arithmetic operation*/
    double d=0.0;
    switch(t->op) 
    {
        case KR_CALCOP_PLUS: d = d0 + d1; break;
        case KR_CALCOP_SUB:  d = d0 - d1; break;
        case KR_CALCOP_MUT:  d = d0 * d1; break;
        case KR_CALCOP_DIV:  d = d0 / d1; break;
        case KR_CALCOP_MOD:  d = (int )d0 % (int )d1; break;
        default: 
        {
            krcalc->calc_status = -1;
            snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                    "unsupport op [%d]!", t->op);
            return -1;      
        }
    }
    
    /*set result value*/
    switch(t->type) 
    {
        case KR_TYPE_INT: 
            t->value.i = (kr_int ) d; 
            break;
        case KR_TYPE_LONG: 
            t->value.l = (kr_long ) d; 
            break;
        case KR_TYPE_DOUBLE: 
            t->value.d = (kr_double ) d; 
            break;
        default: 
        {
            krcalc->calc_status = -1;
            snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                    "unsupport type [%c]!", t->type);
            return -1;
        }
    }
    t->ind = KR_VALUE_SETED;

    return 0;
}


/*do the logical evaluation*/
static int _kr_calc_tree_eval_logic(T_KRCalcTree *t, T_KRCalc *krcalc)
{
    t->ind = KR_VALUE_UNSET;

    T_KRCalcTree *left = t->children[0];
    T_KRCalcTree *right = t->children[1];
    if (left->ind != KR_VALUE_SETED || right->ind != KR_VALUE_SETED) {
        return -1;
    }

    /*promote left value to double*/
    double d0=0.0;
    if (left) {
        switch(left->type) 
        {
            case KR_TYPE_BOOL:
                d0 = (double )left->value.b; 
                break;
            case KR_TYPE_INT: 
                d0 = (double )left->value.i; 
                break;
            case KR_TYPE_LONG: 
                d0 = (double )left->value.l; 
                break;
            case KR_TYPE_DOUBLE: 
                d0 = (double )left->value.d; 
                break;
        }
    }
    
    /*promote right value to double*/
    double d1=0.0;
    if (right) {
        switch(right->type) 
        {
            case KR_TYPE_BOOL:
                d1 = (double )right->value.b; 
                break;
            case KR_TYPE_INT: 
                d1 = (double )right->value.i; 
                break;
            case KR_TYPE_LONG: 
                d1 = (double )right->value.l; 
                break;
            case KR_TYPE_DOUBLE: 
                d1 = (double )right->value.d; 
                break;
        }
    }

    /*do the logic operation*/
    kr_bool b = FALSE;
    switch(t->op) 
    {
        case KR_CALCOP_LT:
            if (left->type == KR_TYPE_STRING && 
                    right->type == KR_TYPE_STRING) {
                b = (strcmp(left->value.s, right->value.s) < 0);
            } else if (left->type != KR_TYPE_STRING &&
                    right->type != KR_TYPE_STRING) {
                b = (d0 < d1)? TRUE:FALSE;
            } else {
                krcalc->calc_status = -1;
                snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                        "op[%d] left[%c] right[%c]!", \
                        t->op, left->type, right->type);
                return -1;
            }
            break;
        case KR_CALCOP_LE:
            if (left->type == KR_TYPE_STRING && 
                    right->type == KR_TYPE_STRING) {
                b = (strcmp(left->value.s, right->value.s) <= 0);
            } else if (left->type != KR_TYPE_STRING &&
                    right->type != KR_TYPE_STRING) {
                b = (d0 <= d1)? TRUE:FALSE;
            } else {
                krcalc->calc_status = -1;
                snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                        "op[%d] left[%c] right[%c]!", \
                        t->op, left->type, right->type);
                return -1;
            }
            break;
        case KR_CALCOP_GT:
            if (left->type == KR_TYPE_STRING && 
                    right->type == KR_TYPE_STRING) {
                b = (strcmp(left->value.s, 
                            right->value.s) > 0);
            } else if (left->type != KR_TYPE_STRING &&
                    right->type != KR_TYPE_STRING) {
                b = (d0 > d1)? TRUE:FALSE;
            } else {
                krcalc->calc_status = -1;
                snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                        "op[%d] left[%c] right[%c]!", \
                        t->op, left->type, right->type);
                return -1;
            }
            break;
        case KR_CALCOP_GE:
            if (left->type == KR_TYPE_STRING && 
                    right->type == KR_TYPE_STRING) {
                b = (strcmp(left->value.s, 
                            right->value.s) >= 0);
            } else if (left->type != KR_TYPE_STRING &&
                    right->type != KR_TYPE_STRING) {
                b = (d0 >= d1)? TRUE:FALSE;
            } else {
                krcalc->calc_status = -1;
                snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                        "op[%d] left[%c] right[%c]!", \
                        t->op, left->type, right->type);
                return -1;
            }
            break;
        case KR_CALCOP_EQ: 
            if (left->type == KR_TYPE_STRING && 
                    right->type == KR_TYPE_STRING) {
                b = (strcmp(left->value.s, 
                            right->value.s) == 0);
            } else if (left->type != KR_TYPE_STRING &&
                    right->type != KR_TYPE_STRING) {
                b = (d0 == d1)? TRUE:FALSE;
            } else {
                krcalc->calc_status = -1;
                snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                        "op[%d] left[%c] right[%c]!", \
                        t->op, left->type, right->type);
                return -1;
            }
            break;
        case KR_CALCOP_NEQ:
            if (left->type == KR_TYPE_STRING && 
                    right->type == KR_TYPE_STRING) {
                b = strcmp(left->value.s, right->value.s);
            } else if (left->type != KR_TYPE_STRING &&
                    right->type != KR_TYPE_STRING) {
                b = (d0 != d1)? TRUE:FALSE;
            } else {
                krcalc->calc_status = -1;
                snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                        "op[%d] left[%c] right[%c]!", \
                        t->op, left->type, right->type);
                return -1;
            }
            break;
        case KR_CALCOP_BL:
            if (right->kind == KR_CALCKIND_MINT || 
                    right->kind == KR_CALCKIND_MFLOAT || 
                    right->kind == KR_CALCKIND_MSTRING || 
                    right->kind == KR_CALCKIND_SET) {
                /*the right value should store with T_KRHashSet pointer*/
                T_KRHashSet *set = (T_KRHashSet *)right->value.p;
                if (left->type == KR_TYPE_STRING &&
                        set->type == KR_TYPE_STRING) {
                    b = kr_hashset_search(set, left->value.s);
                } else if(left->type == set->type) {
                    b = kr_hashset_search(set, &left->value);
                } else {
                    krcalc->calc_status = -1;
                    snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                            "BL left[%c] right[%c]!", \
                            left->type, set->type);
                    return -1;
                }
            } else {
                krcalc->calc_status = -1;
                snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                        "BL right kind not valid[%c]!", right->kind);
                return -1;
            }
            break;
        case KR_CALCOP_NBL:
            if (right->kind == KR_CALCKIND_MINT || 
                    right->kind == KR_CALCKIND_MFLOAT || 
                    right->kind == KR_CALCKIND_MSTRING || 
                    right->kind == KR_CALCKIND_SET) {
                /*the right value should store with T_KRHashSet pointer*/
                T_KRHashSet *set = (T_KRHashSet *)right->value.p;
                if (left->type == KR_TYPE_STRING &&
                        set->type == KR_TYPE_STRING) {
                    b = !kr_hashset_search(set, left->value.s);
                } else if(left->type == right->type) {
                    b = !kr_hashset_search(set, &left->value);
                } else {
                    krcalc->calc_status = -1;
                    snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                            "NBL left[%c] right [%c]!", \
                            left->type, set->type);
                    return -1;
                }
            } else {
                krcalc->calc_status = -1;
                snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                        "NBL right kind not valid[%c]!", right->kind);
                return -1;
            }
            break;    
        case KR_CALCOP_MATCH:
            if (left->type == KR_TYPE_STRING &&
                    right->kind == KR_CALCKIND_REGEX) {
                /*the right value should store with T_KRRegex pointer*/
                T_KRRegex *regex = (T_KRRegex *)right->value.p;
                b = kr_regex_execute(regex, left->value.s);
            } else {
                krcalc->calc_status = -1;
                snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                        "REGEX left[%c] right kind[%c]!", \
                        left->type, right->kind);
                return -1;
            }
            break;
        default:
            krcalc->calc_status = -1;
            snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                    "unsupport op [%d]!", t->op);
            return -1;
    }
        
    /*set result*/
    t->ind = KR_VALUE_SETED;
    t->value.b = b;
    
    return 0;
}

/*do the extern identifier evaluation*/
static int _kr_calc_tree_eval_extern(T_KRCalcTree *t, T_KRCalc *krcalc)
{
    t->ind = KR_VALUE_UNSET;

    /*type only set once for optimization*/
    if (t->type == KR_TYPE_UNKNOWN) {
        t->type = krcalc->get_type_cb(t->kind, t->id, krcalc->calc_param);
        if (t->type == KR_TYPE_UNKNOWN) {
            krcalc->calc_status = -1;
            snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                    "kind[%d],id[%d] type unknown", \
                    t->kind, t->id);
            return -1;
        }
    }
    
    void *val = krcalc->get_value_cb(t->kind, t->id, krcalc->calc_param);
    KR_LOG(KR_LOGDEBUG, "get extern value kind[%d] id[%d]", t->kind, t->id);
    if (val != NULL) {
        if (t->kind == KR_CALCKIND_SET) {
            t->type = KR_TYPE_POINTER;
            t->value.p = val;
            t->ind = KR_VALUE_SETED;
            KR_LOG(KR_LOGDEBUG, "set[%d]", t->id);
            return 0;
        }

        switch(t->type) 
        {
            case KR_TYPE_BOOL:
                t->value.b = *(kr_bool *)val;
                KR_LOG(KR_LOGDEBUG, "type[%c] value[%d]", \
                        t->type, t->value.b);
                break;
            case KR_TYPE_INT:
                t->value.i = *(kr_int *)val;
                KR_LOG(KR_LOGDEBUG, "type[%c] value[%d]", \
                        t->type, t->value.i);
                break;
            case KR_TYPE_LONG:
                t->value.l = *(kr_long *)val;
                KR_LOG(KR_LOGDEBUG, "type[%c] value[%d]", \
                        t->type, t->value.l);
                break;
            case KR_TYPE_DOUBLE:
                t->value.d = *(kr_double *)val;
                KR_LOG(KR_LOGDEBUG, "type[%c] value[%lf]", \
                        t->type, t->value.d);
                break;
            case KR_TYPE_STRING:
                t->value.s = (kr_string )val;
                KR_LOG(KR_LOGDEBUG, "type[%c] value[%s]", \
                        t->type, t->value.s);
                break;
            case KR_TYPE_POINTER:
                t->value.p = (kr_pointer )val;
                KR_LOG(KR_LOGDEBUG, "type[%c] value[%p]", \
                        t->type, t->value.p);
                break;
            default:
                krcalc->calc_status = -1;
                snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                        "unsupported type[%c]", t->type);
                return -1;
        }
        t->ind = KR_VALUE_SETED;
    }
    return 0;
}

/* evaluate an calculator tree node
 */
static int _kr_calc_tree_eval(T_KRCalcTree *t, T_KRCalc *krcalc)
{
    switch (t->kind) { 
        case KR_CALCKIND_REL: 
            return _kr_calc_tree_eval_rel(t, krcalc);
        case KR_CALCKIND_ARITH: 
            return _kr_calc_tree_eval_arith(t, krcalc);
        case KR_CALCKIND_LOGIC: 
            return _kr_calc_tree_eval_logic(t, krcalc);
        case KR_CALCKIND_SET:
        case KR_CALCKIND_CID:
        case KR_CALCKIND_FID:
        case KR_CALCKIND_SID:
        case KR_CALCKIND_DID:
        case KR_CALCKIND_HID:
            return _kr_calc_tree_eval_extern(t, krcalc);
    }

    return 0;
}

int kr_calc_tree_eval(T_KRCalcTree *root, T_KRCalc *krcalc)
{
    return kr_calc_tree_traverse(root, krcalc, 
            NULL, (traverse_func )_kr_calc_tree_eval);
}


/* free this calctree node and free his allocated subvalue */
static int _kr_calc_tree_free(T_KRCalcTree *t, void *data)
{ 
    if (t != NULL) {
        switch (t->kind) {
            case KR_CALCKIND_STRING:
                kr_free(t->value.s); 
                break;
            case KR_CALCKIND_MINT:
            case KR_CALCKIND_MFLOAT:
            case KR_CALCKIND_MSTRING:
            case KR_CALCKIND_SET:
                kr_hashset_destroy(t->value.p); 
                break;
            case KR_CALCKIND_REGEX:
                kr_regex_free(t->value.p); 
                break;
        }
        kr_free(t); t = NULL;
    }
    
    return 0;
}

/* free calctree */
void kr_calc_tree_free(T_KRCalcTree *root)
{
    kr_calc_tree_traverse(root, NULL, 
            NULL, (traverse_func )_kr_calc_tree_free);
}


