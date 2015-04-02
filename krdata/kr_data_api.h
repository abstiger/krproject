#ifndef __KR_DATA_API_H__
#define __KR_DATA_API_H__

#include "kr_data.h"

cJSON *kr_set_info(T_KRSet *ptSet);
cJSON *kr_sdi_info(T_KRSDI *ptSDI);
cJSON *kr_ddi_info(T_KRDDI *ptDDI);
cJSON *kr_hdi_info(T_KRHDI *ptHDI);
cJSON *kr_data_info(T_KRData *ptData);


#endif /* __KR_DATA_API_H__ */
