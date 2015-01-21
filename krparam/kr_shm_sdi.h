#ifndef __KR_SHM_SDI_H__
#define __KR_SHM_SDI_H__

#include <stdio.h>
#include <time.h>
#include "dbs/dbs_basopr.h"

#define N_MAX_SDI_NUM  500

typedef struct _kr_shm_sdi_def_t
{
    long        lSdiId;
    char        caSdiName[30+1];
    char        caSdiDesc[100+1];
    char        caSdiType[1+1];
    char        caSdiValueType[1+1];
    char        caSdiAggrFunc[50+1];
    char        caSdiFreeFunc[50+1];
    long        lStatisticsDatasrc;
    long        lStatisticsIndex;
    long        lStatisticsField;
    long        lStatisticsLocation;
    char        caLocationProperty[1+1];
    char        caSdiFilterFormat[1+1];
    char        caSdiFilterString[500+1];
}T_KRShmSDIDef;

typedef struct _kr_shm_sdi_t
{
    long             lSDIDefCnt;
    T_KRShmSDIDef    stShmSDIDef[N_MAX_SDI_NUM];
    time_t           tLastLoadTime;           /*share memory last loaded time*/
}T_KRShmSDI;


int kr_shm_sdi_load(T_DbsEnv *dbsenv, T_KRShmSDI *ptShmSDI);
int kr_shm_sdi_dump(T_KRShmSDI *ptShmSDI, FILE *fp);
cJSON *kr_shm_sdi_info(T_KRShmSDIDef *ptShmSDIDef);

#endif  /*__KR_SHM_SDI_H__*/
