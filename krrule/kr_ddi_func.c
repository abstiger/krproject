#include "kr_ddi.h"
#include "kr_ddi_func.h"

int kr_ddi_aggr_func(T_KRDDI *krddi, T_KRContext *krcontext)
{
    int iResult = -1;
    int iAbsLoc = -1;
    int iRelLoc = -1;
    
    time_t tCurrTransTime = kr_get_transtime(krcontext->ptCurrRec);
    time_t tRecTransTime;
    
    T_KRListNode *node = krcontext->ptRecList->tail;
    while(node)
    {
        krcontext->ptRecord = (T_KRRecord *)kr_list_value(node);
        
        iAbsLoc++; /*绝对位置加一*/
        
        /*当笔是否包含校验*/
        if ((krddi->ptShmDDIDef->caStatisticsType[0] == \
                               KR_DDI_STATISTICS_EXCLUDE) && 
            (krcontext->ptRecord == krcontext->ptCurrRec)) {
            node = node->prev;
            continue;
        }
        
        /*统计数据源校验*/
        if (((T_KRTable *)krcontext->ptRecord->ptTable)->iTableId != \
            krddi->ptShmDDIDef->lStatisticsDatasrc) {
            node = node->prev;
            continue;
        }
        
        /*时间窗口校验*/
        tRecTransTime = kr_get_transtime(krcontext->ptRecord);
        if ((tCurrTransTime - tRecTransTime) > krddi->ptShmDDIDef->lStatisticsValue ) {
            node = node->prev;
            continue;
        }
        
        /*过滤器校验*/
        iResult = kr_calc_eval(krddi->ptDDICalc, krcontext);
        if (iResult != 0) {
            KR_LOG(KR_LOGERROR, "kr_calc_eval[%ld] failed!", krddi->lDDIId);
            return -1;
        } else if (krddi->ptDDICalc->result_type != KR_CALCTYPE_BOOLEAN) {
            KR_LOG(KR_LOGERROR, "result_type of ddi_calc must be boolean!");
            return -1;
        } else if (krddi->ptDDICalc->result_ind != KR_VALUE_SETED ||
                   !krddi->ptDDICalc->result_value.b) {
            node = node->prev;
            continue;
        }
    
        iRelLoc++; /*相对位置加一*/
                        
        /*获取数据项值*/
        E_KRFieldType type = kr_get_field_type(krcontext->ptRecord, krddi->ptShmDDIDef->lStatisticsField);
        void *val = kr_get_field_value(krcontext->ptRecord, krddi->ptShmDDIDef->lStatisticsField);
        U_KRFieldVal stValue = {0};
        switch(type)
        {
            case KR_FIELDTYPE_INT:
                stValue.i = *(int *)val;
                break;
            case KR_FIELDTYPE_LONG:
                stValue.l = *(long *)val;
                break;
            case KR_FIELDTYPE_DOUBLE:
                stValue.d = *(double *)val;
                break;
            case KR_FIELDTYPE_STRING:
                stValue.s = (char *)val;
                break;
            default:
                KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", type);
                return -1;
        }
        
        switch(krddi->ptShmDDIDef->caStatisticsMethod[0])
        {
            case KR_DDI_METHOD_SUM:
                switch(krddi->eValueType)
                {
                    case KR_FIELDTYPE_INT:
                        krddi->uValue.i = krddi->uValue.i + stValue.i;
                        break;
                    case KR_FIELDTYPE_LONG:
                        krddi->uValue.l = krddi->uValue.l + stValue.l;
                        break;
                    case KR_FIELDTYPE_DOUBLE:
                        krddi->uValue.d = krddi->uValue.d + stValue.d;
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", 
                                krddi->eValueType);
                        return -1;
                }
                break;
            case KR_DDI_METHOD_MIN:
                switch(krddi->eValueType)
                {
                    case KR_FIELDTYPE_INT:
                        krddi->uValue.i = MIN(krddi->uValue.i, stValue.i);
                        break;
                    case KR_FIELDTYPE_LONG:
                        krddi->uValue.l = MIN(krddi->uValue.l, stValue.l);
                        break;
                    case KR_FIELDTYPE_DOUBLE:
                        krddi->uValue.d = MIN(krddi->uValue.d, stValue.d);
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", 
                                krddi->eValueType);
                        return -1;
                }
                break;
            case KR_DDI_METHOD_MAX:
                switch(krddi->eValueType)
                {
                    case KR_FIELDTYPE_INT:
                        krddi->uValue.i = MAX(krddi->uValue.i, stValue.i);
                        break;
                    case KR_FIELDTYPE_LONG:
                        krddi->uValue.l = MAX(krddi->uValue.l, stValue.l);
                        break;
                    case KR_FIELDTYPE_DOUBLE:
                        krddi->uValue.d = MAX(krddi->uValue.d, stValue.d);
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", 
                                krddi->eValueType);
                        return -1;
                }
                break;
            case KR_DDI_METHOD_COUNT:
                switch(krddi->eValueType)
                {
                    case KR_FIELDTYPE_INT:
                        krddi->uValue.i = krddi->uValue.i + 1;
                        break;
                    case KR_FIELDTYPE_LONG:
                        krddi->uValue.l = krddi->uValue.l + 1;
                        break;
                    case KR_FIELDTYPE_DOUBLE:
                        krddi->uValue.d = krddi->uValue.d + 1;
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", 
                                krddi->eValueType);
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
                       krddi->ptShmDDIDef->caStatisticsMethod[0]);
                return -1;    
        }
    
        /* This is what the difference between SDI and DDI:
         * SDI only set once, while DDI still need to traversal all the list
         */
        krddi->eValueInd = KR_VALUE_SETED;
        
        node = node->prev;
    }

    return 0;
}
