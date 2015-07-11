#ifndef __KR_DATA_ITEM_DDI_H__
#define __KR_DATA_ITEM_DDI_H__

#include "kr_data_item.h"
#include "krparam/kr_param_class_ddi.h"


typedef enum {
    KR_DDI_STATISTICS_INCLUDE     = 'I',  /*include current record*/
    KR_DDI_STATISTICS_EXCLUDE     = 'E'   /*exclude current record*/
}E_KRDDIStatisticsType;

typedef enum {
    KR_DDI_METHOD_SUM        = '0',  /*sum*/
    KR_DDI_METHOD_MIN        = '1',  /*min*/
    KR_DDI_METHOD_MAX        = '2',  /*max*/
    KR_DDI_METHOD_COUNT      = '3',  /*count*/
    KR_DDI_METHOD_CON_INC    = '4',  /*continuous increase*/
    KR_DDI_METHOD_CON_DEC    = '5',  /*continuous decrease*/
    KR_DDI_METHOD_CNT_DIS    = '6'   /*count distinct*/
}E_KRDDIMethod;

typedef struct _kr_data_item_ddi
{
    T_KRCalc              *ptFilterCalc;
    T_KRCalc              *ptResultCalc;
    
    E_KRType              eKeyType;
    void                  *pKeyValue;
    T_KRList              *ptRecList;
    T_KRHashTable         *ptRelated;
}T_KRDdi;


//function declaration
void *kr_data_item_ddi_new(T_KRDataItem *ptDataItem);
void kr_data_item_ddi_free(void *priv);
int kr_data_item_ddi_aggr(T_KRDataItem *ptDataItem, T_KRContext *ptContext);


#endif /* __KR_DATA_ITEM_DDI_H__ */
