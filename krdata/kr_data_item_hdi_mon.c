#include "kr_data_item_hdi.h"
#include "krparam/kr_param_class_hdi.h"

/*
#include "dbs/dbs_basopr.h"
#include "dbs/dbs/hdi_mon_sel.h"

int kr_hdi_aggr_mon(T_KRHdi *ptHdi, T_KRContext *ptContext, char *object)
{
    int iResult = 0;
    double dValue = 0.0;
    char caDataDate[8+1] = {0};
    T_DbsEnv *ptDbsEnv = ptData->ptDbsEnv;
    
    T_HdiMonSel stHdiMonSel = {0};
    strcpy(stHdiMonSel.caInDataObject, object);
    stHdiMonSel.lInDataId = ptHdi->lHdiId;
    time_t tEndTime = kr_record_get_transtime(ptData->ptCurrRec);
    time_t tBeginTime = tEndTime - ptHdi->ptParamHdiDef->lStatisticsValue;
    kr_ttime_to_date(tBeginTime, caDataDate);
    memcpy(stHdiMonSel.caInDataMonthBegin, caDataDate, 6);
    kr_ttime_to_date(tEndTime, caDataDate);
    memcpy(stHdiMonSel.caInDataMonthEnd, caDataDate, 6);
    iResult = dbsHdiMonSel(ptDbsEnv, KR_DBSELECT, &stHdiMonSel);
    if (iResult != KR_DBOK && iResult != KR_DBNOTFOUND) {
        KR_LOG(KR_LOGERROR, "dbsHdiMonSel [%s],[%ld],[%s],[%s] Error![%s]:[%s]",
                stHdiMonSel.caInDataObject, stHdiMonSel.lInDataId,
                stHdiMonSel.caInDataMonthBegin, stHdiMonSel.caInDataMonthEnd,
                ptDbsEnv->sqlstate, ptDbsEnv->sqlerrmsg);
        return -1;
    }
    
    //TODO:sqlite3 let sum,cnt->char(255)
    switch(ptParamHdi->caStatisticsMethod[0])
    {
        case KR_Hdi_METHOD_SUM:
            dValue = stHdiMonSel.dOutSumDataValue;
            break;
        case KR_Hdi_METHOD_MIN:
            dValue = stHdiMonSel.dOutMinDataValue;
            break;
        case KR_Hdi_METHOD_MAX:
            dValue = stHdiMonSel.dOutMaxDataValue;
            break;
        case KR_Hdi_METHOD_COUNT:
            dValue = stHdiMonSel.lOutCountDataValue;
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported statistics_method [%s]!",\
                ptParamHdi->caStatisticsMethod);
            return -1;
    }
    
    switch(ptHdi->eValueType)
    {
        case KR_TYPE_INT:
            ptHdi->uValue.i = (int )dValue;
            break;
        case KR_TYPE_LONG:
            ptHdi->uValue.l = (long )dValue;
            break;
        case KR_TYPE_DOUBLE:
            ptHdi->uValue.d = (double )dValue;
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported ValueType [%s],[%c]!",\
                ptHdi->ptParamHdiDef->caStatisticsType, ptHdi->eValueType);
            return -1;
    }
    
    return 0;
}
*/

int kr_hdi_aggr_mon(T_KRHdi *ptHdi, T_KRContext *ptContext, char *object)
{
    //FIXME:dummy implemention
    return 0;
}
