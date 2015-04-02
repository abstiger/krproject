#ifndef __KR_PARAM_GROUP_H__
#define __KR_PARAM_GROUP_H__

#include <stdio.h>
#include <time.h>
#include "dbs/dbs_basopr.h"
#include "kr_param_rule.h"

#define N_MAX_GROUP_NUM 20

typedef struct _kr_param_group_def_t
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
    T_KRParamRule stParamRule;
}T_KRParamGroupDef;

typedef struct _kr_param_group_t
{
    long               lGroupDefCnt;
    T_KRParamGroupDef  stParamGroupDef[N_MAX_GROUP_NUM];
    time_t             tLastLoadTime;           /*share memory last loaded time*/
}T_KRParamGroup;


int kr_param_group_load(T_DbsEnv *ptDbsEnv, T_KRParamGroup *ptParamGroup);
int kr_param_group_dump(T_KRParamGroup *ptParamGroup, FILE *fp);

#endif  /*__KR_PARAM_GROUP_H__*/
