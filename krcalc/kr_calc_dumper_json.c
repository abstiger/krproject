#include "kr_calc.h"
#include "kr_calc_tree.h"
#include "kr_calc_dumper_json.h"
#include "krutils/kr_json.h"


/* dump calctree with json format */
static cJSON *_kr_calc_dump_json(T_KRCalcTree *t)
{
    cJSON *krjson = cJSON_CreateObject();
    switch(t->kind) 
    {
        case KR_CALCKIND_REL:
        case KR_CALCKIND_ARITH:
        case KR_CALCKIND_LOGIC:
        {
            cJSON_AddNumberToObject(krjson, "op", t->op);
            cJSON *childs = cJSON_CreateArray();
            for (int i; i<t->childnum; ++i) {
                cJSON_AddItemToArray(childs, \
                        _kr_calc_dump_json(t->children[i]));
            }
            cJSON_AddItemToObject(krjson, "child", childs);
            break;
        }
        case KR_CALCKIND_INT:
        {
            cJSON_AddNumberToObject(krjson, "kind", t->kind);
            cJSON_AddNumberToObject(krjson, "value", t->value.i);
            break;
        }
        case KR_CALCKIND_FLOAT:
        {
            cJSON_AddNumberToObject(krjson, "kind", t->kind);
            cJSON_AddNumberToObject(krjson, "value", t->value.d);
            break;
        }
        case KR_CALCKIND_STRING:
        {
            cJSON_AddNumberToObject(krjson, "kind", t->kind);
            cJSON_AddStringToObject(krjson, "value", t->value.s);
            break;
        }
        case KR_CALCKIND_MINT:
        case KR_CALCKIND_MFLOAT:
        case KR_CALCKIND_MSTRING:
        {
            cJSON_AddNumberToObject(krjson, "kind", t->kind);
            T_KRHashSet *hashset = (T_KRHashSet *)(t->value.p);
            cJSON_AddStringToObject(krjson, "value", hashset->name);
            break;
        }
        case KR_CALCKIND_REGEX:
        {
            cJSON_AddNumberToObject(krjson, "kind", t->kind);
            T_KRRegex *regex = (T_KRRegex *)(t->value.p);
            cJSON_AddStringToObject(krjson, "value", regex->pattern);
            break;
        }
        case KR_CALCKIND_SET:
        case KR_CALCKIND_CID:
        case KR_CALCKIND_FID:
        case KR_CALCKIND_SID:
        case KR_CALCKIND_DID:
        case KR_CALCKIND_HID:
        {
            cJSON_AddNumberToObject(krjson, "kind", t->kind);
            cJSON_AddNumberToObject(krjson, "value", t->id);
            break;
        }
        default:
        {
            return NULL;
        }
    }   

    return krjson;
}


char *kr_calc_dump_json(T_KRCalc *krcalc)
{
    cJSON *json = _kr_calc_dump_json(krcalc->calc_tree);
    if (json == NULL) {
        krcalc->calc_status = -1;
        snprintf(krcalc->calc_errmsg, sizeof(krcalc->calc_errmsg),
                "call _kr_calc_dump_json failed!");
        return NULL;
    }

    char *dump_str = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);

    return dump_str;
}
