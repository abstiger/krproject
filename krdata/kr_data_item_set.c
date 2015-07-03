#include "kr_data_item_set.h"
#include "krparam/kr_param_class_set.h"


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


int kr_data_item_set_aggr(T_KRDataItem *ptDataItem, T_KRData *ptData)
{
    //TODO:set value type and value
    ptDataItem->eValueInd = KR_VALUE_SETED;


}


static int _kr_data_item_set_load(char *psParamClassName, char *psParamObjectKey, 
        char *psParamObjectString, void *ptParamObject, void *data)
{
    T_KRParamSet *ptParamSet = (T_KRParamSet *)ptParamObject;
    T_KRHashTable *ptItemTable = (T_KRHashTable *)data;

    T_KRDataItem *ptDataItem = kr_data_item_new(
            ptParamSet,
            ptParamSet->caSetName, //FIXME
            ptParamSet->lSetId, 
            kr_data_item_set_new,
            kr_data_item_set_aggr,
            kr_data_item_set_free);
    if (ptDataItem == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_item_create [%ld] failed!", \
                ptParamSet->lSetId);
        return -1; 
    }

    //insert into item table
    kr_hashtable_replace(ptItemTable, ptDataItem->caDataItemId, ptDataItem);

    return 0;
}


int kr_data_item_set_load(T_KRHashTable *ptItemTable, T_KRParam *ptParam)
{
    //load set
    if (kr_param_object_foreach(ptParam, KR_PARAM_SET, 
                _kr_data_item_set_load, ptItemTable) != 0) {
        KR_LOG(KR_LOGERROR, "kr_data_item_set_load Error!");
        return -1;
    }
    
    return 0;
}
