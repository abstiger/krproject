#ifndef __KR_SHM_DDI_H__
#define __KR_SHM_DDI_H__

#include <stdio.h>
#include <time.h>

#define N_MAX_DDI_NUM  500


typedef struct _kr_shm_ddi_def_t
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
    char        caDdiFilterString[500+1];
}T_KRShmDDIDef;

typedef struct _kr_shm_ddi_t
{
    long             lDDIDefCnt;
    T_KRShmDDIDef    stShmDDIDef[N_MAX_DDI_NUM];
    time_t           tLastLoadTime;           /*share memory last loaded time*/
}T_KRShmDDI;


int LoadShmDDI(T_KRShmDDI *ptShmDDI);
int DumpShmDDI(T_KRShmDDI *ptShmDDI, FILE *fp);

#endif  /*__KR_SHM_DDI_H__*/
