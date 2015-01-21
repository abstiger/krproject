#ifndef __KR_SHM_GROUP_H__
#define __KR_SHM_GROUP_H__

#include <stdio.h>
#include <time.h>
#include "dbs/dbs_basopr.h"
#include "kr_shm_rule.h"

#define N_MAX_GROUP_NUM 20

typedef struct _kr_shm_group_def_t
{   
    long        lGroupId;
    char        caGroupName[30+1];
    char        caGroupDesc[100+1];
    char        caGroupCalcFormat[1+1];
    char        caGroupCalcString[500+1];
    char        caGroupFunc[50+1];
    long        lGroupBlockLeft;
    long        lGroupBlockRight;
    long        lGroupAlertLeft;
    long        lGroupAlertRight;
    T_KRShmRule stShmRule;
}T_KRShmGroupDef;

typedef struct _kr_shm_group_t
{
    long             lGroupDefCnt;
    T_KRShmGroupDef  stShmGroupDef[N_MAX_GROUP_NUM];
    time_t           tLastLoadTime;           /*share memory last loaded time*/
}T_KRShmGroup;


int kr_shm_group_load(T_DbsEnv *dbsenv, T_KRShmGroup *ptShmGroup);
int kr_shm_group_dump(T_KRShmGroup *ptShmGroup, FILE *fp);
cJSON *kr_shm_group_info(T_KRShmGroupDef *ptShmGroupDef);

#endif  /*__KR_SHM_GROUP_H__*/
