#include "kr_data_item.h"
#include "kr_data_item_set.h"
#include "kr_data_item_sdi.h"
#include "kr_data_item_ddi.h"
#include "kr_data_item_hdi.h"

/*data item*/
T_KRDataItem *kr_data_item_new(void *ptDataItemDef, char kind, int id,
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


int kr_data_item_load(T_KRHashTable *ptItemTable, T_KRParam *ptParam)
{
    int nRet = 0;
    
    //load set
    nRet = kr_data_item_set_load(ptItemTable, ptParam);
    if (nRet != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_item_set_load failed!");
        return nRet;
    }

    //load sdi
    kr_data_item_sdi_load(ptItemTable, ptParam);
    if (nRet != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_item_sdi_load failed!");
        return nRet;
    }

    //load ddi
    kr_data_item_ddi_load(ptItemTable, ptParam);
    if (nRet != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_item_ddi_load failed!");
        return nRet;
    }

    //load hdi
    kr_data_item_hdi_load(ptItemTable, ptParam);
    if (nRet != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_item_hdi_load failed!");
        return nRet;
    }

    return nRet;
}


static T_KRDataItem *kr_data_item_get(char kind, int id, T_KRData *ptData)
{
    char caDataItemId[20+1] = {0};
    snprintf(caDataItemId, sizeof(caDataItemId), "%c_%d", kind, id);

    T_KRDataItem *ptDataItem = kr_hashtable_lookup(ptData->ptItemTable, caDataItemId);
    if (ptDataItem == NULL) {
        KR_LOG(KR_LOGERROR, "kr_hashtable_lookup [%c]:[%d] failed!", kind, id);
        return NULL;
    }

    //check whether computed yet, that is record level cache
    if (ptDataItem->ptCurrRec != ptData->ptCurrRec) {
        ptDataItem->ptCurrRec = ptData->ptCurrRec;
        if (ptDataItem->pfDataItemAggr != NULL) {
            if (ptDataItem->pfDataItemAggr(ptDataItem, ptData) != 0) {
                KR_LOG(KR_LOGERROR, "pfDataItemAggr [%c]:[%d] failed!");
                return NULL;
            }
        }
    } else {
        KR_LOG(KR_LOGDEBUG, "Get [%c]:[%d] record level cached!", kind, id);
    }
    
    return ptDataItem;
}


E_KRType kr_data_item_type(char kind, int id, T_KRData *ptData)
{
    T_KRDataItem *ptDataItem = kr_data_item_get(kind, id, ptData);
    if (ptDataItem == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_item_get [%c]:[%d] failed!", kind, id);
        return KR_TYPE_UNKNOWN;
    }
    
    if (ptDataItem->eValueInd != KR_VALUE_SETED) {
        KR_LOG(KR_LOGERROR, "[%c]:[%d] value unset!", kind, id);
        return KR_TYPE_UNKNOWN;
    }

    return ptDataItem->eValueType;
}


void *kr_data_item_value(char kind, int id, T_KRData *ptData)
{
    T_KRDataItem *ptDataItem = kr_data_item_get(kind, id, ptData);
    if (ptDataItem == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_item_get [%c]:[%d] failed!", kind, id);
        return NULL;
    }
    
    if (ptDataItem->eValueInd != KR_VALUE_SETED) {
        KR_LOG(KR_LOGERROR, "[%c]:[%d] value unset!", kind, id);
        return NULL;
    }

    return kr_get_value(&ptDataItem->uValue, ptDataItem->eValueType);
}
