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
            
    memset(&ptShmBuf->stShmSDI[nBackSecId], 0, sizeof(T_KRShmSDI));
    if (LoadShmSDI(dbsenv, &ptShmBuf->stShmSDI[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "LoadShmSDI failed!");
        return -1;
    }
    
    memset(&ptShmBuf->stShmDDI[nBackSecId], 0, sizeof(T_KRShmDDI));
    if (LoadShmDDI(dbsenv, &ptShmBuf->stShmDDI[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "LoadShmDDI failed!");
        return -1;
    }

    memset(&ptShmBuf->stShmHDI[nBackSecId], 0, sizeof(T_KRShmHDI));
    if (LoadShmHDI(dbsenv, &ptShmBuf->stShmHDI[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "LoadShmHDI failed!");
        return -1;
    }
    
    memset(&ptShmBuf->stShmGroup[nBackSecId], 0, sizeof(T_KRShmGroup));
    if (LoadShmGroup(dbsenv, &ptShmBuf->stShmGroup[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "LoadShmGroup failed!");
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
    DumpShmSDI(&ptShmBuf->stShmSDI[nCurrSecId], fp);
    DumpShmDDI(&ptShmBuf->stShmDDI[nCurrSecId], fp);
    DumpShmGroup(&ptShmBuf->stShmGroup[nCurrSecId], fp);

    fprintf(fp, "\n****Dump Standby******\n");
    DumpShmSDI(&ptShmBuf->stShmSDI[nBackSecId], fp);
    DumpShmDDI(&ptShmBuf->stShmDDI[nBackSecId], fp);
    DumpShmGroup(&ptShmBuf->stShmGroup[nBackSecId], fp);
    
    fflush(fp);
}
