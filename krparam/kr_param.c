#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"
#include "kr_param.h"


T_KRParam *kr_param_create(T_DbsEnv *ptDbsEnv)
{
    T_KRParam *ptParam = kr_calloc(sizeof(T_KRParam));
    if (ptParam == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptParam failed!");
        return NULL;
    }
    ptParam->ptDbsEnv = ptDbsEnv;

    /* load share memory */
    if (kr_param_load(ptParam) < 0) {
        KR_LOG(KR_LOGERROR, "kr_param_load ptParam failed!");
        return NULL;
    }

    return ptParam;
}


void kr_param_destroy(T_KRParam *ptParam)
{
    kr_free(ptParam);
}


short kr_param_switch(T_KRParam *ptParam)
{
    short nSecId = -1;
    
    nSecId = (ptParam->nSecId+1)%N_MAX_SEC;
    ptParam->nSecId = nSecId;
    
    return nSecId;
}


short kr_param_load(T_KRParam *ptParam)
{
    if(ptParam == NULL) {
        KR_LOG(KR_LOGERROR, "ptParam is NULL!\n");
        return -1;
    }
    
    T_DbsEnv *ptDbsEnv = ptParam->ptDbsEnv;
    short       nSecId = -1;
    short nCurrSecId = ptParam->nSecId;
    short nBackSecId = (ptParam->nSecId+1)%N_MAX_SEC;
            
    memset(&ptParam->stParamSet[nBackSecId], 0, sizeof(T_KRParamSet));
    if (kr_param_set_load(ptDbsEnv, &ptParam->stParamSet[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "kr_param_set_load failed!");
        return -1;
    }

    memset(&ptParam->stParamSDI[nBackSecId], 0, sizeof(T_KRParamSDI));
    if (kr_param_sdi_load(ptDbsEnv, &ptParam->stParamSDI[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "kr_param_sdi_load failed!");
        return -1;
    }
    
    memset(&ptParam->stParamDDI[nBackSecId], 0, sizeof(T_KRParamDDI));
    if (kr_param_ddi_load(ptDbsEnv, &ptParam->stParamDDI[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "kr_param_ddi_load failed!");
        return -1;
    }

    memset(&ptParam->stParamHDI[nBackSecId], 0, sizeof(T_KRParamHDI));
    if (kr_param_hdi_load(ptDbsEnv, &ptParam->stParamHDI[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "kr_param_hdi_load failed!");
        return -1;
    }
    
    memset(&ptParam->stParamGroup[nBackSecId], 0, sizeof(T_KRParamGroup));
    if (kr_param_group_load(ptDbsEnv, &ptParam->stParamGroup[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "kr_param_group_load failed!");
        return -1;
    }

    ptParam->tLastLoadTime = time(NULL);

    nSecId = kr_param_switch(ptParam);
    
    return nSecId;
}


void kr_param_dump(T_KRParam *ptParam, FILE *fp)
{
    short nCurrSecId = ptParam->nSecId;
    short nBackSecId = (ptParam->nSecId+1)%N_MAX_SEC;

    fprintf(fp, "Dumping Share-Memory:Size[%ld], Active[%d], Standby[%d]...", \
            sizeof(*ptParam), nCurrSecId, nBackSecId);
    
    fprintf(fp, "\n******Dump Active******\n");
    kr_param_set_dump(&ptParam->stParamSet[nCurrSecId], fp);
    kr_param_sdi_dump(&ptParam->stParamSDI[nCurrSecId], fp);
    kr_param_ddi_dump(&ptParam->stParamDDI[nCurrSecId], fp);
    kr_param_hdi_dump(&ptParam->stParamHDI[nCurrSecId], fp);
    kr_param_group_dump(&ptParam->stParamGroup[nCurrSecId], fp);

    fprintf(fp, "\n****Dump Standby******\n");
    kr_param_set_dump(&ptParam->stParamSet[nCurrSecId], fp);
    kr_param_sdi_dump(&ptParam->stParamSDI[nBackSecId], fp);
    kr_param_ddi_dump(&ptParam->stParamDDI[nBackSecId], fp);
    kr_param_hdi_dump(&ptParam->stParamHDI[nBackSecId], fp);
    kr_param_group_dump(&ptParam->stParamGroup[nBackSecId], fp);
    
    fflush(fp);
}


