#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>

#include "krshm/kr_shm.h"

short            gnSecId = 0;
T_KRShareMem     *gptShmBuf = NULL;

#define N_SHM_HELP          (1<<0)
#define N_SHM_CREATE        (1<<1)
#define N_SHM_LOAD          (1<<2)
#define N_SHM_VIEW          (1<<3)
#define N_SHM_REMOVE        (1<<4)


static void DumpAllMemory(void);
static int LoadAllMemory(void);
static int ViewMemory(void);
static int ViewSDIMemory(char cInd);
static int ViewDDIMemory(char cInd);
static int ViewHDIMemory(char cInd);
static int ViewRuleMemory(char cInd);
static void usage(void);
static int kr_get_shmkey(void);
static int kr_get_detectmode(void);


/** 
 *  @brief 欺诈侦测共享内存操作main函数
 *  @param[in] argc 参数个数
 *  @param[in] argv 参数指针
 *  @return 函数处理结果
 *  @retval   0: 成功
 *  @retval  -1: 失败
 */
int main(int argc,char* argv[])
{
    int             ch;
    int             iInputFlag=0;
    int             iShmKey=0;
    
    /// 获取操作指示代码
    if (argc < 2) {
        usage();
        return -1;
    }

    while ((ch = getopt(argc, argv, "?CRLV")) != -1) 
    {
        switch (ch) 
        {
            case '?':
                iInputFlag |= N_SHM_HELP;
                break;
            case 'C':
                iInputFlag |= N_SHM_CREATE;
                break;
            case 'R':
                iInputFlag |= N_SHM_REMOVE;
                break;
            case 'L':
                iInputFlag |= N_SHM_LOAD;
                break;    
            case 'V':
                iInputFlag |= N_SHM_VIEW;
                break;
            default:
                usage();
                return -1;
        }
    }
 
    /// 查看帮助
    if (N_SHM_HELP&iInputFlag) {
        usage();
    }
    
    iShmKey=kr_get_shmkey();
    if (iShmKey == -1) {
        printf("kr_get_shmkey error!\n");
        return -1;
    }
    
    /// 创建共享内存
    if (N_SHM_CREATE&iInputFlag) { 
		printf("N_SHM_CREATE\n");
		gptShmBuf=kr_shm_create(iShmKey);
        if (gptShmBuf == NULL) {
            printf("Initial Share Memory failed!\n");
            return -1;
        }
        
        if (LoadAllMemory() != 0) {
            printf("Load Share Memory failed!\n");
            kr_shm_destroy(iShmKey);
            return -1;
        }
        
        kr_shm_detach(gptShmBuf);
    }
    
    /// 加载共享内存
    if (N_SHM_LOAD&iInputFlag) {
        printf("N_SHM_LOAD\n");
        gptShmBuf=kr_shm_attach(iShmKey);
        if (gptShmBuf == NULL) {
            printf("Attach Share Memory failed!\n");
            return -1;
        }
        
        if (LoadAllMemory() != 0) {
            printf("Load Share Memory failed!\n");
            return -1;
        }

        kr_shm_detach(gptShmBuf);
    }
    
    /// 查看共享内存
    if (N_SHM_VIEW&iInputFlag) {
		printf("N_SHM_VIEW\n");
        gptShmBuf=kr_shm_attach(iShmKey);
        if (gptShmBuf == NULL) {
            printf("Attach Share Memory failed!\n");
            return -1;
        }
        
        if (ViewMemory() != 0) {
            printf("View Share Memory failed!\n");
            return -1;
        }

        kr_shm_detach(gptShmBuf);
    }

    /// 删除共享内存
    if (N_SHM_REMOVE&iInputFlag) {
		printf("N_SHM_REMOVE\n");
        printf("Are you sure to remove cum share memory?(Y/N)\n");
        ch=getchar();
        if(ch != 'Y' && ch != 'y') {
            printf("You choose [%c],return!\n", ch);
            return -1;
        }

        if (kr_shm_destroy(iShmKey) != 0) {
            printf("Remove share memory failed!\n");
            return -1;
        }
        printf("Share Memory Removed!\n");
    }
        
    return 0;
}


static int kr_get_shmkey(void)
{
    int     iShmKey;
    
    if (kr_config_setfile(getenv("KR_CONFIG_FILE")) != 0) 
    {
        printf("kr_config_setfile [%s] failed!", getenv("KR_CONFIG_FILE"));
        return -1;
    }

    if (kr_config_getint("SYSTEM", "IPCKEY", &iShmKey) != 0) 
    {
        printf("kr_config_getint IPCKEY failed!");
        return -1;
    }

    return iShmKey;
}

static int kr_get_detectmode(void)
{
    short     nDetectMode;
    
    if (kr_config_setfile(getenv("KR_CONFIG_FILE")) != 0) 
    {
        printf("kr_config_setfile [%s] failed!", getenv("KR_CONFIG_FILE"));
        return -1;
    }

    if (kr_config_getshort("SYSTEM", "DETECTMODE", &nDetectMode) != 0) 
    {
        printf("kr_config_getint DETECTMODE failed!");
        return -1;
    }

    return nDetectMode;
}

/** 
 *  @brief 导出共享内存所有数据
 *  @param[in]  无
 *  @param[out] 无
 *  @return 函数处理结果
 *  @retval   0: 成功
 *  @retval  -1: 失败
 */
static void DumpAllMemory(void)
{
    FILE *fp = NULL;
    char caDateTime[14+1] = {0};
    char caShmDumpFile[1024] = {0};
    
    kr_time_system(caDateTime);
    snprintf(caShmDumpFile, sizeof(caShmDumpFile), "krshm.%s.dump", caDateTime);
    if ((fp = fopen(caShmDumpFile, "a+")) == NULL) {
        printf("cant't open dump file!\n");
        return ;
    }
        
    /** dump all share memory */
    kr_shm_dump(gptShmBuf, fp);
    
    fclose(fp);
}

/** 
 *  @brief 加载共享内存所有数据
 *  @param[in]  无
 *  @param[out] 无
 *  @return 函数处理结果
 *  @retval   0: 成功
 *  @retval  -1: 失败
 */
static int LoadAllMemory(void)
{
    /// 连接交易数据库
    if (dbsDbConnect( ) != 0) {
        printf("connect to db failed!\n");
        return -1;
    }
    
    /** load all share memory */
    gnSecId = kr_shm_load(gptShmBuf);
    if (gnSecId == -1) {
        printf("kr_shm_load failed!\n");
        return -1;
    }
    
    /// 断开数据库链接 
    dbsDbDisconnect();
    
    return 0;
}

/** 
 *  @brief 有选择的查看共享内存块数据
 *  @param[in]  无
 *  @param[out] 无
 *  @return 函数处理结果
 *  @retval   0: 成功
 *  @retval  -1: 失败
 */
static int ViewMemory(void)
{
    char ch = '\0';
    char ind = '\0';
        
    printf("Choose Memory Block To View: \n");
    printf("S--[S]DI memory block\n");
    printf("D--[D]DI memory block\n");
    printf("H--[H]DI memory block\n");
    printf("R--[R]ule memory block\n");
    printf("else to quit!\n");

    do
    {        
        ch = getchar();
        if(ch == 'S' || ch == 's') {
            printf("Choose [A]ctive Or [S]tandby: \n");
            do
            {
                ind = getchar();
            }while(ind == '\0' || ind == '\n' || ind == '\t');
            ViewSDIMemory(ind);
        } else if(ch == 'D' || ch == 'd') {
            printf("Choose [A]ctive Or [S]tandby: \n");
            do
            {
                ind = getchar();
            }while(ind == '\0' || ind == '\n' || ind == '\t');
            ViewDDIMemory(ind);
        } else if(ch == 'H' || ch == 'h') {
            printf("Choose [A]ctive Or [S]tandby: \n");
            do
            {
                ind = getchar();
            }while(ind == '\0' || ind == '\n' || ind == '\t');
            ViewHDIMemory(ind);
        } else if(ch == 'R' || ch == 'r') {
            printf("Choose [A]ctive Or [S]tandby: \n");
            do
            {
                ind = getchar();
            }while(ind == '\0' || ind == '\n' || ind == '\t');
            ViewRuleMemory(ind);
        } else {
            printf("Quit:[%c]!\n", ch);
            break;
        }    
    }while(ch == '\0' || ch == '\n' || ch == '\t');
    
    return 0;
}


/** 
 *  @brief 查看静态数据项内容
 *  @param[in]  cInd - 主备块指示变量
 *  @param[out] 无
 *  @return 函数处理结果
 *  @retval   0: 成功
 *  @retval  -1: 失败
 */
static int ViewSDIMemory(char cInd)
{
    short       nCurrSecId;
    short       nBackSecId;

    nCurrSecId = gptShmBuf->nSecId;
    nBackSecId = (gptShmBuf->nSecId+1)%N_MAX_SEC;
    
    printf("sizeofshm:[%d], nDetectMode=[%d], nCurrSecId=[%d], nBackSecId=[%d]\n", \
            sizeof(*gptShmBuf), gptShmBuf->nDetectMode, nCurrSecId, nBackSecId);
        
    if (cInd == 'A' || cInd == 'a') {
        printf("[SDI] [Active] Memory Block:\n");
        DumpShmSDI(&gptShmBuf->stShmSDI[nCurrSecId], stdout);
    } else if (cInd == 'S' || cInd == 's') {
        printf("[SDI] [Standby] Memory Block:\n");
        DumpShmSDI(&gptShmBuf->stShmSDI[nBackSecId], stdout);
    } else {
        printf("Unrecognized Choice:[%c]\n", cInd);
        return -1;
    }
    
    return 0;
}


/** 
 *  @brief 查看动态统计量内容
 *  @param[in]  cInd - 主备块指示变量
 *  @param[out] 无
 *  @return 函数处理结果
 *  @retval   0: 成功
 *  @retval  -1: 失败
 */
static int ViewDDIMemory(char cInd)
{
    short       nCurrSecId;
    short       nBackSecId;

    nCurrSecId = gptShmBuf->nSecId;
    nBackSecId = (gptShmBuf->nSecId+1)%N_MAX_SEC;
    
    printf("sizeofshm:[%d], nDetectMode=[%d], nCurrSecId=[%d], nBackSecId=[%d]\n", \
            sizeof(*gptShmBuf), gptShmBuf->nDetectMode, nCurrSecId, nBackSecId);
    
    if (cInd == 'A' || cInd == 'a') {
        printf("[DDI] [Active] Memory Block:\n");
        DumpShmDDI(&gptShmBuf->stShmDDI[nCurrSecId], stdout);
    } else if (cInd == 'S' || cInd == 's') {
        printf("[DDI] [Standby] Memory Block:\n");
        DumpShmDDI(&gptShmBuf->stShmDDI[nBackSecId], stdout);
    } else {
        printf("Unrecognized Choice:[%c]\n", cInd);
        return -1;
    }
    
    return 0;
}


/** 
 *  @brief 查看历史统计量内容
 *  @param[in]  cInd - 主备块指示变量
 *  @param[out] 无
 *  @return 函数处理结果
 *  @retval   0: 成功
 *  @retval  -1: 失败
 */
static int ViewHDIMemory(char cInd)
{
    short       nCurrSecId;
    short       nBackSecId;

    nCurrSecId = gptShmBuf->nSecId;
    nBackSecId = (gptShmBuf->nSecId+1)%N_MAX_SEC;
    
    printf("sizeofshm:[%d], nDetectMode=[%d], nCurrSecId=[%d], nBackSecId=[%d]\n", \
            sizeof(*gptShmBuf), gptShmBuf->nDetectMode, nCurrSecId, nBackSecId);
    
    if (cInd == 'A' || cInd == 'a') {
        printf("[HDI] [Active] Memory Block:\n");
        DumpShmHDI(&gptShmBuf->stShmHDI[nCurrSecId], stdout);
    } else if (cInd == 'S' || cInd == 's') {
        printf("[HDI] [Standby] Memory Block:\n");
        DumpShmHDI(&gptShmBuf->stShmHDI[nBackSecId], stdout);
    } else {
        printf("Unrecognized Choice:[%c]\n", cInd);
        return -1;
    }
    
    return 0;
}


/** 
 *  @brief 查看规则项内容
 *  @param[in]  cInd - 主备块指示变量
 *  @param[out] 无
 *  @return 函数处理结果
 *  @retval   0: 成功
 *  @retval  -1: 失败
 */
static int ViewRuleMemory(char cInd)
{
    short       nCurrSecId;
    short       nBackSecId;

    nCurrSecId = gptShmBuf->nSecId;
    nBackSecId = (gptShmBuf->nSecId+1)%N_MAX_SEC;
    
    printf("sizeofshm:[%d], nDetectMode=[%d], nCurrSecId=[%d], nBackSecId=[%d]\n", \
            sizeof(*gptShmBuf), gptShmBuf->nDetectMode, nCurrSecId, nBackSecId);

    if (cInd == 'A' || cInd == 'a') {
        printf("[RULE] [Active] Memory Block:\n");
        DumpShmRule(&gptShmBuf->stShmRule[nCurrSecId], stdout);
    } else if (cInd == 'S' || cInd == 's') {
        printf("[RULE] [Standby] Memory Block:\n");
        DumpShmRule(&gptShmBuf->stShmRule[nBackSecId], stdout);
    } else {
        printf("Unrecognized Choice:[%c]\n", cInd);
        return -1;
    }
    
    return 0;
}




/** 
 *  @brief 用法帮助
 *  @param[in]  无
 *  @param[out] 无
 *  @return 无
 */
static void usage(void)
{
    printf("usage: afdipc [-? -C -R -L -V]");
    printf("\n"
"             -? help\n"
"             -C Create Share Memory\n"
"             -L Load Share Memory\n"
"             -R Remove Share Memory\n"
"             -V View Share Memory\n");
}
