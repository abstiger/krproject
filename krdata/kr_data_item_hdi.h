#ifndef __KR_DATA_ITEM_HDI_H__
#define __KR_DATA_ITEM_HDI_H__

#include "kr_data_item.h"
#include "krparam/kr_param_class_hdi.h"


#define KR_HDI_OBJECT_LEN 30

typedef enum {
    KR_HDI_STATISTICS_DAY     = '0',  /*day's statistics*/
    KR_HDI_STATISTICS_MONTH   = '1',  /*month's statistics*/
    KR_HDI_STATISTICS_FLAG    = '2'   /*flag's statistics*/
}E_KRHDIStatisticsType;

typedef enum {
    KR_HDI_METHOD_SUM        = '0',  /*sum*/
    KR_HDI_METHOD_MIN        = '1',  /*min*/
    KR_HDI_METHOD_MAX        = '2',  /*max*/
    KR_HDI_METHOD_COUNT      = '3',  /*count*/
    KR_HDI_METHOD_FLAG       = 'F'   /*flag*/
}E_KRHDIMethod;

typedef struct _kr_data_item_hdi
{
    E_KRType              eKeyType;
    void                  *pKeyValue;
}T_KRHdi;

//function declaration
void *kr_data_item_hdi_new(T_KRDataItem *ptDataItem);
void kr_data_item_hdi_free(void *priv);
int kr_data_item_hdi_aggr(T_KRDataItem *ptDataItem, T_KRContext *ptContext);


#endif /* __KR_DATA_ITEM_HDI_H__ */
