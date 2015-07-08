#include "kr_data_item_ddi.h"
#include "krparam/kr_param_class_ddi.h"


void *kr_data_item_ddi_new(T_KRDataItem *ptDataItem)
{
    T_KRParamDdi *ptParamDdi = (T_KRParamDdi *)ptDataItem->ptDataItemDef;

    T_KRDdi *ptDdi = kr_calloc(sizeof(*ptDdi));
    if (ptDdi == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptDdi failed!");
        return NULL;
    }

    ptDdi->ptFilterCalc = kr_calc_construct(
            ptParamDdi->caDdiFilterFormat[0], \
            ptParamDdi->caDdiFilterString, \
            kr_data_get_type, kr_data_get_value);
    if (ptDdi->ptFilterCalc == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calc_construct filter failed!");
        return NULL;
    }

    /*
    ptDdi->ptResultCalc = kr_calc_construct(
            ptParamSdi->caSdiResultFormat[0], \
            ptParamSdi->caSdiResultString, \
            kr_data_get_type, kr_data_get_value);
    if (ptDdi->ptResultCalc == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calc_construct result failed!");
        return NULL;
    }
    */

    ptDdi->ptRelated = kr_hashtable_new(kr_pointer_hash, kr_pointer_equal);
    if (ptDdi->ptRelated == NULL) {
        KR_LOG(KR_LOGERROR, "kr_hashtable_new related failed!");
        return NULL;
    }

    ptDataItem->ptPrivate = ptDdi;
    
    return ptDdi;
}


void kr_data_item_ddi_free(void *priv) 
{
    T_KRDdi *ptDdi = (T_KRDdi *)priv;
    if (ptDdi != NULL) {
        kr_calc_destruct(ptDdi->ptFilterCalc);
        kr_calc_destruct(ptDdi->ptResultCalc);
        kr_hashtable_destroy(ptDdi->ptRelated);
        kr_free(ptDdi);
    }
}


int kr_data_item_ddi_aggr(T_KRDataItem *ptDataItem, T_KRData *ptData)
{
    T_KRParamDdi *ptParamDdi = (T_KRParamDdi *)ptDataItem->ptDataItemDef;
    T_KRDdi *ptDdi = (T_KRDdi *)ptDataItem->ptPrivate;

    //FIXME:get record list 
    /*
    ptDdi->ptRecList = kr_db_select_by_record(ptData->ptCurrRec, 
        ptParamDdi->lStatisticsIndex, 
        ptParamDdi->lDdiId, //FIXME:tBeginTime
        ptParamDdi->lDdiId, //FIXME:tEndTime
        KR_FIELDID_TRANSTIME //FIXME:iSortFieldId
        );
        */

    int iResult = -1, iAbsLoc = -1, iRelLoc = -1;
    
    T_KRListNode *node = ptDdi->ptRecList->tail;
    while(node)
    {
        ptData->ptTravRec = (T_KRRecord *)kr_list_value(node);
        
        iAbsLoc++; 
        
        if ((ptParamDdi->caStatisticsType[0] == \
             KR_DDI_STATISTICS_EXCLUDE) && 
            (ptData->ptTravRec == ptData->ptCurrRec)) {
            node = node->prev;
            continue;
        }
        
        if (ptParamDdi->lStatisticsDatasrc != \
            kr_record_get_input_id(ptData->ptTravRec)) {
            node = node->prev;
            continue;
        }
        
        time_t tCurrTransTime = kr_record_get_transtime(ptData->ptCurrRec);
        time_t tRecTransTime = kr_record_get_transtime(ptData->ptTravRec);
        if ((tCurrTransTime - tRecTransTime) > ptParamDdi->lStatisticsValue ) {
            node = node->prev;
            continue;
        }
        
        iResult = kr_calc_eval(ptDdi->ptFilterCalc, ptData);
        if (iResult != 0) {
            KR_LOG(KR_LOGERROR, "kr_calc_eval filter failed!");
            return -1;
        } else if (kr_calc_type(ptDdi->ptFilterCalc) != KR_TYPE_BOOL) {
            KR_LOG(KR_LOGERROR, "filter calc type must be boolean!");
            return -1;
        } else if (kr_calc_ind(ptDdi->ptFilterCalc) != KR_VALUE_SETED ||
                   !kr_calc_value(ptDdi->ptFilterCalc)->b) {
            node = node->prev;
            continue;
        }
    
        iRelLoc++; 
                        
        E_KRType type = kr_record_get_field_type(ptData->ptTravRec, ptParamDdi->lStatisticsField);
        void *val = kr_record_get_field_value(ptData->ptTravRec, ptParamDdi->lStatisticsField);
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
        
        /*
        switch(ptParamDdi->caStatisticsMethod[0])
        {
            case KR_DDI_METHOD_SUM:
                switch(ptDataItem->eValueType)
                {
                    case KR_TYPE_INT:
                        ptDdi->uValue.i = ptDdi->uValue.i + stValue.i;
                        break;
                    case KR_TYPE_LONG:
                        ptDdi->uValue.l = ptDdi->uValue.l + stValue.l;
                        break;
                    case KR_TYPE_DOUBLE:
                        ptDdi->uValue.d = ptDdi->uValue.d + stValue.d;
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", 
                                ptDdi->eValueType);
                        return -1;
                }
                break;
            case KR_DDI_METHOD_MIN:
                switch(ptDataItem->eValueType)
                {
                    case KR_TYPE_INT:
                        ptDdi->uValue.i = MIN(ptDdi->uValue.i, stValue.i);
                        break;
                    case KR_TYPE_LONG:
                        ptDdi->uValue.l = MIN(ptDdi->uValue.l, stValue.l);
                        break;
                    case KR_TYPE_DOUBLE:
                        ptDdi->uValue.d = MIN(ptDdi->uValue.d, stValue.d);
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", 
                                ptDdi->eValueType);
                        return -1;
                }
                break;
            case KR_DDI_METHOD_MAX:
                switch(ptDataItem->eValueType)
                {
                    case KR_TYPE_INT:
                        ptDdi->uValue.i = MAX(ptDdi->uValue.i, stValue.i);
                        break;
                    case KR_TYPE_LONG:
                        ptDdi->uValue.l = MAX(ptDdi->uValue.l, stValue.l);
                        break;
                    case KR_TYPE_DOUBLE:
                        ptDdi->uValue.d = MAX(ptDdi->uValue.d, stValue.d);
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", 
                                ptDdi->eValueType);
                        return -1;
                }
                break;
            case KR_DDI_METHOD_COUNT:
                switch(ptDataItem->eValueType)
                {
                    case KR_TYPE_INT:
                        ptDdi->uValue.i = ptDdi->uValue.i + 1;
                        break;
                    case KR_TYPE_LONG:
                        ptDdi->uValue.l = ptDdi->uValue.l + 1;
                        break;
                    case KR_TYPE_DOUBLE:
                        ptDdi->uValue.d = ptDdi->uValue.d + 1;
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", 
                                ptDdi->eValueType);
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
                       ptDdi->ptParamDDIDef->caStatisticsMethod[0]);
                return -1;    
        }
        */
        
        /*add this record to related*/
        kr_hashtable_insert(ptDdi->ptRelated, \
                ptData->ptTravRec, ptData->ptTravRec);
        
        node = node->prev;
    }

    /* This is what the difference between DDI and DDI:
     * DDI only set once, while DDI still need to traversal all the list
     */
    ptDataItem->eValueInd = KR_VALUE_SETED;

    return 0;
}


static int _kr_data_item_ddi_load(char *psParamClassName, char *psParamObjectKey, 
        char *psParamObjectString, void *ptParamObject, void *data)
{
    T_KRParamDdi *ptParamDdi = (T_KRParamDdi *)ptParamObject;
    T_KRHashTable *ptItemTable = (T_KRHashTable *)data;

    T_KRDataItem *ptDataItem = kr_data_item_new(
            ptParamDdi,
            ptParamDdi->caDdiName[0], //FIXME
            ptParamDdi->lDdiId, 
            kr_data_item_ddi_new,
            kr_data_item_ddi_aggr,
            kr_data_item_ddi_free);
    if (ptDataItem == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_item_create [%ld] failed!", \
                ptParamDdi->lDdiId);
        return -1; 
    }

    //insert into item table
    kr_hashtable_replace(ptItemTable, ptDataItem->caDataItemId, ptDataItem);

    return 0;
}


int kr_data_item_ddi_load(T_KRHashTable *ptItemTable, T_KRParam *ptParam)
{
    //load ddi
    if (kr_param_object_foreach(ptParam, KR_PARAM_DDI, 
                _kr_data_item_ddi_load, ptItemTable) != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_item_ddi_load Error!");
        return -1;
    }
    
    return 0;
}