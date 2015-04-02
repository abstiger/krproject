#include "kr_data.h"

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


static int kr_hdi_aggr_day(T_KRHDI *ptHDI, T_KRData *ptData, char *object);
static int kr_hdi_aggr_mon(T_KRHDI *ptHDI, T_KRData *ptData, char *object);
static int kr_hdi_aggr_flag(T_KRHDI *ptHDI, T_KRData *ptData, char *object);

int kr_hdi_aggr_func(T_KRHDI *ptHDI, T_KRData *ptData)
{
    int nRet = 0;
    char caDataObject[KR_HDI_OBJECT_LEN+1];

    switch(ptHDI->eKeyType)
    {
        case KR_TYPE_INT:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%d", *(int *)ptHDI->pKeyValue);
            break;         
        case KR_TYPE_LONG:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%ld", *(long *)ptHDI->pKeyValue);
            break; 
        case KR_TYPE_DOUBLE:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%lf", *(double *)ptHDI->pKeyValue);
            break; 
        case KR_TYPE_STRING:
            snprintf(caDataObject, sizeof(caDataObject), \
                     "%s", (char *)ptHDI->pKeyValue);
            break; 
        default:
            KR_LOG(KR_LOGERROR, "Unsupported object type[%c]!", \
                   ptHDI->eKeyType);
            return -1;
    }

    switch(ptHDI->ptParamHDIDef->caStatisticsType[0])
    {
        case KR_HDI_STATISTICS_DAY:
            nRet = kr_hdi_aggr_day(ptHDI, ptData, caDataObject);
            if (nRet != 0) {
                KR_LOG(KR_LOGERROR, "kr_hdi_aggr_day Error!");
            }
            break;
        case KR_HDI_STATISTICS_MONTH:
            nRet = kr_hdi_aggr_mon(ptHDI, ptData, caDataObject);
            if (nRet != 0) {
                KR_LOG(KR_LOGERROR, "kr_hdi_aggr_mon Error!");
            }
            break;
        case KR_HDI_STATISTICS_FLAG:
            nRet = kr_hdi_aggr_flag(ptHDI, ptData, caDataObject);
            if (nRet != 0) {
                KR_LOG(KR_LOGERROR, "kr_hdi_aggr_flag Error!");
            }
            break;
        default:
            KR_LOG(KR_LOGERROR, "Unsupported statistics type[%s]!", \
                   ptHDI->ptParamHDIDef->caStatisticsType);
            return -1;        
    }
        
    return 0;
}


int kr_hdi_aggr_day(T_KRHDI *ptHDI, T_KRData *ptData, char *object)
{
    int iResult = 0;
    double dValue = 0.0;
    T_DbsEnv *ptDbsEnv = ptData->ptDbsEnv;
    
    T_HdiDaySel stHdiDaySel = {0};
    strcpy(stHdiDaySel.caInDataObject, object);
    stHdiDaySel.lInDataId = ptHDI->lHDIId;
    time_t tEndTime = kr_get_transtime(ptData->ptCurrRec);
    time_t tBeginTime = tEndTime - ptHDI->ptParamHDIDef->lStatisticsValue;
    kr_ttime_to_date(tBeginTime, stHdiDaySel.caInDataDateBegin);
    kr_ttime_to_date(tEndTime, stHdiDaySel.caInDataDateEnd);
    iResult = dbsHdiDaySel(ptDbsEnv, KR_DBSELECT, &stHdiDaySel);
    if (iResult != KR_DBOK && iResult != KR_DBNOTFOUND) {
        KR_LOG(KR_LOGERROR, "dbsHdiDaySel [%s],[%ld],[%s],[%s] Error![%s]:[%s]",
                stHdiDaySel.caInDataObject, stHdiDaySel.lInDataId,
                stHdiDaySel.caInDataDateBegin, stHdiDaySel.caInDataDateEnd,
                ptDbsEnv->sqlstate, ptDbsEnv->sqlerrmsg);
        return -1;
    }
    
    //TODO:sqlite3 let sum,cnt->char(255)
    /*
    switch(ptHDI->ptParamHDIDef->caStatisticsMethod[0])
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
                ptHDI->ptParamHDIDef->caStatisticsMethod);
            return -1;
    }
    */
    
    switch(ptHDI->eValueType)
    {
        case KR_TYPE_INT:
            ptHDI->uValue.i = (int )dValue;
            break;
        case KR_TYPE_LONG:
            ptHDI->uValue.l = (long )dValue;
            break;
        case KR_TYPE_DOUBLE:
            ptHDI->uValue.d = (double )dValue;
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported ValueType [%s],[%c]!",\
                ptHDI->ptParamHDIDef->caStatisticsType, ptHDI->eValueType);
            return -1;
    }
    
    ptHDI->eValueInd = KR_VALUE_SETED;
    return 0;
}

int kr_hdi_aggr_mon(T_KRHDI *ptHDI, T_KRData *ptData, char *object)
{
    int iResult = 0;
    double dValue = 0.0;
    char caDataDate[8+1] = {0};
    T_DbsEnv *ptDbsEnv = ptData->ptDbsEnv;
    
    T_HdiMonSel stHdiMonSel = {0};
    strcpy(stHdiMonSel.caInDataObject, object);
    stHdiMonSel.lInDataId = ptHDI->lHDIId;
    time_t tEndTime = kr_get_transtime(ptData->ptCurrRec);
    time_t tBeginTime = tEndTime - ptHDI->ptParamHDIDef->lStatisticsValue;
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
    /*
    switch(ptHDI->ptParamHDIDef->caStatisticsMethod[0])
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
                ptHDI->ptParamHDIDef->caStatisticsMethod);
            return -1;
    }
    */
    
    switch(ptHDI->eValueType)
    {
        case KR_TYPE_INT:
            ptHDI->uValue.i = (int )dValue;
            break;
        case KR_TYPE_LONG:
            ptHDI->uValue.l = (long )dValue;
            break;
        case KR_TYPE_DOUBLE:
            ptHDI->uValue.d = (double )dValue;
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported ValueType [%s],[%c]!",\
                ptHDI->ptParamHDIDef->caStatisticsType, ptHDI->eValueType);
            return -1;
    }
    
    ptHDI->eValueInd = KR_VALUE_SETED;
    return 0;
}


int kr_hdi_aggr_flag(T_KRHDI *ptHDI, T_KRData *ptData, char *object)
{
    int iResult = 0;
    char caDataDate[8+1] = {0};
    T_DbsEnv *ptDbsEnv = ptData->ptDbsEnv;
    
    T_HdiFlagSel stHdiFlagSel = {0};
    strcpy(stHdiFlagSel.caInDataObject, object);
    stHdiFlagSel.lInDataId = ptHDI->lHDIId;
    iResult = dbsHdiFlagSel(ptDbsEnv, KR_DBSELECT, &stHdiFlagSel);
    if (iResult != KR_DBOK && iResult != KR_DBNOTFOUND) {
        KR_LOG(KR_LOGERROR, "dbsHdiFlagSel [%s],[%ld] Error![%s]:[%s]",\
               stHdiFlagSel.caInDataObject, stHdiFlagSel.lInDataId,
               ptDbsEnv->sqlstate, ptDbsEnv->sqlerrmsg);
        return -1;
    }
        
    switch(ptHDI->eValueType)
    {
        case KR_TYPE_INT:
            ptHDI->uValue.i = atoi(stHdiFlagSel.caOutDataFlag);
            break;
        case KR_TYPE_LONG:
            ptHDI->uValue.l = atol(stHdiFlagSel.caOutDataFlag);
            break;
        case KR_TYPE_DOUBLE:
            ptHDI->uValue.d = atof(stHdiFlagSel.caOutDataFlag);
            break;
        case KR_TYPE_STRING:
            ptHDI->uValue.s = kr_strdup(stHdiFlagSel.caOutDataFlag);
            break;
        default:
            KR_LOG(KR_LOGERROR, "unsupported ValueType [%s],[%c]!",\
                ptHDI->ptParamHDIDef->caStatisticsType, ptHDI->eValueType);
            return -1;
    }
    
    ptHDI->eValueInd = KR_VALUE_SETED;
    return 0;
}


int kr_hdi_compute(T_KRHDI *ptHDI, T_KRData *ptData)
{
    /*initialize first*/
    kr_hdi_init(ptHDI);

    int iIndexId = ptHDI->ptParamHDIDef->lStatisticsIndex;
    
    T_KRRecord *ptCurrRec = ptData->ptCurrRec;
    T_KRTable *ptTable = ptCurrRec->ptTable;
    T_KRDB *ptDB = ptTable->ptDB;
    T_KRIndexTable *ptIndexTable = \
        kr_index_table_get(ptDB, iIndexId, ptTable->iTableId);
    if (ptIndexTable == NULL) {
        KR_LOG(KR_LOGERROR, "index[%d] table[%d] not found!", \
               iIndexId, ptTable->iTableId);
        return -1;
    }
    ptHDI->ptCurrRec = ptData->ptCurrRec;
    ptHDI->eKeyType  = \
        kr_field_get_type(ptCurrRec, ptIndexTable->iIndexFieldId);
    ptHDI->pKeyValue = \
        kr_field_get_value(ptCurrRec, ptIndexTable->iIndexFieldId);

    if (ptHDI->pfHDIAggr == NULL) 
        ptHDI->pfHDIAggr = (KRHDIAggrFunc )kr_hdi_aggr_func;
    if (ptHDI->pfHDIAggr(ptHDI, ptData) != 0) {
        KR_LOG(KR_LOGERROR, "Run HDI[%ld] AggrFunc failed!", ptHDI->lHDIId);
        return -1;
    }
    
    return 0;
}


E_KRType kr_hdi_get_type(int hid, T_KRData *ptData)
{
    if (ptData == NULL) return KR_TYPE_UNKNOWN;
        
    long lHID = (long )hid;
    T_KRHDITable *ptHdiTable = ptData->ptHdiTable;
    T_KRHDI *ptHDI = kr_hashtable_lookup(ptHdiTable->ptHDITable, &lHID);
    if (ptHDI == NULL) return KR_TYPE_UNKNOWN;
    
    return ptHDI->eValueType;
}


static void kr_hdi_get_value_from_cache(T_KRHDI *ptHDI, T_KRData *ptData)
{
    T_KRHDICacheValue *cache_value = \
        kr_hdi_cache_get(ptData->ptHDICache, ptHDI->pKeyValue, ptHDI->lHDIId);
    //TODO:replace "00000000" with sharememory's batchdate
    if (cache_value->eValueInd == KR_VALUE_SETED &&
        strcmp(cache_value->caDate, "00000000") >= 0) {
        /*if cached and date valid, get it directly*/
        KR_LOG(KR_LOGDEBUG, "Get HDI [%ld] value date cached!", ptHDI->lHDIId);
        switch(ptHDI->eValueType)
        {
            case KR_TYPE_INT:
                ptHDI->uValue.i = cache_value->uValue.i; break;
            case KR_TYPE_LONG:
                ptHDI->uValue.l = cache_value->uValue.l; break;
            case KR_TYPE_DOUBLE:
                ptHDI->uValue.d = cache_value->uValue.d; break;
            case KR_TYPE_STRING:
                ptHDI->uValue.s = kr_strdup(cache_value->uValue.s);
                break;
        }
        ptHDI->eValueInd = KR_VALUE_SETED;
    } else {
        /*if not, compute it and set the cache*/
        kr_hdi_compute(ptHDI, ptData);
        if (ptHDI->eValueInd != KR_VALUE_SETED) {
            KR_LOG(KR_LOGDEBUG, "kr_hdi_compute [%ld] unset!", ptHDI->lHDIId);
            return;
        }
        
        cache_value->eValueType = ptHDI->eValueType;
        time_t tTransTime = kr_get_transtime(ptData->ptCurrRec);
        kr_ttime_to_date(tTransTime, cache_value->caDate);
        switch(ptHDI->eValueType)
        {
            case KR_TYPE_INT:
                cache_value->uValue.i = ptHDI->uValue.i; break;
            case KR_TYPE_LONG:
                cache_value->uValue.l = ptHDI->uValue.l; break;
            case KR_TYPE_DOUBLE:
                cache_value->uValue.d = ptHDI->uValue.d; break;
            case KR_TYPE_STRING:
                cache_value->uValue.s = kr_strdup(ptHDI->uValue.s);
                break;
        }
        cache_value->eValueInd = KR_VALUE_SETED;
    }
}


void *kr_hdi_get_value(int hid, T_KRData *ptData)
{
    if (ptData == NULL) return NULL;
    
    long lHID = (long )hid;
    T_KRHDITable *ptHdiTable = ptData->ptHdiTable;
    T_KRHDI *ptHDI = kr_hashtable_lookup(ptHdiTable->ptHDITable, &lHID);
    if (ptHDI == NULL) return NULL;
    
    if (ptHDI->ptCurrRec != ptData->ptCurrRec) {
        /*get the value from cache*/
        if (ptData->ptHDICache != NULL) {
            kr_hdi_get_value_from_cache(ptHDI, ptData);
        } else if (kr_hdi_compute(ptHDI, ptData) != 0) {
            KR_LOG(KR_LOGDEBUG, "kr_hdi_compute [%d] unset!", hid);
            return NULL;
        }
        ptHDI->ptCurrRec = ptData->ptCurrRec;
    } else {
        KR_LOG(KR_LOGDEBUG, "Get HDI [%d] value record cached!", hid);
    }
    
    if (ptHDI->eValueInd == KR_VALUE_SETED) {
        return kr_get_value(&ptHDI->uValue, ptHDI->eValueType);
    }
    return NULL;
}

