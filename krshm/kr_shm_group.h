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
    char        caGroupString[500+1];
    char        caGroupFunc[50+1];
    T_KRShmRule stShmRule;
}T_KRShmGroupDef;

typedef struct _kr_shm_group_t
{
    long             lGroupDefCnt;
    T_KRShmGroupDef  stShmGroupDef[N_MAX_GROUP_NUM];
    time_t           tLastLoadTime;           /*share memory last loaded time*/
}T_KRShmGroup;


extern int LoadShmGroup(T_DbsEnv *dbsenv, T_KRShmGroup *ptShmGroup);
extern int DumpShmGroup(T_KRShmGroup *ptShmGroup, FILE *fp);

#endif  /*__KR_SHM_GROUP_H__*/
