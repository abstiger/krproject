#include "krparam/kr_dym_hdi.h"
#include "kr_context.h"

#include "dbs/dbs_basopr.h"
#include "dbs/dbs/hdi_day_sel.h"
#include "dbs/dbs/hdi_mon_sel.h"
#include "dbs/dbs/hdi_flag_sel.h"

#define KR_HDI_OBJECT_LEN 30

typedef enum {
    KR_HDI_STATISTICS_DAY     = '0',  /*day's statistics*/
    KR_HDI_STATISTICS_MONTH   = '1',  /*month's statistics*/
    KR_HDI_STATISTICS_FLAG    = '2'   /*flag's statistics*/
}E_KRHDIStatisticsType;

typedef enum {
    KR_HDI_METHOD_SUM        = '0',  /*sum*/
    KR_HDI_METHOD_MIN        = '1',  /*min*/
    KR_HDI_METHOD_MAX        = '2',  /*max*/
    KR_HDI_METHOD_COUNT      = '3',  /*count*/
    KR_HDI_METHOD_FLAG       = 'F'   /*flag*/
}E_KRHDIMethod;


static int kr_hdi_aggr_day(T_KRHDI *krhdi, T_KRContext *krcontext, char *object);
static int kr_hdi_aggr_mon(T_KRHDI *krhdi, T_KRContext *krcontext, char *object);
static int kr_hdi_aggr_flag(T_KRHDI *krhdi, T_KRContext *krcontext, char *object);

int kr_hdi_aggr_func(T_KRHDI *krhdi, T_KRContext *krcontext)
{
    int nRet = 0;
    char caDataObject[KR_HDI_OBJECT_LEN+1];

    switch(krcontext->eKeyType)
    {
        case KR_TYPE_INT:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%d", *(int *)krcontext->pKeyValue);
            break;         
        case KR_TYPE_LONG:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%ld", *(long *)krcontext->pKeyValue);
            break; 
        case KR_TYPE_DOUBLE:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%lf", *(double *)krcontext->pKeyValue);
            break; 
        case KR_TYPE_STRING:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%s", (char *)krcontext->pKeyValue);
            break; 
        default:
            KR_LOG(KR_LOGERROR, "Unsupported object type[%c]!", \
                   krcontext->eKeyType);
            return -1;
    }

    switch(krhdi->ptShmHDIDef->caStatisticsType[0])
    {
        case KR_HDI_STATISTICS_DAY:
            nRet = kr_hdi_aggr_day(krhdi, krcontext, caDataObject);
            if (nRet != 0) {
                KR_LOG(KR_LOGERROR, "kr_hdi_aggr_day Error!");
            }
            break;
        case KR_HDI_STATISTICS_MONTH:
            nRet = kr_hdi_aggr_mon(krhdi, krcontext, caDataObject);
            if (nRet != 0) {
                KR_LOG(KR_LOGERROR, "kr_hdi_aggr_mon Error!");
            }
            break;
        case KR_HDI_STATISTICS_FLAG:
            nRet = kr_hdi_aggr_flag(krhdi, krcontext, caDataObject);
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


int kr_hdi_aggr_day(T_KRHDI *krhdi, T_KRContext *krcontext, char *object)
{
    int iResult = 0;
    double dValue = 0.0;
    T_DbsEnv *dbsenv = krcontext->ptDbs;
    
    T_HdiDaySel stHdiDaySel = {0};
    strcpy(stHdiDaySel.caInDataObject, object);
    stHdiDaySel.lInDataId = krhdi->lHDIId;
    time_t tEndTime = kr_get_transtime(krcontext->ptCurrRec);
    time_t tBeginTime = tEndTime - krhdi->ptShmHDIDef->lStatisticsValue;
    kr_ttime_to_date(tBeginTime, stHdiDaySel.caInDataDateBegin);
    kr_ttime_to_date(tEndTime, stHdiDaySel.caInDataDateEnd);
    iResult = dbsHdiDaySel(dbsenv, KR_DBSELECT, &stHdiDaySel);
    if (iResult != KR_DBOK && iResult != KR_DBNOTFOUND) {
        KR_LOG(KR_LOGERROR, "dbsHdiDaySel [%s],[%ld],[%s],[%s] Error![%s]:[%s]",
                stHdiDaySel.caInDataObject, stHdiDaySel.lInDataId,
                stHdiDaySel.caInDataDateBegin, stHdiDaySel.caInDataDateEnd,
                dbsenv->sqlstate, dbsenv->sqlerrmsg);
        return -1;
    }
    
    //TODO:sqlite3 let sum,cnt->char(255)
    /*
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
    */
    
    switch(krhdi->eValueType)
    {
        case KR_TYPE_INT:
            krhdi->uValue.i = (int )dValue;
            break;
        case KR_TYPE_LONG:
            krhdi->uValue.l = (long )dValue;
            break;
        case KR_TYPE_DOUBLE:
            krhdi->uValue.d = (double )dValue;
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported ValueType [%s],[%c]!",\
                krhdi->ptShmHDIDef->caStatisticsType, krhdi->eValueType);
            return -1;
    }
    
    krhdi->eValueInd = KR_VALUE_SETED;
    return 0;
}

int kr_hdi_aggr_mon(T_KRHDI *krhdi, T_KRContext *krcontext, char *object)
{
    int iResult = 0;
    double dValue = 0.0;
    char caDataDate[8+1] = {0};
    T_DbsEnv *dbsenv = krcontext->ptDbs;
    
    T_HdiMonSel stHdiMonSel = {0};
    strcpy(stHdiMonSel.caInDataObject, object);
    stHdiMonSel.lInDataId = krhdi->lHDIId;
    time_t tEndTime = kr_get_transtime(krcontext->ptCurrRec);
    time_t tBeginTime = tEndTime - krhdi->ptShmHDIDef->lStatisticsValue;
    kr_ttime_to_date(tBeginTime, caDataDate);
    memcpy(stHdiMonSel.caInDataMonthBegin, caDataDate, 6);
    kr_ttime_to_date(tEndTime, caDataDate);
    memcpy(stHdiMonSel.caInDataMonthEnd, caDataDate, 6);
    iResult = dbsHdiMonSel(dbsenv, KR_DBSELECT, &stHdiMonSel);
    if (iResult != KR_DBOK && iResult != KR_DBNOTFOUND) {
        KR_LOG(KR_LOGERROR, "dbsHdiMonSel [%s],[%ld],[%s],[%s] Error![%s]:[%s]",
                stHdiMonSel.caInDataObject, stHdiMonSel.lInDataId,
                stHdiMonSel.caInDataMonthBegin, stHdiMonSel.caInDataMonthEnd,
                dbsenv->sqlstate, dbsenv->sqlerrmsg);
        return -1;
    }
    
    //TODO:sqlite3 let sum,cnt->char(255)
    /*
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
    */
    
    switch(krhdi->eValueType)
    {
        case KR_TYPE_INT:
            krhdi->uValue.i = (int )dValue;
            break;
        case KR_TYPE_LONG:
            krhdi->uValue.l = (long )dValue;
            break;
        case KR_TYPE_DOUBLE:
            krhdi->uValue.d = (double )dValue;
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported ValueType [%s],[%c]!",\
                krhdi->ptShmHDIDef->caStatisticsType, krhdi->eValueType);
            return -1;
    }
    
    krhdi->eValueInd = KR_VALUE_SETED;
    return 0;
}


int kr_hdi_aggr_flag(T_KRHDI *krhdi, T_KRContext *krcontext, char *object)
{
    int iResult = 0;
    char caDataDate[8+1] = {0};
    T_DbsEnv *dbsenv = krcontext->ptDbs;
    
    T_HdiFlagSel stHdiFlagSel = {0};
    strcpy(stHdiFlagSel.caInDataObject, object);
    stHdiFlagSel.lInDataId = krhdi->lHDIId;
    iResult = dbsHdiFlagSel(dbsenv, KR_DBSELECT, &stHdiFlagSel);
    if (iResult != KR_DBOK && iResult != KR_DBNOTFOUND) {
        KR_LOG(KR_LOGERROR, "dbsHdiFlagSel [%s],[%ld] Error![%s]:[%s]",\
               stHdiFlagSel.caInDataObject, stHdiFlagSel.lInDataId,
               dbsenv->sqlstate, dbsenv->sqlerrmsg);
        return -1;
    }
        
    switch(krhdi->eValueType)
    {
        case KR_TYPE_INT:
            krhdi->uValue.i = atoi(stHdiFlagSel.caOutDataFlag);
            break;
        case KR_TYPE_LONG:
            krhdi->uValue.l = atol(stHdiFlagSel.caOutDataFlag);
            break;
        case KR_TYPE_DOUBLE:
            krhdi->uValue.d = atof(stHdiFlagSel.caOutDataFlag);
            break;
        case KR_TYPE_STRING:
            krhdi->uValue.s = kr_strdup(stHdiFlagSel.caOutDataFlag);
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported ValueType [%s],[%c]!",\
                krhdi->ptShmHDIDef->caStatisticsType, krhdi->eValueType);
            return -1;
    }
    
    krhdi->eValueInd = KR_VALUE_SETED;
    return 0;
}

