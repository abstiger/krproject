#ifndef __KR_SHM_H__
#define __KR_SHM_H__

#include <stdio.h>
#include <time.h>
#include "dbs/dbs_basopr.h"
#include "kr_shm_sdi.h"
#include "kr_shm_ddi.h"
#include "kr_shm_hdi.h"
#include "kr_shm_rule.h"
#include "kr_shm_group.h"

#define N_MAX_SEC  2

/*struct definition of share memory*/
typedef struct _kr_sharemem_t
{
    short            nSecId;                  /*current section indicator*/
    time_t           tLastLoadTime;           /*share memory last loaded time*/
    T_KRShmSDI       stShmSDI[N_MAX_SEC];
    T_KRShmDDI       stShmDDI[N_MAX_SEC];
    T_KRShmHDI       stShmHDI[N_MAX_SEC];
    T_KRShmGroup     stShmGroup[N_MAX_SEC];
}T_KRShareMem;



T_KRShareMem *kr_shm_create(T_DbsEnv *dbsenv);
void kr_shm_destroy(T_KRShareMem *ptShmBuf);
short kr_shm_switch(T_KRShareMem *ptShmBuf);
short kr_shm_load(T_DbsEnv *dbsenv, T_KRShareMem *ptShmBuf);
void kr_shm_dump(T_KRShareMem *ptShmBuf, FILE *fp);

#endif  /*__KR_SHM_H__*/
