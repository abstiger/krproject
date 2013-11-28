#ifndef __KR_DATA_H__
#define __KR_DATA_H__

#include "kr_context.h"
extern int kr_group_list_route(T_KRContext *krcontext);
extern int kr_rule_list_detect(T_KRRuleList *ptRuleList, T_KRContext *krcontext);


extern E_KRType kr_data_get_type(char kind, int id, void *param);
extern void *kr_data_get_value(char kind, int id, void *param);


#endif /* __KR_DATA_H__ */

