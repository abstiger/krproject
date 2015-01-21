#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"
#include "kr_shm.h"


T_KRShareMem *kr_shm_create(T_DbsEnv *dbsenv)
{
    T_KRShareMem *ptShmBuf = kr_calloc(sizeof(T_KRShareMem));
    if (ptShmBuf == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptShmBuf failed!");
        return NULL;
    }

    /* load share memory */
    if (kr_shm_load(dbsenv, ptShmBuf) < 0) {
        KR_LOG(KR_LOGERROR, "kr_shm_load ptShmBuf failed!");
        return NULL;
    }

    return ptShmBuf;
}


void kr_shm_destroy(T_KRShareMem *ptShmBuf)
{
    kr_free(ptShmBuf);
}


short kr_shm_switch(T_KRShareMem *ptShmBuf)
{
    short nSecId = -1;
    
    nSecId = (ptShmBuf->nSecId+1)%N_MAX_SEC;
    ptShmBuf->nSecId = nSecId;
    
    return nSecId;
}


short kr_shm_load(T_DbsEnv *dbsenv, T_KRShareMem *ptShmBuf)
{
    short       nSecId = -1;
    short       nCurrSecId;
    short       nBackSecId;
    
    if(ptShmBuf == NULL) {
        KR_LOG(KR_LOGERROR, "ptShmBuf is NULL!\n");
        return -1;
    }
     
    nCurrSecId = ptShmBuf->nSecId;
    nBackSecId = (ptShmBuf->nSecId+1)%N_MAX_SEC;
            
    memset(&ptShmBuf->stShmSet[nBackSecId], 0, sizeof(T_KRShmSet));
    if (kr_shm_set_load(dbsenv, &ptShmBuf->stShmSet[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "kr_shm_set_load failed!");
        return -1;
    }

    memset(&ptShmBuf->stShmSDI[nBackSecId], 0, sizeof(T_KRShmSDI));
    if (kr_shm_sdi_load(dbsenv, &ptShmBuf->stShmSDI[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "kr_shm_sdi_load failed!");
        return -1;
    }
    
    memset(&ptShmBuf->stShmDDI[nBackSecId], 0, sizeof(T_KRShmDDI));
    if (kr_shm_ddi_load(dbsenv, &ptShmBuf->stShmDDI[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "kr_shm_ddi_load failed!");
        return -1;
    }

    memset(&ptShmBuf->stShmHDI[nBackSecId], 0, sizeof(T_KRShmHDI));
    if (kr_shm_hdi_load(dbsenv, &ptShmBuf->stShmHDI[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "kr_shm_hdi_load failed!");
        return -1;
    }
    
    memset(&ptShmBuf->stShmGroup[nBackSecId], 0, sizeof(T_KRShmGroup));
    if (kr_shm_group_load(dbsenv, &ptShmBuf->stShmGroup[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "kr_shm_group_load failed!");
        return -1;
    }

    ptShmBuf->tLastLoadTime = time(NULL);

    nSecId = kr_shm_switch(ptShmBuf);
    
    return nSecId;
}


void kr_shm_dump(T_KRShareMem *ptShmBuf, FILE *fp)
{
    short nCurrSecId = ptShmBuf->nSecId;
    short nBackSecId = (ptShmBuf->nSecId+1)%N_MAX_SEC;

    fprintf(fp, "Dumping Share-Memory:Size[%ld], Active[%d], Standby[%d]...", \
            sizeof(*ptShmBuf), nCurrSecId, nBackSecId);
    
    fprintf(fp, "\n******Dump Active******\n");
    kr_shm_set_dump(&ptShmBuf->stShmSet[nCurrSecId], fp);
    kr_shm_sdi_dump(&ptShmBuf->stShmSDI[nCurrSecId], fp);
    kr_shm_ddi_dump(&ptShmBuf->stShmDDI[nCurrSecId], fp);
    kr_shm_hdi_dump(&ptShmBuf->stShmHDI[nCurrSecId], fp);
    kr_shm_group_dump(&ptShmBuf->stShmGroup[nCurrSecId], fp);

    fprintf(fp, "\n****Dump Standby******\n");
    kr_shm_set_dump(&ptShmBuf->stShmSet[nCurrSecId], fp);
    kr_shm_sdi_dump(&ptShmBuf->stShmSDI[nBackSecId], fp);
    kr_shm_ddi_dump(&ptShmBuf->stShmDDI[nBackSecId], fp);
    kr_shm_hdi_dump(&ptShmBuf->stShmHDI[nBackSecId], fp);
    kr_shm_group_dump(&ptShmBuf->stShmGroup[nBackSecId], fp);
    
    fflush(fp);
}


cJSON *kr_shm_info(T_KRShareMem *ptShm)
{
    cJSON *shm = cJSON_CreateObject();

    short nCurrSecId = ptShm->nSecId;
    cJSON_AddNumberToObject(shm, "current_secid", nCurrSecId);

    cJSON_AddNumberToObject(shm, "set_load_time", 
            (long )ptShm->stShmSet[nCurrSecId].tLastLoadTime);
    cJSON_AddNumberToObject(shm, "sdi_load_time", 
            (long )ptShm->stShmSDI[nCurrSecId].tLastLoadTime);
    cJSON_AddNumberToObject(shm, "ddi_load_time", 
            (long )ptShm->stShmDDI[nCurrSecId].tLastLoadTime);
    cJSON_AddNumberToObject(shm, "hdi_load_time", 
            (long )ptShm->stShmHDI[nCurrSecId].tLastLoadTime);
    cJSON_AddNumberToObject(shm, "group_load_time", 
            (long )ptShm->stShmGroup[nCurrSecId].tLastLoadTime);

    return shm;
}
