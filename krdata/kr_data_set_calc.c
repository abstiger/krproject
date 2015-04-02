#include "kr_data.h"

E_KRType kr_set_get_type(int aid, T_KRData *ptData)
{
    if (ptData == NULL) return KR_TYPE_UNKNOWN;
        
    long lAid = (long )aid;
    T_KRSetTable *ptSetTable = ptData->ptSetTable;
    if (ptSetTable == NULL) {
        KR_LOG(KR_LOGERROR, "ptSetTable is null");
        return KR_TYPE_UNKNOWN;
    }
    T_KRSet *ptSet = kr_hashtable_lookup(ptSetTable->ptSetTable, &lAid);
    if (ptSet == NULL) return KR_TYPE_UNKNOWN;
    
    return ptSet->eValueType;
}

void *kr_set_get_value(int aid, T_KRData *ptData)
{
    if (ptData == NULL) return NULL;
        
    long lAID = (long )aid;
    T_KRSetTable *ptSetTable = ptData->ptSetTable;
    if (ptSetTable == NULL) {
        KR_LOG(KR_LOGERROR, "ptSetTable is null");
        return NULL;
    }
    T_KRSet *ptSet = kr_hashtable_lookup(ptSetTable->ptSetTable, &lAID);
    if (ptSet == NULL) return NULL;
    
    return ptSet->ptHashSet;
}
