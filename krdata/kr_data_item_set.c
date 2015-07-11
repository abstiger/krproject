#include "kr_data_item_set.h"


void *kr_data_item_set_new(T_KRDataItem *ptDataItem)
{
    T_KRParamSet *ptParamSet = (T_KRParamSet *)ptDataItem->ptDataItemDef;

    T_KRHashSet *ptHashSet = kr_hashset_create(ptParamSet->caSetName, \
            (E_KRType )ptParamSet->caElementType[0]);
    if (ptHashSet == NULL) {
        KR_LOG(KR_LOGERROR, "kr_hashset_create [%ld] Failed!", \
                ptParamSet->lSetId);
        return NULL;
    }
    
    for (int i=0; i<ptParamSet->lEleCount; ++i) {
        switch(ptHashSet->type)
        {
            case KR_TYPE_INT: 
                kr_hashset_add(ptHashSet, &ptParamSet->puEleValue->i);
                break;
            case KR_TYPE_LONG: 
                kr_hashset_add(ptHashSet, &ptParamSet->puEleValue->l);
                break;
            case KR_TYPE_DOUBLE: 
                kr_hashset_add(ptHashSet, &ptParamSet->puEleValue->d);
                break;
            case KR_TYPE_STRING: 
                kr_hashset_add(ptHashSet, ptParamSet->puEleValue->s);
                break;
            default:
                KR_LOG(KR_LOGERROR, "Unknown set type[%c]!", ptHashSet->type);
                return NULL;
        }
    }
    
    return ptHashSet;
}


void kr_data_item_set_free(void *priv) 
{
    T_KRHashSet *ptHashSet = (T_KRHashSet *)priv;
    if (ptHashSet != NULL) {
        kr_hashset_destroy(ptHashSet);
    }
}


int kr_data_item_set_aggr(T_KRDataItem *ptDataItem, T_KRContext *ptContext)
{
    //TODO:set value type and value
    ptDataItem->eValueInd = KR_VALUE_SETED;

}


