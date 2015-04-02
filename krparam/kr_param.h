#ifndef __KR_PARAM_H__
#define __KR_PARAM_H__

#include <stdio.h>
#include <time.h>
#include "kr_param_set.h"
#include "kr_param_sdi.h"
#include "kr_param_ddi.h"
#include "kr_param_hdi.h"
#include "kr_param_rule.h"
#include "kr_param_group.h"
#include "dbs/dbs_basopr.h"

#define N_MAX_SEC  2

/*struct definition of share memory*/
typedef struct _kr_param_t
{
    short              nSecId;                  /*current section indicator*/
    time_t             tLastLoadTime;           /*share memory last loaded time*/
    T_DbsEnv           *ptDbsEnv;
    T_KRParamSet       stParamSet[N_MAX_SEC];
    T_KRParamSDI       stParamSDI[N_MAX_SEC];
    T_KRParamDDI       stParamDDI[N_MAX_SEC];
    T_KRParamHDI       stParamHDI[N_MAX_SEC];
    T_KRParamGroup     stParamGroup[N_MAX_SEC];
}T_KRParam;


T_KRParam *kr_param_create(T_DbsEnv *ptDbsEnv);
void kr_param_destroy(T_KRParam *ptParam);
short kr_param_switch(T_KRParam *ptParam);
short kr_param_load(T_KRParam *ptParam);
void kr_param_dump(T_KRParam *ptParam, FILE *fp);

#endif  /*__KR_PARAM_H__*/
