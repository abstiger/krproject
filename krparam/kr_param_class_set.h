#ifndef __KR_PARAM_SET_H__
#define __KR_PARAM_SET_H__

#include "krutils/kr_utils.h"

#define KR_PARAM_SET "set"

typedef struct _kr_param_set_t
{
    long        lSetId;
    char        caSetName[30+1];
    char        caSetDesc[100+1];
    char        caSetUsage[2+1];
    char        caSetType[1+1];
    char        caElementType[1+1];
    long        lElementLength;
    
    long        lEleCount;
    U_KRValue   *puEleValue;
}T_KRParamSet;


char *kr_param_set_serialize(T_KRParamSet *ptParamSet);
void kr_param_set_serialize_free(char *ptParamSetString);

T_KRParamSet *kr_param_set_deserialize(char *ptParamSetString);
void kr_param_set_deserialize_free(T_KRParamSet *ptParamSet);

#endif  /*__KR_PARAM_SET_H__*/
