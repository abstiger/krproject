#include "kr_data_item_sdi.h"
#include "krparam/kr_param_class_sdi.h"


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
            kr_data_get_type, kr_data_get_value);
    if (ptSdi->ptFilterCalc == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calc_construct filter failed!");
        return NULL;
    }

    /*
    ptSdi->ptResultCalc = kr_calc_construct(
            ptParamSdi->caSdiResultFormat[0], \
            ptParamSdi->caSdiResultString, \
            kr_data_get_type, kr_data_get_value);
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


int kr_data_item_sdi_aggr(T_KRDataItem *ptDataItem, T_KRData *ptData)
{
    T_KRParamSdi *ptParamSdi = (T_KRParamSdi *)ptDataItem->ptDataItemDef;
    T_KRSdi *ptSdi = (T_KRSdi *)ptDataItem->ptPrivate;
    
    //FIXME:get record list 
    /*
    ptSdi->ptRecList = kr_db_select_by_record(ptData->ptCurrRec, 
        ptParamSdi->lStatisticsIndex, 
        ptParamSdi->lSdiId, //FIXME:tBeginTime
        ptParamSdi->lSdiId, //FIXME:tEndTime
        KR_FIELDID_TRANSTIME //FIXME:iSortFieldId
        );
        */

    int iResult = -1, iAbsLoc = -1, iRelLoc = -1;
    
    T_KRListNode *node = ptSdi->ptRecList->tail;
    while(node)
    {
        ptData->ptTravRec = (T_KRRecord *)kr_list_value(node);
        
        iAbsLoc++; 
                
        if (ptParamSdi->caLocationProperty[0] == KR_LOC_ABSOLUTE) {
            if (ptParamSdi->lStatisticsLocation != iAbsLoc) {
                node = node->prev;
                continue;
            }
        }
        
        if (ptParamSdi->lStatisticsDatasrc != \
            kr_record_get_input_id(ptData->ptTravRec)) {
            node = node->prev;
            continue;
        }
        
        iResult = kr_calc_eval(ptSdi->ptFilterCalc, ptData);
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
        
        iResult = kr_calc_eval(ptSdi->ptResultCalc, ptData);
        if (iResult != 0) {
            KR_LOG(KR_LOGERROR, "kr_calc_eval result failed!");
            return -1;
        } else if (kr_calc_ind(ptSdi->ptResultCalc) != KR_VALUE_SETED) {
            KR_LOG(KR_LOGERROR, "kr_calc_eval result failed!");
            return -1;
        }

        /*add this record to related*/
        kr_hashtable_insert(ptSdi->ptRelated, ptData->ptTravRec, ptData->ptTravRec);
    
        /* This is what the difference between SDI and DDI:
         * SDI only set once, while DDI still need to traversal all the list
         */
        ptDataItem->eValueInd = KR_VALUE_SETED;
        memcpy(&ptDataItem->uValue, kr_calc_value(ptSdi->ptResultCalc), sizeof(ptDataItem->uValue));
        
        break;
    }
    
    return 0;
}


static int _kr_data_item_sdi_load(char *psParamClassName, char *psParamObjectKey, 
        char *psParamObjectString, void *ptParamObject, void *data)
{
    T_KRParamSdi *ptParamSdi = (T_KRParamSdi *)ptParamObject;
    T_KRHashTable *ptItemTable = (T_KRHashTable *)data;

    T_KRDataItem *ptDataItem = kr_data_item_new(
            ptParamSdi,
            ptParamSdi->caSdiName, //FIXME
            ptParamSdi->lSdiId, 
            kr_data_item_sdi_new,
            kr_data_item_sdi_aggr,
            kr_data_item_sdi_free);
    if (ptDataItem == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_item_create [%ld] failed!", \
                ptParamSdi->lSdiId);
        return -1; 
    }

    //insert into item table
    kr_hashtable_replace(ptItemTable, ptDataItem->caDataItemId, ptDataItem);

    return 0;
}


int kr_data_item_sdi_load(T_KRHashTable *ptItemTable, T_KRParam *ptParam)
{
    //load sdi
    if (kr_param_object_foreach(ptParam, KR_PARAM_SDI, 
                _kr_data_item_sdi_load, ptItemTable) != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_item_sdi_load Error!");
        return -1;
    }
    
    return 0;
}
