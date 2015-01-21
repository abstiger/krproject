#include "kr_context.h"


E_KRType kr_set_get_type(int aid, T_KRContext *krcontext)
{
    if (krcontext == NULL) return KR_TYPE_UNKNOWN;
        
    long lAid = (long )aid;
    T_KRSetTable *krsettable = krcontext->ptDym->settable;
    if (krsettable == NULL) {
        KR_LOG(KR_LOGERROR, "settable is null");
        return KR_TYPE_UNKNOWN;
    }
    T_KRSet *krset = kr_hashtable_lookup(krsettable->ptSetTable, &lAid);
    if (krset == NULL) return KR_TYPE_UNKNOWN;
    
    return krset->eValueType;
}

void *kr_set_get_value(int aid, T_KRContext *krcontext)
{
    if (krcontext == NULL) return NULL;
        
    long lAID = (long )aid;
    T_KRSetTable *krsettable = krcontext->ptDym->settable;
    if (krsettable == NULL) {
        KR_LOG(KR_LOGERROR, "settable is null");
        return NULL;
    }
    T_KRSet *krset = kr_hashtable_lookup(krsettable->ptSetTable, &lAID);
    if (krset == NULL) return NULL;
    
    return krset->ptHashSet;
}
