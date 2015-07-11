#ifndef __KR_DATA_ITEM_SET_H__
#define __KR_DATA_ITEM_SET_H__

#include "kr_data_item.h"
#include "krparam/kr_param_class_set.h"

//function declaration
void *kr_data_item_set_new(T_KRDataItem *ptDataItem);
void kr_data_item_set_free(void *priv);
int kr_data_item_set_aggr(T_KRDataItem *ptDataItem, T_KRContext *ptContext);


#endif /* __KR_DATA_ITEM_SET_H__ */
