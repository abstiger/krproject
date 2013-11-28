#include "krutils/kr_utils.h"
#include "kr_db_mmap.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>


int kr_db_mmap_sync(T_KRTable *krtable)
{
    T_KRTable *ptTable = (T_KRTable *)krtable->pMMapAddr;
    ptTable->uiRecordNum = krtable->uiRecordNum;
    ptTable->uiRecordLoc = krtable->uiRecordLoc;
    /*synchronize header*/
    msync(ptTable, sizeof(T_KRTable), MS_SYNC);
    /*TODO:synchronize record*/
    /*
       msync(ptTable+sizeof(T_KRTable)+ptTable->iFieldCnt*sizeof(T_KRFieldDef)+ 
       (ptTable->uiRecordLoc<=0?ptTable->lSizeKeepValue:ptTable->uiRecordLoc-1)*ptTable->iRecordSize, ptTable->iRecordSize, MS_SYNC);
     */
    return 0;
}


int kr_db_mmap_table(T_KRTable *ptTable)
{
    char caTempFile[256] = {0};
    snprintf(caTempFile,256,"%s-%d.krdb", ptTable->caMMapFile, (int)getpid());
    
    int fd = open(caTempFile, O_CREAT|O_RDWR|O_TRUNC, 00777);
    if (fd < 0) {
        fprintf(stderr, "open temp mmap file [%s] error!\n", caTempFile);
        return -1;
    }
    ptTable->uiMMapSize = sizeof(T_KRTable) + \
                (ptTable->iFieldCnt * sizeof(T_KRFieldDef)) + \
                (ptTable->iRecordSize * ptTable->lSizeKeepValue);
                
    
    /* offset must be a multiple of the page size 
       as returned by sysconf(_SC_PAGE_SIZE). */
    int i = 0;
    while(ptTable->uiMMapSize>i*sysconf(_SC_PAGE_SIZE)) {
        ++i;
    }
    ptTable->uiMMapSize = i*sysconf(_SC_PAGE_SIZE);

printf("mmap_file_size =[%u]:[%ld] + [%ld] + [%ld] page_size=[%ld].\n", \
        ptTable->uiMMapSize, 
        (long )sizeof(T_KRTable), 
        (long )(ptTable->iFieldCnt * sizeof(T_KRFieldDef)), 
        (long )(ptTable->iRecordSize * ptTable->lSizeKeepValue), 
        (long )sysconf(_SC_PAGE_SIZE));
    
    lseek(fd, ptTable->uiMMapSize-1, SEEK_SET);
    if (write(fd, " ", 1) < 0) {
        fprintf(stderr, "write mmapfile error!\n");
        return -1;
    }
    lseek(fd, 0L, SEEK_SET);
    
    ptTable->pMMapAddr = mmap(NULL, ptTable->uiMMapSize, \
                              PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptTable->pMMapAddr == MAP_FAILED) {
        fprintf(stderr, "mmap ptTable->uiMMapSize [%u] [%d]error!\n",\
                ptTable->uiMMapSize, errno);
        return -1;
    }
    close(fd);
    
    char *p = ptTable->pMMapAddr;
    memcpy(p, ptTable, sizeof(T_KRTable));
    p += sizeof(T_KRTable);
    memcpy(p, ptTable->ptFieldDef, (ptTable->iFieldCnt * sizeof(T_KRFieldDef)));
    T_KRTable *mmap_table = (T_KRTable *)ptTable->pMMapAddr;
    mmap_table->ptFieldDef = (T_KRFieldDef *)p;
    p += (ptTable->iFieldCnt * sizeof(T_KRFieldDef));
    ptTable->pRecordBuff = p;
    
    return 0;
}


void kr_db_unmmap_table(T_KRTable *ptTable)
{
    char caTempFile[256] = {0};
    
    munmap(ptTable->pMMapAddr, ptTable->uiMMapSize);
    
    snprintf(caTempFile,256,"%s-%d.krdb", ptTable->caMMapFile, (int)getpid());
    if (rename(caTempFile, ptTable->caMMapFile) == -1) {
        fprintf(stderr, "rename [%s] to [%s] failed!\n", 
                caTempFile, ptTable->caMMapFile);
    }
}


int kr_db_mmap_file_handle(char *mmap_file, MMapFunc handle_record_func, void *user_data)
{
    int nRet = 0;
    FILE *fp = NULL;

    fp = fopen(mmap_file, "rb");
    if (fp == NULL) {
        fprintf(stderr, "fopen [%s] failed[%s]!\n", mmap_file, strerror(errno));
        nRet = -1; goto clean;
    }
    
    T_KRTable *ptTable = kr_calloc(sizeof(T_KRTable));
    if (ptTable == NULL) {
        fprintf(stderr, "mmap ptTable failed!\n");
        nRet = -1; goto clean;
    }
    
    if (fread(ptTable, sizeof(T_KRTable), 1, fp) <= 0) {
        fprintf(stderr, "fread ptTable failed!\n");
        nRet = -1; goto clean;
    }
    
    ptTable->ptFieldDef = kr_calloc(ptTable->iFieldCnt * sizeof(T_KRFieldDef));
    if (ptTable == NULL) {
        fprintf(stderr, "kr_calloc ptTable->ptFieldDef failed!\n");
        nRet = -1; goto clean;
    }
    
    if (fread(ptTable->ptFieldDef, ptTable->iFieldCnt * sizeof(T_KRFieldDef), 1, fp) <= 0) {
        fprintf(stderr, "fread ptFieldDef failed!\n");
        nRet = -1; goto clean;
    }

    char *pRecordBuff = (char *)kr_calloc(ptTable->iRecordSize);
    if (pRecordBuff == NULL) {
        fprintf(stderr, "kr_calloc pRecordBuff failed!\n");
        nRet = -1; goto clean;
    }

printf("ptTable->uiRecordLoc:[%u], ptTable->uiRecordNum:[%u]\n", \
       ptTable->uiRecordLoc, ptTable->uiRecordNum);

    T_KRRecord *ptRecord = NULL;
    unsigned int uiCnt = 0;
    unsigned int uiLoc = ptTable->uiRecordLoc;
    while(uiCnt < ptTable->uiRecordNum) 
    {        
        memset(pRecordBuff, 0x00, ptTable->iRecordSize);
        fseek(fp, sizeof(T_KRTable)+ptTable->iFieldCnt*sizeof(T_KRFieldDef)+ \
                  uiLoc*ptTable->iRecordSize, SEEK_SET);
        if (fread(pRecordBuff, ptTable->iRecordSize, 1, fp) <= 0) {
            fprintf(stderr, "fread pRecordBuff failed!\n");
            nRet = -1; goto clean;
        }
        
        ptRecord = (T_KRRecord *)pRecordBuff;
        ptRecord->ptTable = ptTable;
        ptRecord->pRecBuf = pRecordBuff+sizeof(T_KRRecord);
        if (handle_record_func(ptRecord, user_data) != 0) {
            fprintf(stderr, "handle_record_func failed!\n");
            nRet = -1; goto clean;
        }
        
        uiLoc = (++uiLoc)%ptTable->lSizeKeepValue;
        ++uiCnt;
    }

clean:
    if (ptTable != NULL) {
        if (ptTable->ptFieldDef != NULL) {
            kr_free(ptTable->ptFieldDef);
        }
        kr_free(ptTable);
    }
    if (pRecordBuff != NULL) kr_free(pRecordBuff);
    if (fp != NULL) fclose(fp);
    return nRet;
}
