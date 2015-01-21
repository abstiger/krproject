#include "kr_calc.h"
#include "kr_calc_tree.h"
#include "kr_calc_parser_json.h"
#include "krutils/kr_json.h"
#include <assert.h>


static T_KRCalcTree *_kr_calc_parse_json(T_KRCalc *krcalc, cJSON *krjson);
static T_KRCalcTree *kr_calc_parse_exp(T_KRCalc *krcalc, int op, cJSON *json);
static T_KRCalcTree *kr_calc_parse_ele(T_KRCalc *krcalc, int kind, cJSON *json);

static inline int kr_calcjson_getint(cJSON *json) {
    return (int )json->valuedouble;
}

static inline double kr_calcjson_getdouble(cJSON *json) {
    return json->valuedouble;
}

static inline char *kr_calcjson_getstring(cJSON *json) {
    return json->valuestring;
}


T_KRCalcTree *kr_calc_parse_ele(T_KRCalc *krcalc, int kind, cJSON *json)
{
    assert(json);
    T_KRCalcTree *tree = NULL;
    switch(kind) 
    {
        case KR_CALCKIND_INT:
            tree = kr_calc_tree_new(kind);
            tree->type = KR_TYPE_INT;
            tree->value.i = kr_calcjson_getint(json);
            tree->ind = KR_VALUE_SETED;
            break;
        case KR_CALCKIND_FLOAT:
            tree = kr_calc_tree_new(kind);
            tree->type = KR_TYPE_DOUBLE;
            tree->value.d = kr_calcjson_getdouble(json);
            tree->ind = KR_VALUE_SETED;
            break;
        case KR_CALCKIND_STRING:
            tree = kr_calc_tree_new(kind);
            tree->type = KR_TYPE_STRING;
            tree->value.s = kr_strdup(kr_calcjson_getstring(json));
            tree->ind = KR_VALUE_SETED;
            break;
        case KR_CALCKIND_CID:
            tree = kr_calc_tree_new(kind);
            tree->id = kr_calcjson_getint(json);
            tree->ind = KR_VALUE_UNSET;
            break;
        case KR_CALCKIND_FID:
            tree = kr_calc_tree_new(kind);
            tree->id = kr_calcjson_getint(json);
            tree->ind = KR_VALUE_UNSET;
            break;
        case KR_CALCKIND_SID:
            tree = kr_calc_tree_new(kind);
            tree->id = kr_calcjson_getint(json);
            tree->ind = KR_VALUE_UNSET;
            break;
        case KR_CALCKIND_DID:
            tree = kr_calc_tree_new(kind);
            tree->id = kr_calcjson_getint(json);
            tree->ind = KR_VALUE_UNSET;
            break;
        case KR_CALCKIND_HID:
            tree = kr_calc_tree_new(kind);
            tree->id = kr_calcjson_getint(json);
            tree->ind = KR_VALUE_UNSET;
            break;
        case KR_CALCKIND_SET:
            tree = kr_calc_tree_new(kind);
            tree->id = kr_calcjson_getint(json);
            tree->ind = KR_VALUE_UNSET;
            break;
        case KR_CALCKIND_MINT:
            tree = kr_calc_tree_new(kind);
            tree->type = KR_TYPE_POINTER;
            tree->value.p = \
                kr_hashset_make(KR_TYPE_INT, kr_calcjson_getstring(json));
            tree->ind = KR_VALUE_SETED;
            break;
        case KR_CALCKIND_MFLOAT:
            tree = kr_calc_tree_new(kind);
            tree->type = KR_TYPE_POINTER;
            tree->value.p = \
                kr_hashset_make(KR_TYPE_DOUBLE, kr_calcjson_getstring(json));
            tree->ind = KR_VALUE_SETED;
            break;
        case KR_CALCKIND_MSTRING:
            tree = kr_calc_tree_new(kind);
            tree->type = KR_TYPE_POINTER;
            tree->value.p = \
                kr_hashset_make(KR_TYPE_STRING, kr_calcjson_getstring(json));
            tree->ind = KR_VALUE_SETED;
            break;
        case KR_CALCKIND_REGEX:
            tree = kr_calc_tree_new(kind);
            tree->type = KR_TYPE_POINTER;
            tree->value.p = kr_regex_compile(kr_calcjson_getstring(json));
            tree->ind = KR_VALUE_SETED;
            break;
        default:
            sprintf(krcalc->calc_errmsg, "unknown kind [%d]!\n", kind);
            return NULL;
            break;
    }

    return tree;
}


T_KRCalcTree *kr_calc_parse_exp(T_KRCalc *krcalc, int op, cJSON *json)
{
    assert(json);
    T_KRCalcTree *tree = NULL;
    /* get operation code */
    switch(op)
    {
        case KR_CALCOP_PLUS:
        case KR_CALCOP_SUB:
        case KR_CALCOP_MUT:
        case KR_CALCOP_DIV:
        case KR_CALCOP_MOD:
        {
            tree = kr_calc_tree_new(KR_CALCKIND_ARITH);
            tree->op = op;
            tree->type = KR_TYPE_DOUBLE;
            tree->ind = KR_VALUE_UNSET;
            break;
        }
        case KR_CALCOP_LT:
        case KR_CALCOP_LE:
        case KR_CALCOP_GT:
        case KR_CALCOP_GE:
        case KR_CALCOP_EQ:
        case KR_CALCOP_NEQ:
        case KR_CALCOP_BL:
        case KR_CALCOP_NBL:
        case KR_CALCOP_MATCH:
        {
            tree = kr_calc_tree_new(KR_CALCKIND_LOGIC);
            tree->op = op;
            tree->type = KR_TYPE_BOOL;
            tree->ind = KR_VALUE_UNSET;
            break;
        }
        case KR_CALCOP_NOT:
        case KR_CALCOP_AND:
        case KR_CALCOP_OR:
        {
            tree = kr_calc_tree_new(KR_CALCKIND_REL);
            tree->op = op;
            tree->type = KR_TYPE_BOOL;
            tree->ind = KR_VALUE_UNSET;
            break;
        }
        default:
        {
            krcalc->calc_status = -1;
            sprintf(krcalc->calc_errmsg, "unknown op [%d]!\n", op);
            return NULL;
        }
    }

    cJSON *children = cJSON_GetObjectItem(json, "child");
    for (int i=0; i<cJSON_GetArraySize(children); ++i) {
        cJSON *child = cJSON_GetArrayItem(children, i);
        T_KRCalcTree *node = _kr_calc_parse_json(krcalc, child);
        if (node == NULL) {
            krcalc->calc_status = -1;
            sprintf(krcalc->calc_errmsg, "_kr_calc_parse_json child%d failed!", i);
            return NULL;
        }
        kr_calc_tree_append(tree, node);
    }

    return tree;
}


static T_KRCalcTree *_kr_calc_parse_json(T_KRCalc *krcalc, cJSON *krjson)
{
    assert(krjson);
    cJSON *json = NULL;

    /* an expression */
    json = cJSON_GetObjectItem(krjson, "op");
    if (json) {
        int op = kr_calcjson_getint(json);
        return kr_calc_parse_exp(krcalc, op, krjson);
    } 

    /* an element */
    json = cJSON_GetObjectItem(krjson, "kind");
    if (json) {
        int kind = kr_calcjson_getint(json);
        cJSON *value = cJSON_GetObjectItem(krjson, "value");
        return kr_calc_parse_ele(krcalc, kind, value);
    }

    return NULL;
}


int kr_calc_parse_json(T_KRCalc *krcalc)
{
    cJSON *krjson = cJSON_Parse(krcalc->calc_string);
    if (krjson == NULL) {
        krcalc->calc_status = -1;
        sprintf(krcalc->calc_errmsg, "cJSON_Parse %s failed", krcalc->calc_string);
        return -1;
    }

    krcalc->calc_tree = _kr_calc_parse_json(krcalc, krjson);
    if (krcalc->calc_tree == NULL) {
        krcalc->calc_status = -1;
        sprintf(krcalc->calc_errmsg, "_kr_calc_parse_json failed");
        cJSON_Delete(krjson);
        return -1;
    }

    cJSON_Delete(krjson);
    return 0;
}


