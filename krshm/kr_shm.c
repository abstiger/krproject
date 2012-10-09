#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include "krutils/kr_utils.h"
#include "kr_shm.h"


#define SHM_PERM  0600


/** 
 *  @brief 欺诈侦测共享内存的创建
 *  @details 根据配置文件创建共享内存
 *  @return 函数处理结果
 *  @retval   0: 成功
 *  @retval  -1：失败
 */
T_KRShareMem *kr_shm_create(int shmkey)
{
    int iShmId;
    key_t tShmKey = (key_t)shmkey;
    T_KRShareMem *ptShmBuf = NULL;
    
    iShmId = shmget(tShmKey, sizeof(T_KRShareMem), SHM_PERM|IPC_EXCL|IPC_CREAT);
    if (iShmId == -1) {
        KR_LOG(KR_LOGERROR, "shmget ptShmBuf failed[%s]!", strerror(errno));
        return NULL;
    }
    
    ptShmBuf = (T_KRShareMem *)shmat(iShmId, 0, 0);
    if (((long)ptShmBuf) == -1L) {
        KR_LOG(KR_LOGERROR, "shmat ptShmBuf failed[%s]!", strerror(errno));
        return NULL;
    }
    
    ptShmBuf->nSecId = 0;

    return ptShmBuf;
}


/** 
 *  @brief 欺诈侦测共享内存的连接
 *  @details 根据配置文件app.ini中的IPCKEY连接共享内存块
 *  @return 函数处理结果
 *  @retval   0: 成功
 *  @retval  -1：失败
 */
T_KRShareMem *kr_shm_attach(int shmkey)
{
    int iShmId;
    key_t tShmKey = (key_t )shmkey;
    T_KRShareMem *ptShmBuf = NULL;

    if ((iShmId = shmget(tShmKey, sizeof(T_KRShareMem), SHM_PERM)) == -1) {
        KR_LOG(KR_LOGERROR, "shmget ptShmBuf failed[%s]!", strerror(errno));
        return NULL;
    }
    
    ptShmBuf = (T_KRShareMem *)shmat(iShmId, 0, 0);
    if (((long)ptShmBuf) == -1L) {
        KR_LOG(KR_LOGERROR, "shmat ptShmBuf failed[%s]!", strerror(errno));
        return NULL;
    }
    
    return ptShmBuf;
}

/** 
 *  @brief 欺诈侦测共享内存的断开
 *  @details 断开共享内存块
 *  @return 函数处理结果
 *  @retval   0: 成功
 *  @retval  -1：失败
 */
void kr_shm_detach(T_KRShareMem *ptShmBuf)
{
    shmdt((void*)ptShmBuf);
    ptShmBuf = NULL;
}


/** 
 *  @brief 欺诈侦测共享内存的删除
 *  @details 根据配置文件app.ini中的IPCKEY删除共享内存块
 *  @return 函数处理结果
 *  @retval   0: 成功
 *  @retval  -1：失败
 */
int kr_shm_destroy(int shmkey)
{
    int     iShmId;
    key_t   tShmKey = (key_t)shmkey;

    iShmId = shmget(tShmKey, sizeof(T_KRShareMem), SHM_PERM| IPC_CREAT);
    if (iShmId == -1) {
        KR_LOG(KR_LOGERROR, "shmget ptShmBuf failed[%s]!", strerror(errno));
        return -1;
    }
    
    if (shmctl(iShmId, IPC_RMID, 0) < 0) {
        KR_LOG(KR_LOGERROR, "shmat ptShmBuf failed[%s]!", strerror(errno));
        return -1;
    }
    
    return 0;
}


/** 
 *  @brief 欺诈侦测共享内存切换
 *  @details 切换当前共享内存的主备块
 *  @return 无
 */
short kr_shm_switch(T_KRShareMem *ptShmBuf)
{
    short nSecId = -1;
    
    nSecId = (ptShmBuf->nSecId+1)%N_MAX_SEC;
    ptShmBuf->nSecId = nSecId;
    
    return nSecId;
}


/** 
 *  @brief 加载共享内存所有数据
 *  @param[in]  无
 *  @param[out] 无
 *  @return 函数处理结果
 *  @retval   0: 成功
 *  @retval  -1: 失败
 */
short kr_shm_load(T_KRShareMem *ptShmBuf)
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
            
    /// 共享内存备块导入静态数据项
    memset(&ptShmBuf->stShmSDI[nBackSecId], 0, sizeof(T_KRShmSDI));
    if (LoadShmSDI(&ptShmBuf->stShmSDI[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "LoadShmSDI failed!");
        return -1;
    }
    
    /// 共享内存备块导入动态统计量
    memset(&ptShmBuf->stShmDDI[nBackSecId], 0, sizeof(T_KRShmDDI));
    if (LoadShmDDI(&ptShmBuf->stShmDDI[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "LoadShmDDI failed!");
        return -1;
    }

    /// 共享内存备块导入历史统计量
    memset(&ptShmBuf->stShmHDI[nBackSecId], 0, sizeof(T_KRShmHDI));
    if (LoadShmHDI(&ptShmBuf->stShmHDI[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "LoadShmHDI failed!");
        return -1;
    }
    
    /// 共享内存备块导入规则信息
    memset(&ptShmBuf->stShmRule[nBackSecId], 0, sizeof(T_KRShmRule));
    if (LoadShmRule(&ptShmBuf->stShmRule[nBackSecId]) != 0) {
        KR_LOG(KR_LOGERROR, "LoadShmRule failed!");
        return -1;
    }

    /// 设置加载时间
    ptShmBuf->tLastLoadTime = time(NULL);

    /// 共享内存主备块切换
    nSecId = kr_shm_switch(ptShmBuf);
    
    return nSecId;
}


/** 
 *  @brief 导出共享内存所有数据
 *  @param[in]  无
 *  @param[out] 无
 *  @return 函数处理结果
 *  @retval   0: 成功
 *  @retval  -1: 失败
 */
void kr_shm_dump(T_KRShareMem *ptShmBuf, FILE *fp)
{
    short nCurrSecId = ptShmBuf->nSecId;
    short nBackSecId = (ptShmBuf->nSecId+1)%N_MAX_SEC;

    fprintf(fp, "Dumping Share-Memory:Size[%d], Active[%d], Standby[%d]...", \
            sizeof(*ptShmBuf), nCurrSecId, nBackSecId);
    
    fprintf(fp, "\n******Dump Active******\n");
    DumpShmSDI(&ptShmBuf->stShmSDI[nCurrSecId], fp);
    DumpShmDDI(&ptShmBuf->stShmDDI[nCurrSecId], fp);
    DumpShmRule(&ptShmBuf->stShmRule[nCurrSecId], fp);

    fprintf(fp, "\n****Dump Standby******\n");
    DumpShmSDI(&ptShmBuf->stShmSDI[nBackSecId], fp);
    DumpShmDDI(&ptShmBuf->stShmDDI[nBackSecId], fp);
    DumpShmRule(&ptShmBuf->stShmRule[nBackSecId], fp);
    
    fflush(fp);
}