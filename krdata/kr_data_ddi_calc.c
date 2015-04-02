#include "kr_data.h"

typedef enum {
    KR_DDI_STATISTICS_INCLUDE     = 'I',  /*include current record*/
    KR_DDI_STATISTICS_EXCLUDE     = 'E'   /*exclude current record*/
}E_KRDDIStatisticsType;

typedef enum {
    KR_DDI_METHOD_SUM        = '0',  /*sum*/
    KR_DDI_METHOD_MIN        = '1',  /*min*/
    KR_DDI_METHOD_MAX        = '2',  /*max*/
    KR_DDI_METHOD_COUNT      = '3',  /*count*/
    KR_DDI_METHOD_CON_INC    = '4',  /*continuous increase*/
    KR_DDI_METHOD_CON_DEC    = '5',  /*continuous decrease*/
    KR_DDI_METHOD_CNT_DIS    = '6'   /*count distinct*/
}E_KRDDIMethod;


int kr_ddi_aggr_func(T_KRDDI *ptDDI, T_KRData *ptData)
{
    int iResult = -1;
    int iAbsLoc = -1;
    int iRelLoc = -1;
    
    T_KRListNode *node = ptDDI->ptRecList->tail;
    while(node)
    {
        ptData->ptRecord = (T_KRRecord *)kr_list_value(node);
        
        iAbsLoc++; 
        
        if ((ptDDI->ptParamDDIDef->caStatisticsType[0] == \
                               KR_DDI_STATISTICS_EXCLUDE) && 
            (ptData->ptRecord == ptData->ptCurrRec)) {
            node = node->prev;
            continue;
        }
        
        if (((T_KRTable *)ptData->ptRecord->ptTable)->iTableId != \
            ptDDI->ptParamDDIDef->lStatisticsDatasrc) {
            node = node->prev;
            continue;
        }
        
        time_t tCurrTransTime = kr_get_transtime(ptData->ptCurrRec);
        time_t tRecTransTime = kr_get_transtime(ptData->ptRecord);
        if ((tCurrTransTime - tRecTransTime) > 
                ptDDI->ptParamDDIDef->lStatisticsValue ) {
            node = node->prev;
            continue;
        }
        
        iResult = kr_calc_eval(ptDDI->ptDDICalc, ptData);
        if (iResult != 0) {
            KR_LOG(KR_LOGERROR, "kr_calc_eval[%ld] failed!", ptDDI->lDDIId);
            return -1;
        } else if (kr_calc_type(ptDDI->ptDDICalc) != KR_TYPE_BOOL) {
            KR_LOG(KR_LOGERROR, "result_type of ddi_calc must be boolean!");
            return -1;
        } else if (kr_calc_ind(ptDDI->ptDDICalc) != KR_VALUE_SETED ||
                   !kr_calc_value(ptDDI->ptDDICalc)->b) {
            node = node->prev;
            continue;
        }
    
        iRelLoc++; 
                        
        E_KRType type = kr_field_get_type(ptData->ptRecord, ptDDI->ptParamDDIDef->lStatisticsField);
        void *val = kr_field_get_value(ptData->ptRecord, ptDDI->ptParamDDIDef->lStatisticsField);
        U_KRValue stValue = {0};
        switch(type)
        {
            case KR_TYPE_INT:
                stValue.i = *(int *)val;
                break;
            case KR_TYPE_LONG:
                stValue.l = *(long *)val;
                break;
            case KR_TYPE_DOUBLE:
                stValue.d = *(double *)val;
                break;
            case KR_TYPE_STRING:
                stValue.s = (char *)val;
                break;
            default:
                KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", type);
                return -1;
        }
        
        switch(ptDDI->ptParamDDIDef->caStatisticsMethod[0])
        {
            case KR_DDI_METHOD_SUM:
                switch(ptDDI->eValueType)
                {
                    case KR_TYPE_INT:
                        ptDDI->uValue.i = ptDDI->uValue.i + stValue.i;
                        break;
                    case KR_TYPE_LONG:
                        ptDDI->uValue.l = ptDDI->uValue.l + stValue.l;
                        break;
                    case KR_TYPE_DOUBLE:
                        ptDDI->uValue.d = ptDDI->uValue.d + stValue.d;
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", 
                                ptDDI->eValueType);
                        return -1;
                }
                break;
            case KR_DDI_METHOD_MIN:
                switch(ptDDI->eValueType)
                {
                    case KR_TYPE_INT:
                        ptDDI->uValue.i = MIN(ptDDI->uValue.i, stValue.i);
                        break;
                    case KR_TYPE_LONG:
                        ptDDI->uValue.l = MIN(ptDDI->uValue.l, stValue.l);
                        break;
                    case KR_TYPE_DOUBLE:
                        ptDDI->uValue.d = MIN(ptDDI->uValue.d, stValue.d);
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", 
                                ptDDI->eValueType);
                        return -1;
                }
                break;
            case KR_DDI_METHOD_MAX:
                switch(ptDDI->eValueType)
                {
                    case KR_TYPE_INT:
                        ptDDI->uValue.i = MAX(ptDDI->uValue.i, stValue.i);
                        break;
                    case KR_TYPE_LONG:
                        ptDDI->uValue.l = MAX(ptDDI->uValue.l, stValue.l);
                        break;
                    case KR_TYPE_DOUBLE:
                        ptDDI->uValue.d = MAX(ptDDI->uValue.d, stValue.d);
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", 
                                ptDDI->eValueType);
                        return -1;
                }
                break;
            case KR_DDI_METHOD_COUNT:
                switch(ptDDI->eValueType)
                {
                    case KR_TYPE_INT:
                        ptDDI->uValue.i = ptDDI->uValue.i + 1;
                        break;
                    case KR_TYPE_LONG:
                        ptDDI->uValue.l = ptDDI->uValue.l + 1;
                        break;
                    case KR_TYPE_DOUBLE:
                        ptDDI->uValue.d = ptDDI->uValue.d + 1;
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", 
                                ptDDI->eValueType);
                        return -1;
                }
                break;
            case KR_DDI_METHOD_CON_INC:
                //TODO
                break;
            case KR_DDI_METHOD_CON_DEC:
                //TODO
                break;
            case KR_DDI_METHOD_CNT_DIS:
                //TODO
                break;
            default:
                KR_LOG(KR_LOGERROR, "Bad Method [%c]!", \
                       ptDDI->ptParamDDIDef->caStatisticsMethod[0]);
                return -1;    
        }
        
        /*add this record to related*/
        kr_hashtable_insert(ptDDI->ptRelated, \
                ptData->ptRecord, ptData->ptRecord);
        
        node = node->prev;
    }

    /* This is what the difference between DDI and DDI:
     * DDI only set once, while DDI still need to traversal all the list
     */
    ptDDI->eValueInd = KR_VALUE_SETED;

    return 0;
}


int kr_ddi_compute(T_KRDDI *ptDDI, T_KRData *ptData)
{
    /*initialize first*/
    kr_ddi_init(ptDDI);
    
    int iIndexId = ptDDI->ptParamDDIDef->lStatisticsIndex;
    
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
    ptDDI->ptCurrRec = ptData->ptCurrRec;
    ptDDI->eKeyType  = \
        kr_field_get_type(ptCurrRec, ptIndexTable->iIndexFieldId);
    ptDDI->pKeyValue = \
        kr_field_get_value(ptCurrRec, ptIndexTable->iIndexFieldId);
    //FIXME:
    /*
    ptDDI->ptRecList = \
        kr_db_select(ptDB, iIndexId, ptData->pKeyValue);
        */

    if (ptDDI->pfDDIAggr == NULL) 
        ptDDI->pfDDIAggr = (KRDDIAggrFunc )kr_ddi_aggr_func;
    if (ptDDI->pfDDIAggr(ptDDI, ptData) != 0) {
        KR_LOG(KR_LOGERROR, "Run DDI[%ld] AggrFunc failed!", ptDDI->lDDIId);
        return -1;
    }
    
    return 0;
}


E_KRType kr_ddi_get_type(int did, T_KRData *ptData)
{
    if (ptData == NULL) return KR_TYPE_UNKNOWN;
        
    long lDID = (long )did;
    T_KRDDITable *ptDdiTable = ptData->ptDdiTable;
    T_KRDDI *ptDDI = kr_hashtable_lookup(ptDdiTable->ptDDITable, &lDID);
    if (ptDDI == NULL) return KR_TYPE_UNKNOWN;
    
    return ptDDI->eValueType;
}

void *kr_ddi_get_value(int did, T_KRData *ptData)
{
    if (ptData == NULL) return NULL;
        
    long lDID = (long )did;
    T_KRDDITable *ptDdiTable = ptData->ptDdiTable;
    T_KRDDI *ptDDI = kr_hashtable_lookup(ptDdiTable->ptDDITable, &lDID);
    if (ptDDI == NULL) return NULL;
    
    if (ptDDI->ptCurrRec != ptData->ptCurrRec) {
        kr_ddi_compute(ptDDI, ptData);
        if (ptDDI->eValueInd != KR_VALUE_SETED) {
            KR_LOG(KR_LOGDEBUG, "kr_ddi_compute [%d] unset!", did);
            return NULL;
        }
        ptDDI->ptCurrRec = ptData->ptCurrRec;
    } else {
        KR_LOG(KR_LOGDEBUG, "Get DDI [%d] value record cached!", did);
    }
    
    if (ptDDI->eValueInd == KR_VALUE_SETED) {
        return kr_get_value(&ptDDI->uValue, ptDDI->eValueType);
    }
    return NULL;
}
