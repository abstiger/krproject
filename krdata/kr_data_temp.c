#include "kr_data_temp.h"

/*data temp*/
T_KRDataTemp *kr_data_temp_new(char *psDataTempId, E_KRType eValueType, U_KRValue uValue)
{
    T_KRDataTemp *ptDataTemp = kr_calloc(sizeof(T_KRDataTemp));
    if (ptDataTemp == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptDataTemp failed!");
        return NULL;
    }
    strncpy(ptDataTemp->caDataTempId, psDataTempId, sizeof(ptDataTemp->caDataTempId));
    ptDataTemp->eValueType = eValueType;
    ptDataTemp->uValue = uValue;
    
    return ptDataTemp;
}


void kr_data_temp_free(T_KRDataTemp *ptDataTemp)
{
    if (ptDataTemp) {
        if (ptDataTemp->eValueType == KR_TYPE_STRING) {
            kr_free(ptDataTemp->uValue.s);
        }
        kr_free(ptDataTemp);
    }
}


static T_KRDataTemp *kr_data_temp_get(char *psDataTempId, T_KRData *ptData)
{
    T_KRDataTemp *ptDataTemp = kr_hashtable_lookup(ptData->ptTempTable, psDataTempId);
    
    return ptDataTemp;
}


E_KRType kr_data_temp_type(char *psDataTempId, T_KRData *ptData)
{
    T_KRDataTemp *ptDataTemp = kr_data_temp_get(psDataTempId, ptData);
    if (ptDataTemp == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_temp_get [%d] failed!", psDataTempId);
        return KR_TYPE_UNKNOWN;
    }
    
    return ptDataTemp->eValueType;
}


void *kr_data_temp_value(char *psDataTempId, T_KRData *ptData)
{
    T_KRDataTemp *ptDataTemp = kr_data_temp_get(psDataTempId, ptData);
    if (ptDataTemp == NULL) {
        KR_LOG(KR_LOGERROR, "kr_data_temp_get [%d] failed!", psDataTempId);
        return NULL;
    }
    
    return kr_get_value(&ptDataTemp->uValue, ptDataTemp->eValueType);
}
