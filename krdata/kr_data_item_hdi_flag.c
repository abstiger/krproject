#include "kr_data_item_hdi.h"
#include "krparam/kr_param_class_hdi.h"

/*
#include "dbs/dbs_basopr.h"
#include "dbs/dbs/hdi_flag_sel.h"

int kr_hdi_aggr_flag(T_KRHdi *ptHdi, T_KRContext *ptContext, char *object)
{
    int iResult = 0;
    char caDataDate[8+1] = {0};
    T_DbsEnv *ptDbsEnv = ptData->ptDbsEnv;
    
    T_HdiFlagSel stHdiFlagSel = {0};
    strcpy(stHdiFlagSel.caInDataObject, object);
    stHdiFlagSel.lInDataId = ptHdi->lHdiId;
    iResult = dbsHdiFlagSel(ptDbsEnv, KR_DBSELECT, &stHdiFlagSel);
    if (iResult != KR_DBOK && iResult != KR_DBNOTFOUND) {
        KR_LOG(KR_LOGERROR, "dbsHdiFlagSel [%s],[%ld] Error![%s]:[%s]",\
               stHdiFlagSel.caInDataObject, stHdiFlagSel.lInDataId,
               ptDbsEnv->sqlstate, ptDbsEnv->sqlerrmsg);
        return -1;
    }
        
    switch(ptHdi->eValueType)
    {
        case KR_TYPE_INT:
            ptHdi->uValue.i = atoi(stHdiFlagSel.caOutDataFlag);
            break;
        case KR_TYPE_LONG:
            ptHdi->uValue.l = atol(stHdiFlagSel.caOutDataFlag);
            break;
        case KR_TYPE_DOUBLE:
            ptHdi->uValue.d = atof(stHdiFlagSel.caOutDataFlag);
            break;
        case KR_TYPE_STRING:
            ptHdi->uValue.s = kr_strdup(stHdiFlagSel.caOutDataFlag);
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported ValueType [%s],[%c]!",\
                ptHdi->ptParamHdiDef->caStatisticsType, ptHdi->eValueType);
            return -1;
    }
    
    return 0;
}

*/

int kr_hdi_aggr_flag(T_KRHdi *ptHdi, T_KRContext *ptContext, char *object)
{
    //FIXME:dummy implemention
    return 0;
}
