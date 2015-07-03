#ifndef __KR_DATA_TEMP_H__
#define __KR_DATA_TEMP_H__

#include "krutils/kr_utils.h"
#include "kr_data.h"

typedef struct _kr_data_temp_t
{
    char                  caDataTempId[20+1];
    
    E_KRType              eValueType;
    U_KRValue             uValue;
}T_KRDataTemp;


//function declaration
T_KRDataTemp *kr_data_temp_new(char *psDataTempId, E_KRType eValueType, U_KRValue uValue);
void kr_data_temp_free(T_KRDataTemp *ptDataTemp);
E_KRType kr_data_temp_type(char *psDataTempId, T_KRData *ptData);
void *kr_data_temp_value(char *psDataTempId, T_KRData *ptData);


#endif /* __KR_DATA_TEMP_H__ */
