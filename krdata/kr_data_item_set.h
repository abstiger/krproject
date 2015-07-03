#ifndef __KR_DATA_ITEM_SET_H__
#define __KR_DATA_ITEM_SET_H__

#include "kr_data_item.h"

//function declaration
void *kr_data_item_set_new(T_KRDataItem *ptDataItem);
void kr_data_item_set_free(void *priv);
int kr_data_item_set_aggr(T_KRDataItem *ptDataItem, T_KRData *ptData);
int kr_data_item_set_load(T_KRHashTable *ptItemTable, T_KRParam *ptParam);


#endif /* __KR_DATA_ITEM_SET_H__ */
