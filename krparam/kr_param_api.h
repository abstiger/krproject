#ifndef __KR_PARAM_API_H__
#define __KR_PARAM_API_H__

#include "krutils/kr_utils.h"
#include "kr_param.h"

cJSON *kr_param_set_info(T_KRParamSetDef *ptParamSetDef);
cJSON *kr_param_sdi_info(T_KRParamSDIDef *ptParamSDIDef);
cJSON *kr_param_ddi_info(T_KRParamDDIDef *ptParamDDIDef);
cJSON *kr_param_hdi_info(T_KRParamHDIDef *ptParamHDIDef); 
cJSON *kr_param_rule_info(T_KRParamRuleDef *ptParamRuleDef);
cJSON *kr_param_group_info(T_KRParamGroupDef *ptParamGroupDef);
cJSON *kr_param_info(T_KRParam *ptParam);


#endif  /*__KR_PARAM_API_H__*/
