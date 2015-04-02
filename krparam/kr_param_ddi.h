#ifndef __KR_PARAM_DDI_H__
#define __KR_PARAM_DDI_H__

#include <stdio.h>
#include <time.h>
#include "dbs/dbs_basopr.h"

#define N_MAX_DDI_NUM  500


typedef struct _kr_param_ddi_def_t
{
    long        lDdiId;
    char        caDdiName[30+1];
    char        caDdiDesc[100+1];
    char        caDdiType[1+1];
    char        caDdiValueType[1+1];
    char        caDdiAggrFunc[50+1];
    char        caDdiFreeFunc[50+1];
    long        lStatisticsDatasrc;
    long        lStatisticsIndex;
    long        lStatisticsField;
    char        caStatisticsType[1+1];
    long        lStatisticsValue;
    long        lStatisticsCount;
    char        caStatisticsMethod[1+1];
    char        caDdiFilterFormat[1+1];
    char        caDdiFilterString[500+1];
}T_KRParamDDIDef;

typedef struct _kr_param_ddi_t
{
    long               lDDIDefCnt;
    T_KRParamDDIDef    stParamDDIDef[N_MAX_DDI_NUM];
    time_t             tLastLoadTime;           /*share memory last loaded time*/
}T_KRParamDDI;


int kr_param_ddi_load(T_DbsEnv *ptDbsEnv, T_KRParamDDI *ptParamDDI);
int kr_param_ddi_dump(T_KRParamDDI *ptParamDDI, FILE *fp);

#endif  /*__KR_PARAM_DDI_H__*/
