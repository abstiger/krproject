#ifndef __KR_DATA_ITEM_SDI_H__
#define __KR_DATA_ITEM_SDI_H__

#include "kr_data_item.h"

typedef enum {
    KR_LOC_ABSOLUTE     = '0', 
    KR_LOC_RELATIVE     = '1'  
}E_KRLocProp;

typedef struct _kr_data_item_sdi
{
    T_KRCalc              *ptFilterCalc;
    T_KRCalc              *ptResultCalc;
    
    E_KRType              eKeyType;
    void                  *pKeyValue;
    T_KRList              *ptRecList;
    T_KRHashTable         *ptRelated;
}T_KRSdi;

//function declaration
void *kr_data_item_sdi_new(T_KRDataItem *ptDataItem);
void kr_data_item_sdi_free(void *priv);
int kr_data_item_sdi_aggr(T_KRDataItem *ptDataItem, T_KRData *ptData);
int kr_data_item_sdi_load(T_KRHashTable *ptItemTable, T_KRParam *ptParam);


#endif /* __KR_DATA_ITEM_SDI_H__ */
