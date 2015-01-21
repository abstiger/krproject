#ifndef __KR_DATA_H__
#define __KR_DATA_H__

#include "kr_context.h"

extern E_KRType kr_data_get_type(char kind, int id, void *param);
extern void *kr_data_get_value(char kind, int id, void *param);


#endif /* __KR_DATA_H__ */

