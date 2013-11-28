#ifndef __KR_SHM_SET_H__
#define __KR_SHM_SET_H__

#include <stdio.h>
#include <time.h>
#include "dbs/dbs_basopr.h"

#define N_MAX_SET_NUM  500


typedef struct _kr_shm_set_def_t
{
    long        lSetId;
    char        caSetName[30+1];
    char        caSetDesc[100+1];
    char        caSetUsage[2+1];
    char        caSetType[1+1];
    char        caElementType[1+1];
    long        lElementLength;
}T_KRShmSetDef;

typedef struct _kr_shm_set_t
{
    long             lSetDefCnt;
    T_KRShmSetDef    stShmSetDef[N_MAX_SET_NUM];
    time_t           tLastLoadTime;           /*share memory last loaded time*/
}T_KRShmSet;


int LoadShmSet(T_DbsEnv *dbsenv, T_KRShmSet *ptShmSet);
int DumpShmSet(T_KRShmSet *ptShmSet, FILE *fp);

#endif  /*__KR_SHM_SET_H__*/
