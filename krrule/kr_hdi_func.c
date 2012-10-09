#include "kr_hdi.h"
#include "kr_hdi_func.h"

#include "dbs/dbs_basopr.h"
#include "dbs/hdi_day_sel.h"
#include "dbs/hdi_mon_sel.h"
#include "dbs/hdi_flag_sel.h"

int kr_hdi_aggr_func(T_KRHDI *krhdi, T_KRContext *calc_param)
{
    int nRet = 0;
    char caDataObject[KR_HDI_OBJECT_LEN+1];

    switch(calc_param->eKeyType)
    {
        case KR_FIELDTYPE_INT:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%d", *(int *)calc_param->pKeyValue);
            break;         
        case KR_FIELDTYPE_LONG:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%ld", *(long *)calc_param->pKeyValue);
            break; 
        case KR_FIELDTYPE_DOUBLE:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%lf", *(double *)calc_param->pKeyValue);
            break; 
        case KR_FIELDTYPE_STRING:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%s", (char *)calc_param->pKeyValue);
            break; 
        default:
            KR_LOG(KR_LOGERROR, "Unsupported object type[%c]!", \
                   calc_param->eKeyType);
            return -1;
    }

    switch(krhdi->ptShmHDIDef->caStatisticsType[0])
    {
        case KR_HDI_STATISTICS_DAY:
            nRet = kr_hdi_aggr_day(krhdi, calc_param, caDataObject);
            if (nRet != 0) {
                KR_LOG(KR_LOGERROR, "kr_hdi_aggr_day Error!");
            }
            break;
        case KR_HDI_STATISTICS_MONTH:
            nRet = kr_hdi_aggr_mon(krhdi, calc_param, caDataObject);
            if (nRet != 0) {
                KR_LOG(KR_LOGERROR, "kr_hdi_aggr_mon Error!");
            }
            break;
        case KR_HDI_STATISTICS_FLAG:
            nRet = kr_hdi_aggr_flag(krhdi, calc_param, caDataObject);
            if (nRet != 0) {
                KR_LOG(KR_LOGERROR, "kr_hdi_aggr_flag Error!");
            }
            break;
        default:
            KR_LOG(KR_LOGERROR, "Unsupported statistics type[%s]!", \
                   krhdi->ptShmHDIDef->caStatisticsType);
            return -1;        
    }
        
    return 0;
}


int kr_hdi_aggr_day(T_KRHDI *krhdi, T_KRContext *calc_param, char *object)
{
    int iResult = 0;
    double dValue = 0.0;
    
    T_HdiDaySel stHdiDaySel = {0};
    strcpy(stHdiDaySel.caInDataObject, object);
    stHdiDaySel.lInDataId = krhdi->lHDIId;
    time_t tEndTime = kr_get_transtime(calc_param->ptCurrRec);
    time_t tBeginTime = tEndTime - krhdi->ptShmHDIDef->lStatisticsValue;
    kr_ttime_to_date(tBeginTime, stHdiDaySel.caInDataDateBegin);
    kr_ttime_to_date(tEndTime, stHdiDaySel.caInDataDateEnd);
    iResult = dbsHdiDaySel(KR_DBSELECT, &stHdiDaySel);
    if (iResult != KR_DBOK && iResult != KR_DBNOTFOUND)
    {
        KR_LOG(KR_LOGERROR, "dbsHdiDaySel [%s],[%ld],[%s],[%s] Error!",\
                stHdiDaySel.caInDataObject, stHdiDaySel.lInDataId,
                stHdiDaySel.caInDataDateBegin, stHdiDaySel.caInDataDateEnd);
        return -1;
    }
    
    switch(krhdi->ptShmHDIDef->caStatisticsMethod[0])
    {
        case KR_HDI_METHOD_SUM:
            dValue = stHdiDaySel.dOutSumDataValue;
            break;
        case KR_HDI_METHOD_MIN:
            dValue = stHdiDaySel.dOutMinDataValue;
            break;
        case KR_HDI_METHOD_MAX:
            dValue = stHdiDaySel.dOutMaxDataValue;
            break;
        case KR_HDI_METHOD_COUNT:
            dValue = stHdiDaySel.lOutCountDataValue;
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported statistics_method [%s]!",\
                krhdi->ptShmHDIDef->caStatisticsMethod);
            return -1;
    }
    
    switch(krhdi->eValueType)
    {
        case KR_FIELDTYPE_INT:
            krhdi->eValue.i = (int )dValue;
            break;
        case KR_FIELDTYPE_LONG:
            krhdi->eValue.l = (long )dValue;
            break;
        case KR_FIELDTYPE_DOUBLE:
            krhdi->eValue.d = (double )dValue;
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported ValueType [%s],[%c]!",\
                krhdi->ptShmHDIDef->caStatisticsType, krhdi->eValueType);
            return -1;
    }
    
    krhdi->eValueInd = KR_VALUE_SETED;
    return 0;
}

int kr_hdi_aggr_mon(T_KRHDI *krhdi, T_KRContext *calc_param, char *object)
{
    int iResult = 0;
    double dValue = 0.0;
    char caDataDate[8+1] = {0};
    
    T_HdiMonSel stHdiMonSel = {0};
    strcpy(stHdiMonSel.caInDataObject, object);
    stHdiMonSel.lInDataId = krhdi->lHDIId;
    time_t tEndTime = kr_get_transtime(calc_param->ptCurrRec);
    time_t tBeginTime = tEndTime - krhdi->ptShmHDIDef->lStatisticsValue;
    kr_ttime_to_date(tBeginTime, caDataDate);
    memcpy(stHdiMonSel.caInDataMonthBegin, caDataDate, 6);
    kr_ttime_to_date(tEndTime, caDataDate);
    memcpy(stHdiMonSel.caInDataMonthEnd, caDataDate, 6);
    iResult = dbsHdiMonSel(KR_DBSELECT, &stHdiMonSel);
    if (iResult != KR_DBOK && iResult != KR_DBNOTFOUND)
    {
        KR_LOG(KR_LOGERROR, "dbsHdiMonSel [%s],[%ld],[%s],[%s] Error!",\
                stHdiMonSel.caInDataObject, stHdiMonSel.lInDataId,
                stHdiMonSel.caInDataMonthBegin, stHdiMonSel.caInDataMonthEnd);
        return -1;
    }
    
    switch(krhdi->ptShmHDIDef->caStatisticsMethod[0])
    {
        case KR_HDI_METHOD_SUM:
            dValue = stHdiMonSel.dOutSumDataValue;
            break;
        case KR_HDI_METHOD_MIN:
            dValue = stHdiMonSel.dOutMinDataValue;
            break;
        case KR_HDI_METHOD_MAX:
            dValue = stHdiMonSel.dOutMaxDataValue;
            break;
        case KR_HDI_METHOD_COUNT:
            dValue = stHdiMonSel.lOutCountDataValue;
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported statistics_method [%s]!",\
                krhdi->ptShmHDIDef->caStatisticsMethod);
            return -1;
    }
    
    switch(krhdi->eValueType)
    {
        case KR_FIELDTYPE_INT:
            krhdi->eValue.i = (int )dValue;
            break;
        case KR_FIELDTYPE_LONG:
            krhdi->eValue.l = (long )dValue;
            break;
        case KR_FIELDTYPE_DOUBLE:
            krhdi->eValue.d = (double )dValue;
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported ValueType [%s],[%c]!",\
                krhdi->ptShmHDIDef->caStatisticsType, krhdi->eValueType);
            return -1;
    }
    
    krhdi->eValueInd = KR_VALUE_SETED;
    return 0;
}


int kr_hdi_aggr_flag(T_KRHDI *krhdi, T_KRContext *calc_param, char *object)
{
    int iResult = 0;
    char caDataDate[8+1] = {0};
    
    T_HdiFlagSel stHdiFlagSel = {0};
    strcpy(stHdiFlagSel.caInDataObject, object);
    stHdiFlagSel.lInDataId = krhdi->lHDIId;
    iResult = dbsHdiFlagSel(KR_DBSELECT, &stHdiFlagSel);
    if (iResult != KR_DBOK && iResult != KR_DBNOTFOUND)
    {
        KR_LOG(KR_LOGERROR, "dbsHdiFlagSel [%s],[%ld] Error!",\
               stHdiFlagSel.caInDataObject, stHdiFlagSel.lInDataId);
        return -1;
    }
        
    switch(krhdi->eValueType)
    {
        case KR_FIELDTYPE_INT:
            krhdi->eValue.i = atoi(stHdiFlagSel.caOutDataFlag);
            break;
        case KR_FIELDTYPE_LONG:
            krhdi->eValue.l = atol(stHdiFlagSel.caOutDataFlag);
            break;
        case KR_FIELDTYPE_DOUBLE:
            krhdi->eValue.d = atof(stHdiFlagSel.caOutDataFlag);
            break;
        case KR_FIELDTYPE_STRING:
            strncpy(krhdi->eValue.s, stHdiFlagSel.caOutDataFlag, \
                    sizeof(krhdi->eValue.s));
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported ValueType [%s],[%c]!",\
                krhdi->ptShmHDIDef->caStatisticsType, krhdi->eValueType);
            return -1;
    }
    
    krhdi->eValueInd = KR_VALUE_SETED;
    return 0;
}
