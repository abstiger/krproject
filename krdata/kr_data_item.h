#ifndef __KR_DATA_ITEM_H__
#define __KR_DATA_ITEM_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"
#include "kr_data.h"

typedef struct _kr_data_item_t T_KRDataItem;

typedef void* (*KRDataItemNewFunc)(T_KRDataItem *ptDataItem);
typedef int (*KRDataItemAggrFunc)(T_KRDataItem *ptDataItem, T_KRData *ptData);
typedef void (*KRDataItemFreeFunc)(void *priv);

struct _kr_data_item_t
{
    void                  *ptDataItemDef;
    char                  cDataItemKind;
    long                  lDataItemId;
    KRDataItemNewFunc     pfDataItemNew;
    KRDataItemAggrFunc    pfDataItemAggr;
    KRDataItemFreeFunc    pfDataItemFree;

    char                  caDataItemId[20+1]; /*hash key:("%c_%d", kind, id) */
    T_KRRecord            *ptCurrRec;

    E_KRValueInd          eValueInd;
    E_KRType              eValueType;
    U_KRValue             uValue;

    void                  *ptPrivate;
};

//function declaration
T_KRDataItem *kr_data_item_new(void *ptDataItemDef, char kind, int id,
        KRDataItemNewFunc pfDataItemNew, 
        KRDataItemAggrFunc pfDataItemAggr, 
        KRDataItemFreeFunc pfDataItemFree);
void kr_data_item_free(T_KRDataItem *ptDataItem);
void kr_data_item_init(T_KRDataItem *ptDataItem);
int kr_data_item_load(T_KRHashTable *ptItemTable, T_KRParam *ptParam);

E_KRType kr_data_item_type(char kind, int id, T_KRData *ptData);
void *kr_data_item_value(char kind, int id, T_KRData *ptData);

#endif /* __KR_DATA_ITEM_H__ */
