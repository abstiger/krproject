#include "kr_data_item.h"

/*data item*/
T_KRDataItem *kr_data_item_new(void *ptDataItemDef, char kind, int id,
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue,
        KRDataItemNewFunc pfDataItemNew, 
        KRDataItemAggrFunc pfDataItemAggr, 
        KRDataItemFreeFunc pfDataItemFree)
{
    T_KRDataItem *ptDataItem = kr_calloc(sizeof(T_KRDataItem));
    if (ptDataItem == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptDataItem failed!");
        return NULL;
    }
    ptDataItem->ptDataItemDef = ptDataItemDef;
    ptDataItem->cDataItemKind = kind;
    ptDataItem->lDataItemId = id;
    ptDataItem->pfGetType = pfGetType;
    ptDataItem->pfGetValue = pfGetValue;
    ptDataItem->pfDataItemNew = pfDataItemNew;
    ptDataItem->pfDataItemAggr = pfDataItemAggr;
    ptDataItem->pfDataItemFree = pfDataItemFree;
    snprintf(ptDataItem->caDataItemId, sizeof(ptDataItem->caDataItemId), 
            "%c_%d", kind, id);

    //initialize variable
    ptDataItem->ptCurrRec = NULL;
    ptDataItem->eValueInd == KR_VALUE_UNSET;
    
    //new private data if new function specified
    if (ptDataItem->pfDataItemNew != NULL) {
        ptDataItem->ptPrivate = ptDataItem->pfDataItemNew(ptDataItem);
        if (ptDataItem->ptPrivate == NULL) {
            KR_LOG(KR_LOGERROR, "call pfDataItemNew failed!");
            kr_free(ptDataItem);
            return NULL;
        }
    }

    return ptDataItem;
}


void kr_data_item_free(T_KRDataItem *ptDataItem)
{
    if (ptDataItem) {
        if (ptDataItem->pfDataItemFree != NULL) {
            ptDataItem->pfDataItemFree(ptDataItem->ptPrivate);
        }
        if (ptDataItem->eValueType == KR_TYPE_STRING) {
            kr_free(ptDataItem->uValue.s);
        }
        kr_free(ptDataItem);
    }
}


void kr_data_item_init(T_KRDataItem *ptDataItem)
{
    ptDataItem->ptCurrRec = NULL;

    /*initialize first*/
    ptDataItem->eValueInd = KR_VALUE_UNSET;

    /*string comes from kr_strdup, need kr_free*/
    if (ptDataItem->eValueType == KR_TYPE_STRING)
        kr_free(ptDataItem->uValue.s);
    memset(&ptDataItem->uValue, 0x00, sizeof(ptDataItem->uValue));
}

