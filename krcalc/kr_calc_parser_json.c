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
            tree = kr_calctree_node_new(kind);
            tree->type = KR_CALCTYPE_INTEGER;
            tree->attr.val.i = kr_calcjson_getint(json);
            tree->ind = KR_VALUE_SETED;
            break;
        case KR_CALCKIND_FLOAT:
            tree = kr_calctree_node_new(kind);
            tree->type = KR_CALCTYPE_DOUBLE;
            tree->attr.val.d = kr_calcjson_getdouble(json);
            tree->ind = KR_VALUE_SETED;
            break;
        case KR_CALCKIND_STRING:
            tree = kr_calctree_node_new(kind);
            tree->type = KR_CALCTYPE_STRING;
            tree->attr.val.s = kr_strdup(kr_calcjson_getstring(json));
            tree->ind = KR_VALUE_SETED;
            break;
        case KR_CALCKIND_CID:
            krcalc->cid_cnt++;
            tree = kr_calctree_node_new(kind);
            tree->id = kr_calcjson_getint(json);
            tree->ind = KR_VALUE_UNSET;
            break;
        case KR_CALCKIND_FID:
            krcalc->fid_cnt++;
            tree = kr_calctree_node_new(kind);
            tree->id = kr_calcjson_getint(json);
            tree->ind = KR_VALUE_UNSET;
            break;
        case KR_CALCKIND_SID:
            krcalc->sid_cnt++;
            tree = kr_calctree_node_new(kind);
            tree->id = kr_calcjson_getint(json);
            tree->ind = KR_VALUE_UNSET;
            break;
        case KR_CALCKIND_DID:
            krcalc->did_cnt++;
            tree = kr_calctree_node_new(kind);
            tree->id = kr_calcjson_getint(json);
            tree->ind = KR_VALUE_UNSET;
            break;
        case KR_CALCKIND_HID:
            krcalc->hid_cnt++;
            tree = kr_calctree_node_new(kind);
            tree->id = kr_calcjson_getint(json);
            tree->ind = KR_VALUE_UNSET;
            break;
        case KR_CALCKIND_SET:
            krcalc->set_cnt++;
            tree = kr_calctree_node_new(kind);
            tree->id = kr_calcjson_getint(json);
            tree->type = 
                krcalc->get_type_cb(kind, tree->id, krcalc->data);
            tree->attr.set = 
                krcalc->get_value_cb(kind, tree->id, krcalc->data);
            tree->ind = KR_VALUE_SETED;
            break;
        case KR_CALCKIND_MINT:
            krcalc->multi_cnt++;
            tree = kr_calctree_node_new(kind);
            tree->type = KR_CALCTYPE_INTEGER;
            tree->attr.set = 
                kr_make_set_from_multi(kind, kr_calcjson_getstring(json));
            tree->ind = KR_VALUE_SETED;
            break;
        case KR_CALCKIND_MFLOAT:
            krcalc->multi_cnt++;
            tree = kr_calctree_node_new(kind);
            tree->type = KR_CALCTYPE_DOUBLE;
            tree->attr.set = 
                kr_make_set_from_multi(kind, kr_calcjson_getstring(json));
            tree->ind = KR_VALUE_SETED;
            break;
        case KR_CALCKIND_MSTRING:
            krcalc->multi_cnt++;
            tree = kr_calctree_node_new(kind);
            tree->type = KR_CALCTYPE_STRING;
            tree->attr.set = 
                kr_make_set_from_multi(kind, kr_calcjson_getstring(json));
            tree->ind = KR_VALUE_SETED;
            break;
        case KR_CALCKIND_REGEX:
            krcalc->regex_cnt++;
            tree = kr_calctree_node_new(kind);
            tree->type = KR_CALCTYPE_STRING;
            tree->attr.regex = kr_regex_compile(kr_calcjson_getstring(json));
            tree->ind = KR_VALUE_SETED;
            break;
        default:
            sprintf(krcalc->err_msg, "unknown kind [%d]!\n", kind);
            return NULL;
            break;
    }

    return tree;
}


T_KRCalcTree *kr_calc_parse_exp(T_KRCalc *krcalc, int op, cJSON *json)
{
    assert(json);
    T_KRCalcTree *tree = NULL;
    int childnum=2; /*unary=1, binary=2(default), ternary=3*/
    /* get operation code */
    switch(op)
    {
        case KR_CALCOP_PLUS:
        case KR_CALCOP_SUB:
        case KR_CALCOP_MUT:
        case KR_CALCOP_DIV:
        case KR_CALCOP_MOD:
            tree = kr_calctree_node_new(KR_CALCKIND_ARITH);
            tree->op = op;
            tree->type = KR_CALCTYPE_DOUBLE;
            tree->ind = KR_VALUE_UNSET;
            break;
        case KR_CALCOP_NOT:
            childnum = 1;
        case KR_CALCOP_LT:
        case KR_CALCOP_LE:
        case KR_CALCOP_GT:
        case KR_CALCOP_GE:
        case KR_CALCOP_EQ:
        case KR_CALCOP_NEQ:
        case KR_CALCOP_AND:
        case KR_CALCOP_OR:
        case KR_CALCOP_BL:
        case KR_CALCOP_NBL:
        case KR_CALCOP_MATCH:
            tree = kr_calctree_node_new(KR_CALCKIND_LOGIC);
            tree->op = op;
            tree->type = KR_CALCTYPE_BOOLEAN;
            tree->ind = KR_VALUE_UNSET;
            break;
        default:
            sprintf(krcalc->err_msg, "unknown op [%d]!\n", op);
            return NULL;
            break;
    }

    int i = 0;
    cJSON *children = cJSON_GetObjectItem(json, "child");
    for (i=0; i<childnum; ++i) {
        cJSON *child = cJSON_GetArrayItem(children, i);
        tree->child[i] = _kr_calc_parse_json(krcalc, child);
        if (tree->child[i] == NULL) {
            sprintf(krcalc->err_msg, "_kr_calc_parse_json child%d failed!\n", i);
            return NULL;
        }
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
        sprintf(krcalc->err_msg, "cJSON_Parse %s failed", krcalc->calc_string);
        return -1;
    }

    krcalc->calc_tree = _kr_calc_parse_json(krcalc, krjson);
    if (krcalc->calc_tree == NULL) {
        sprintf(krcalc->err_msg, "_kr_calc_parse_json failed");
        cJSON_Delete(krjson);
        return -1;
    }

    cJSON_Delete(krjson);
    return 0;
}
