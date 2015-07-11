#include "kr_data_item_sdi.h"
#include "krdb/kr_db.h"


void *kr_data_item_sdi_new(T_KRDataItem *ptDataItem)
{
    T_KRParamSdi *ptParamSdi = (T_KRParamSdi *)ptDataItem->ptDataItemDef;

    T_KRSdi *ptSdi = kr_calloc(sizeof(*ptSdi));
    if (ptSdi == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptSdi failed!");
        return NULL;
    }

    ptSdi->ptFilterCalc = kr_calc_construct(
            ptParamSdi->caSdiFilterFormat[0], \
            ptParamSdi->caSdiFilterString, \
            ptDataItem->pfGetType, \
            ptDataItem->pfGetValue);
    if (ptSdi->ptFilterCalc == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calc_construct filter failed!");
        return NULL;
    }

    //FIXME:add result calc
    /*
    ptSdi->ptResultCalc = kr_calc_construct(
            ptParamSdi->caSdiResultFormat[0], \
            ptParamSdi->caSdiResultString, \
            ptDataItem->pfGetType, \
            ptDataItem->pfGetValue);
    if (ptSdi->ptResultCalc == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calc_construct result failed!");
        return NULL;
    }
    */

    ptSdi->ptRelated = kr_hashtable_new(kr_pointer_hash, kr_pointer_equal);
    if (ptSdi->ptRelated == NULL) {
        KR_LOG(KR_LOGERROR, "kr_hashtable_new related failed!");
        return NULL;
    }

    ptDataItem->ptPrivate = ptSdi;
    
    return ptSdi;
}


void kr_data_item_sdi_free(void *priv) 
{
    T_KRSdi *ptSdi = (T_KRSdi *)priv;
    if (ptSdi != NULL) {
        kr_calc_destruct(ptSdi->ptFilterCalc);
        kr_calc_destruct(ptSdi->ptResultCalc);
        kr_hashtable_destroy(ptSdi->ptRelated);
        kr_free(ptSdi);
    }
}


int kr_data_item_sdi_aggr(T_KRDataItem *ptDataItem, T_KRContext *ptContext)
{
    T_KRParamSdi *ptParamSdi = (T_KRParamSdi *)ptDataItem->ptDataItemDef;
    T_KRSdi *ptSdi = (T_KRSdi *)ptDataItem->ptPrivate;
    
    T_KRRecord *ptCurrRec = kr_context_get_data(ptContext, "curr_rec");
    if (ptCurrRec == NULL) {
        KR_LOG(KR_LOGERROR, "no current record in context");
        return -1;
    }

    T_KRDB *ptDB = kr_context_get_data(ptContext, "db");
    if (ptDB == NULL) {
        KR_LOG(KR_LOGERROR, "no db in context");
        return -1;
    }

    time_t tCurrTime = kr_record_get_transtime(ptCurrRec);
    //get record list 
    ptSdi->ptRecList = kr_db_select(ptDB,
            ptParamSdi->lStatisticsIndex, 
            NULL, //FIXME:key
            ptParamSdi->lSdiId, //FIXME:tBeginTime
            ptParamSdi->lSdiId, //FIXME:tEndTime
            KR_FIELDID_TRANSTIME //FIXME:iSortFieldId
            );

    int iResult = -1, iAbsLoc = -1, iRelLoc = -1;
    
    T_KRListNode *node = ptSdi->ptRecList->tail;
    while(node)
    {
        T_KRRecord *ptTravRec = (T_KRRecord *)kr_list_value(node);
        kr_context_add_data(ptContext, "trav_rec", ptTravRec);
        
        iAbsLoc++; 
                
        if (ptParamSdi->caLocationProperty[0] == KR_LOC_ABSOLUTE) {
            if (ptParamSdi->lStatisticsLocation != iAbsLoc) {
                node = node->prev;
                continue;
            }
        }
        
        if (ptParamSdi->lStatisticsDatasrc != \
                kr_record_get_input_id(ptTravRec)) {
            node = node->prev;
            continue;
        }
        
        iResult = kr_calc_eval(ptSdi->ptFilterCalc, ptContext);
        if (iResult != 0) {
            KR_LOG(KR_LOGERROR, "kr_calc_eval filter failed!");
            return -1;
        } else if (kr_calc_type(ptSdi->ptFilterCalc) != KR_TYPE_BOOL) {
            KR_LOG(KR_LOGERROR, "filter calc type must be boolean!");
            return -1;
        } else if (kr_calc_ind(ptSdi->ptFilterCalc) != KR_VALUE_SETED ||
                   !kr_calc_value(ptSdi->ptFilterCalc)->b) {
            node = node->prev;
            continue;
        }
    
        iRelLoc++; 
            
        if (ptParamSdi->caLocationProperty[0] == KR_LOC_RELATIVE) {
            if (ptParamSdi->lStatisticsLocation != iRelLoc) {
                node = node->prev;
                continue;
            }
        }
        
        iResult = kr_calc_eval(ptSdi->ptResultCalc, ptContext);
        if (iResult != 0) {
            KR_LOG(KR_LOGERROR, "kr_calc_eval result failed!");
            return -1;
        } else if (kr_calc_ind(ptSdi->ptResultCalc) != KR_VALUE_SETED) {
            KR_LOG(KR_LOGERROR, "kr_calc_eval result failed!");
            return -1;
        }

        /*add this record to related*/
        kr_hashtable_insert(ptSdi->ptRelated, ptTravRec, ptTravRec);
    
        /* This is what the difference between SDI and DDI:
         * SDI only set once, while DDI still need to traversal all the list
         */
        ptDataItem->eValueInd = KR_VALUE_SETED;
        memcpy(&ptDataItem->uValue, kr_calc_value(ptSdi->ptResultCalc), 
                sizeof(ptDataItem->uValue));
        
        break;
    }
    
    return 0;
}

