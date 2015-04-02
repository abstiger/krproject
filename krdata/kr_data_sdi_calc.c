#include "kr_data.h"

typedef enum {
    KR_LOC_ABSOLUTE     = '0', 
    KR_LOC_RELATIVE     = '1'  
}E_KRLocProp;

int kr_sdi_aggr_func(T_KRSDI *ptSDI, T_KRData *ptData)
{
    int iResult = -1;
    int iAbsLoc = -1;
    int iRelLoc = -1;
    
    T_KRListNode *node = ptSDI->ptRecList->tail;
    while(node)
    {
        ptData->ptRecord = (T_KRRecord *)kr_list_value(node);
        
        iAbsLoc++; 
                
        if (ptSDI->ptParamSDIDef->caLocationProperty[0] == KR_LOC_ABSOLUTE) {
            if (ptSDI->ptParamSDIDef->lStatisticsLocation != iAbsLoc) {
                node = node->prev;
                continue;
            }
        }
        
        if (((T_KRTable *)ptData->ptRecord->ptTable)->iTableId != \
            ptSDI->ptParamSDIDef->lStatisticsDatasrc) {
            node = node->prev;
            continue;
        }
        
        iResult = kr_calc_eval(ptSDI->ptSDICalc, ptData);
        if (iResult != 0) {
            KR_LOG(KR_LOGERROR, "kr_calc_eval [%ld] failed!", ptSDI->lSDIId);
            return -1;
        } else if (kr_calc_type(ptSDI->ptSDICalc) != KR_TYPE_BOOL) {
            KR_LOG(KR_LOGERROR, "result_type of sdi_calc must be boolean!");
            return -1;
        } else if (kr_calc_ind(ptSDI->ptSDICalc) != KR_VALUE_SETED ||
                   !kr_calc_value(ptSDI->ptSDICalc)->b) {
            node = node->prev;
            continue;
        }
    
        iRelLoc++; 
            
        if (ptSDI->ptParamSDIDef->caLocationProperty[0] == KR_LOC_RELATIVE) {
            if (ptSDI->ptParamSDIDef->lStatisticsLocation != iRelLoc) {
                node = node->prev;
                continue;
            }
        }
            
        void *val = kr_field_get_value(ptData->ptRecord, \
                          ptSDI->ptParamSDIDef->lStatisticsField);
        switch(ptSDI->eValueType)
        {
            case KR_TYPE_INT:
                ptSDI->uValue.i = *(int *)val;
                break;
            case KR_TYPE_LONG:
                ptSDI->uValue.l = *(long *)val;
                break;
            case KR_TYPE_DOUBLE:
                ptSDI->uValue.d = *(double *)val;
                break;
            case KR_TYPE_STRING:
                ptSDI->uValue.s = kr_strdup(val);
                break;
            default:
                KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", ptSDI->eValueType);
                return -1;
        }

        /*add this record to related*/
        kr_hashtable_insert(ptSDI->ptRelated, \
                ptData->ptRecord, ptData->ptRecord);
    
        /* This is what the difference between SDI and DDI:
         * SDI only set once, while DDI still need to traversal all the list
         */
        ptSDI->eValueInd = KR_VALUE_SETED;
        break;
        
        node = node->prev;
    }
    
    return 0;
}


int kr_sdi_compute(T_KRSDI *ptSDI, T_KRData *ptData)
{
    /*initialize first*/
    kr_sdi_init(ptSDI);
    
    int iIndexId = ptSDI->ptParamSDIDef->lStatisticsIndex;
    
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
    ptSDI->ptCurrRec = ptData->ptCurrRec;
    ptSDI->eKeyType  = \
        kr_field_get_type(ptCurrRec, ptIndexTable->iIndexFieldId);
    ptSDI->pKeyValue = \
        kr_field_get_value(ptCurrRec, ptIndexTable->iIndexFieldId);
    //FIXME:
    /*
    ptSDI->ptRecList = \
        kr_db_select(ptDB, iIndexId, ptData->pKeyValue);
        */

    if (ptSDI->pfSDIAggr == NULL) 
        ptSDI->pfSDIAggr = (KRSDIAggrFunc )kr_sdi_aggr_func;
    if (ptSDI->pfSDIAggr(ptSDI, ptData) != 0) {
        KR_LOG(KR_LOGERROR, "Run SDI[%ld] AggrFunc failed!", ptSDI->lSDIId);
        return -1;
    }
    
    return 0;
}


E_KRType kr_sdi_get_type(int sid, T_KRData *ptData)
{
    if (ptData == NULL) return KR_TYPE_UNKNOWN;
        
    long lSID = (long )sid;
    T_KRSDITable *ptSdiTable = ptData->ptSdiTable;
    T_KRSDI *ptSDI = kr_hashtable_lookup(ptSdiTable->ptSDITable, &lSID);
    if (ptSDI == NULL) return KR_TYPE_UNKNOWN;
    
    return ptSDI->eValueType;
}

void *kr_sdi_get_value(int sid, T_KRData *ptData)
{
    if (ptData == NULL) return NULL;
        
    long lSID = (long )sid;
    T_KRSDITable *ptSdiTable = ptData->ptSdiTable;
    T_KRSDI *ptSDI = kr_hashtable_lookup(ptSdiTable->ptSDITable, &lSID);
    if (ptSDI == NULL) return NULL;
    
    if (ptSDI->ptCurrRec != ptData->ptCurrRec) {
        kr_sdi_compute(ptSDI, ptData);
        if (ptSDI->eValueInd != KR_VALUE_SETED) {
            KR_LOG(KR_LOGDEBUG, "kr_sdi_compute [%d] unset!", sid);
            return NULL;
        }
        ptSDI->ptCurrRec = ptData->ptCurrRec;
    } else {
        KR_LOG(KR_LOGDEBUG, "Get SDI [%d] value record cached!", sid);
    }
    
    if (ptSDI->eValueInd == KR_VALUE_SETED) {
        return kr_get_value(&ptSDI->uValue, ptSDI->eValueType);
    }
    return NULL;
}

