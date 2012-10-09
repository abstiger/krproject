#include "kr_aid.h"
#include "krutils/kr_utils.h"
#include "krcalc/kr_calc.h"

#include "dbs/dbs_basopr.h"
#include "dbs/set_def_sel.h"
#include "dbs/set_cfg_cur.h"

static int kr_aid_load(T_KRSet *krset, int set_id)
{
    int iCnt = 0;    
    int iResult = 0;
    int nRet = 0;
    T_SetCfgCur stSetCfgCur = {0};
    U_KRFieldVal uEleVal ={0};
    
    stSetCfgCur.lInSetId = set_id;
    iResult = dbsSetCfgCur(KR_DBCUROPEN, &stSetCfgCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSetCfgCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsSetCfgCur(KR_DBCURFETCH, &stSetCfgCur);
        if (iResult != KR_DBNOTFOUND && iResult != KR_DBOK)
        {
            KR_LOG(KR_LOGERROR, "dbsSetCfgCur Fetch Error!");
            nRet = -1;
            break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGERROR, "Load DataSet[%d],Elements[%d] Totally!", \
                    stSetCfgCur.lInSetId, iCnt);
            break;
        }
        
        kr_string_rtrim(stSetCfgCur.caOutElementValue);
        memset(&uEleVal, 0x00, sizeof(uEleVal));
        switch(krset->type)
        {
            case KR_FIELDTYPE_INT: 
                uEleVal.i = atoi(stSetCfgCur.caOutElementValue);
                nRet = kr_set_add(krset, &uEleVal.i);
                break;
            case KR_FIELDTYPE_LONG: 
                uEleVal.l = atol(stSetCfgCur.caOutElementValue);
                nRet = kr_set_add(krset, &uEleVal.l);
                break;
            case KR_FIELDTYPE_DOUBLE: 
                uEleVal.d = atof(stSetCfgCur.caOutElementValue);
                nRet = kr_set_add(krset, &uEleVal.d);
                break;
            case KR_FIELDTYPE_STRING: 
                uEleVal.s = stSetCfgCur.caOutElementValue;
                nRet = kr_set_add(krset, &uEleVal.s);
                break;
            default:
                KR_LOG(KR_LOGERROR, "Unknown set value type[%c]!", krset->type);
                nRet = -1;
                break;
        }
        
        if (nRet != 0) {
            KR_LOG(KR_LOGERROR, "kr_set_add [%s] Error!", \
                   stSetCfgCur.caOutElementValue);
            break;
        }

        iCnt++;
    }

    iResult = dbsSetCfgCur(KR_DBCURCLOSE, &stSetCfgCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSetCfgCur Close Error!");
        return -1;
    }
    
    return nRet;
}


T_KRSet *kr_aid_get_value(int set_id)
{
    int iResult = 0;
    T_SetDefSel stSetDefSel = {0};
    
    stSetDefSel.lInSetId = set_id;
    iResult = dbsSetDefSel(KR_DBSELECT, &stSetDefSel);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSetDefSel [%ld] Failed!", \
                stSetDefSel.lInSetId);
        return NULL;
    }
    
    T_KRSet *krset = kr_set_create(kr_string_rtrim(stSetDefSel.caOutSetName), \
                                 (E_KRFieldType )stSetDefSel.caOutElementType[0]);
    if (krset == NULL) {
        KR_LOG(KR_LOGERROR, "kr_set_create [%ld] Failed!", \
                stSetDefSel.lInSetId);
        return NULL;
    }
    
    iResult = kr_aid_load(krset, stSetDefSel.lInSetId);
    if (iResult != 0) {
        KR_LOG(KR_LOGERROR, "kr_aid_load [%ld] Failed!", \
                stSetDefSel.lInSetId);
        kr_set_destroy(krset);
        return NULL;
    }
    
    return krset;
}

E_KRFieldType kr_aid_get_type(int set_id)
{
    int iResult = 0;
    T_SetDefSel stSetDefSel = {0};
    
    stSetDefSel.lInSetId = set_id;
    iResult = dbsSetDefSel(KR_DBSELECT, &stSetDefSel);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsSetDefSel [%ld] Failed!", \
                stSetDefSel.lInSetId);
        return KR_FIELDTYPE_UNKNOWN;
    }
    
    return (E_KRFieldType )stSetDefSel.caOutElementType[0];
}
