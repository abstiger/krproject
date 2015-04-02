#ifndef __KR_PARAM_SET_H__
#define __KR_PARAM_SET_H__

#include <stdio.h>
#include <time.h>
#include "dbs/dbs_basopr.h"

#define N_MAX_SET_NUM  500


typedef struct _kr_param_set_def_t
{
    long        lSetId;
    char        caSetName[30+1];
    char        caSetDesc[100+1];
    char        caSetUsage[2+1];
    char        caSetType[1+1];
    char        caElementType[1+1];
    long        lElementLength;
}T_KRParamSetDef;

typedef struct _kr_param_set_t
{
    long               lSetDefCnt;
    T_KRParamSetDef    stParamSetDef[N_MAX_SET_NUM];
    time_t             tLastLoadTime;           /*share memory last loaded time*/
}T_KRParamSet;


int kr_param_set_load(T_DbsEnv *ptDbsEnv, T_KRParamSet *ptParamSet);
int kr_param_set_dump(T_KRParamSet *ptParamSet, FILE *fp);

#endif  /*__KR_PARAM_SET_H__*/
