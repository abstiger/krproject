#ifndef __KR_PARAM_HDI_H__
#define __KR_PARAM_HDI_H__

#include <stdio.h>
#include <time.h>
#include "dbs/dbs_basopr.h"

#define N_MAX_HDI_NUM  500

typedef struct _kr_param_hdi_def_t
{
    long        lHdiId;
    char        caHdiName[30+1];
    char        caHdiDesc[100+1];
    char        caHdiType[1+1];
    char        caHdiValueType[1+1];
    char        caHdiAggrFunc[50+1];
    char        caHdiFreeFunc[50+1];
    long        lStatisticsDatasrc;
    long        lStatisticsIndex;
    char        caStatisticsType[1+1];
    long        lStatisticsValue;
    char        caStatisticsMethod[1+1];
}T_KRParamHDIDef;


typedef struct _kr_param_hdi_t
{
    long               lHDIDefCnt;
    T_KRParamHDIDef    stParamHDIDef[N_MAX_HDI_NUM];
    time_t             tLastLoadTime;           /*share memory last loaded time*/
}T_KRParamHDI;


int kr_param_hdi_load(T_DbsEnv *ptDbsEnv, T_KRParamHDI *ptParamHDI);
int kr_param_hdi_dump(T_KRParamHDI *ptParamHDI, FILE *fp);


#endif  /*__KR_PARAM_HDI_H__*/
