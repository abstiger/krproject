#include "krdata/kr_data.h"

extern int kr_group_func(T_KRGroup *krgroup, T_KRContext *krcontext);

int kr_group_route(T_KRGroup *krgroup, void *krcontext)
{
    /*calculate group string*/
    if (kr_calc_eval(krgroup->ptGroupCalc, krcontext) != 0) {
        KR_LOG(KR_LOGERROR, "kr_calc_eval group [%ld] failed!", 
                krgroup->lGroupId);
        return -1;
    }
    
    /*handle calculator result:matched*/
    if (krgroup->ptGroupCalc->result_ind == KR_VALUE_SETED &&
            krgroup->ptGroupCalc->result_type == KR_TYPE_BOOL &&
            krgroup->ptGroupCalc->result_value.b != 0) {
        return 1;
    }

    return 0;
}


int kr_group_list_route(T_KRContext *krcontext)
{
    int ret = 0;
    /* detecting context check*/
    if (krcontext == NULL || krcontext->ptDym->grouplist == NULL ||
        krcontext->ptCurrRec == NULL) {
        KR_LOG(KR_LOGERROR, "detecting context not set correctly!");
        return -1;
    }

    /* if no groups to be detected, return asap */
    T_KRGroupList *ptGroupList = krcontext->ptDym->grouplist;
    if (ptGroupList->lGroupCnt == 0) {
        KR_LOG(KR_LOGDEBUG, "no groups to be detected!");
        return 0;
    }
    
    /*traversal the group list*/
    T_KRGroup *krgroup = NULL;
    T_KRListNode *node = ptGroupList->ptGroupList->head;
    while(node)
    {
        krgroup = kr_list_value(node);
        ret = kr_group_route(krgroup, krcontext); 
        if (ret == 1) {
            krcontext->ptDym->rulelist = krgroup->ptRuleList;
            KR_LOG(KR_LOGDEBUG, "kr_group_route [%ld]!", krgroup->lGroupId);
            break;
        } else if (ret != 0) {
            KR_LOG(KR_LOGERROR, "kr_group_route failed!");
            return -1;
        }
        node = node->next;
    }

    /* if no group routed, return asap */
    if (krcontext->ptDym->rulelist == NULL) {
        KR_LOG(KR_LOGDEBUG, "no group routed!");
        return 0;
    }

    /*run rule list detect*/
    if (kr_rule_list_detect(krcontext->ptDym->rulelist, krcontext) != 0) {
        KR_LOG(KR_LOGERROR, "kr_rule_list_detect failed!\n");
        return -1;
    }

    /*run the group func*/
    krgroup->GroupFunc = krgroup->GroupFunc?krgroup->GroupFunc:kr_group_func;
    if (krgroup->GroupFunc(krgroup, krcontext) != 0) {
        KR_LOG(KR_LOGERROR, "run Group[%ld] Func[%s] failed!", 
                krgroup->lGroupId, krgroup->ptShmGroupDef->caGroupFunc);
        return -1;
    }
    
    return 0;
}

