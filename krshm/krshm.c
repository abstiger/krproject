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


int main(int argc,char* argv[])
{
    int             ch;
    int             iInputFlag=0;
    int             iShmKey=0;
    
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
 
    /** show usage */
    if (N_SHM_HELP&iInputFlag) {
        usage();
    }
    
    iShmKey=kr_get_shmkey();
    if (iShmKey == -1) {
        printf("kr_get_shmkey error!\n");
        return -1;
    }
    
    /** create share memory */
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
    
    /** load share memory */
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
    
    /** view share memory */
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

    /** delete share memory */
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
    
    if (getenv("KR_SHMKEY") == NULL) {
        printf("env KR_SHMKEY not set!");
        return -1;
    }
    iShmKey = atoi(getenv("KR_SHMKEY"));
printf("KR_SHMKEY=[%d]!", iShmKey);

    return iShmKey;
}


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


static int LoadAllMemory(void)
{
    /** connect db */
    T_DbsEnv *dbsenv = NULL;
    char  szDBName[128];
    char  szDBUser[128];
    char  szDBPass[32];

    if (getenv("DBNAME") != NULL) {    
        strncpy(szDBName, getenv("DBNAME"), sizeof(szDBName));
    }
    if (getenv("DBUSER") != NULL) {    
        strncpy(szDBUser, getenv("DBUSER"), sizeof(szDBUser));
    }
    if (getenv("DBPASS") != NULL) {    
        strncpy(szDBPass, getenv("DBPASS"), sizeof(szDBPass));
    }
    printf("DBNAME:[%s], DBUSER:[%s], DBPASS:[%s].\n", \
            szDBName, szDBUser, szDBPass);

    dbsenv = dbsConnect(szDBName, szDBUser, szDBPass);
    if (dbsenv == NULL) {
        printf("connect to [%s] [%s] [%s] failed!\n", 
                szDBName, szDBUser, szDBPass);
        return -1;
    }
    
    /** load all share memory */
    gnSecId = kr_shm_load(dbsenv, gptShmBuf);
    if (gnSecId == -1) {
        printf("kr_shm_load failed!\n");
        return -1;
    }
    
    /** disconnect db */
    dbsDisconnect(dbsenv);
    
    return 0;
}


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


static int ViewSDIMemory(char cInd)
{
    short       nCurrSecId;
    short       nBackSecId;

    nCurrSecId = gptShmBuf->nSecId;
    nBackSecId = (gptShmBuf->nSecId+1)%N_MAX_SEC;
    
    printf("sizeofshm:[%ld], nCurrSecId=[%d], nBackSecId=[%d]\n", \
            sizeof(*gptShmBuf), nCurrSecId, nBackSecId);
        
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


static int ViewDDIMemory(char cInd)
{
    short       nCurrSecId;
    short       nBackSecId;

    nCurrSecId = gptShmBuf->nSecId;
    nBackSecId = (gptShmBuf->nSecId+1)%N_MAX_SEC;
    
    printf("sizeofshm:[%ld], nCurrSecId=[%d], nBackSecId=[%d]\n", \
            sizeof(*gptShmBuf), nCurrSecId, nBackSecId);
    
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


static int ViewHDIMemory(char cInd)
{
    short       nCurrSecId;
    short       nBackSecId;

    nCurrSecId = gptShmBuf->nSecId;
    nBackSecId = (gptShmBuf->nSecId+1)%N_MAX_SEC;
    
    printf("sizeofshm:[%ld], nCurrSecId=[%d], nBackSecId=[%d]\n", \
            sizeof(*gptShmBuf), nCurrSecId, nBackSecId);
    
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


static int ViewRuleMemory(char cInd)
{
    short       nCurrSecId;
    short       nBackSecId;

    nCurrSecId = gptShmBuf->nSecId;
    nBackSecId = (gptShmBuf->nSecId+1)%N_MAX_SEC;
    
    printf("sizeofshm:[%ld], nCurrSecId=[%d], nBackSecId=[%d]\n", \
            sizeof(*gptShmBuf), nCurrSecId, nBackSecId);

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
