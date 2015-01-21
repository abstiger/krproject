#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krcalc/kr_calc.h"
#include "krdb/kr_db.h"
#include "krdata/kr_data.h"

#if 0
T_KRDynamicMem *ptDynMem = NULL;
T_KRContext gstContext = {0};
T_KRShareMem *gptShmBuf = NULL;

int ERMInitial(void)
{
    //Step 1:kr_db_startup
    gstContext.ptKRDB = kr_db_startup("ERM_KRDB");
    if (gstContext.ptKRDB == NULL)
    {
        KR_LOG(KR_LOGERROR, "kr_db_startup Failed!");
        return -1;
    }

    //kr_db_load(gptKRDB);
    //Step 3: kr_shm_attach 
    gstContext.ptShm = kr_shm_attach(74561);
    if (gstContext.ptShm == NULL) {
        KR_LOG(KR_LOGERROR, "Attach ShareMemory Failed!");
        return -1;
    }

    ptDynMem = kr_dym_construct(gptShmBuf);
    if (ptDynMem == NULL) 
    {
        KR_LOG(KR_LOGERROR, "kr_dym_construct Failed!");
        return -1;
    }

       return 0;
}


int ERMFinish(void)
{
    int iResult = 0;

    kr_dym_destruct(ptDynMem);
    
    /* kr_shm_detach */
    kr_shm_detach(gstContext.ptShm);
    
    /* kr_db_dump */
    FILE *fpKRDBDump = NULL;
    char caDateTime[14+1] = {0};
    char caKRDBDumpFileName[1024]= {0};
    snprintf(caKRDBDumpFileName, sizeof(caKRDBDumpFileName), \
             "KRDB.%s.Dump", kr_time_system(caDateTime));
    if ((fpKRDBDump = fopen(caKRDBDumpFileName, "w")) != NULL) 
    {
        kr_db_dump(gstContext.ptKRDB, 0, fpKRDBDump);
        fclose(fpKRDBDump);
    }
    
    // kr_db_shutdown
    iResult = kr_db_shutdown(gstContext.ptKRDB);
    if (iResult != 0)
    {
        KR_LOG(KR_LOGERROR, "kr_db_shutdown Failed!");
        return -1;
    }
    return 0;
}


int ERMHandle(int iDataSrc, void *ptReqDef)
{
    int iResult = 0;
    
    /* 插入KRDB内存数据库 */
    gstContext.ptCurrRec = kr_db_insert(gstContext.ptKRDB, iDataSrc, ptReqDef);
    if (gstContext.ptCurrRec == NULL)
    {
        KR_LOG(KR_LOGERROR, "kr_db_insert Failed!");
        return -1;
    }
KR_LOG(KR_LOGDEBUG, "0.kr_db_insert [%d] Passed!", iDataSrc);

    /* 规则侦测 */
    iResult = kr_rule_group_detect(ptDynMem, &gstContext);
    if (iResult != 0)
    {
        KR_LOG(KR_LOGERROR, "kr_rule_group_detect Failed!");
        return -1;
    }
KR_LOG(KR_LOGDEBUG, "1.kr_rule_group_detect [%d] Passed!", iDataSrc);
    
    /*DumpDataVal(stdout, &gptDataVal);*/
    
    return 0;
}


int main(int argc,char *argv[])
{
    int i = 0;
    int iResult = 0;
    char caTmpBuff[200] = {0};
    T_KRTradFlow_1 stTradFlow1 = {0};
    time_t    lTime;
    struct tm    *tTmLocal;
    char    sLogTime[128];
    
    iResult = dbsDbConnect();
    if (iResult != 0)
    {
        KR_LOG(KR_LOGERROR, "dbsDbConnect Failed!");
        return -1;
    }
    
    ERMInitial();
        
    //Step 2:FraudDetect
    for (i=0; i<22; i++)
    {
        memset(&stTradFlow1, 0x00, sizeof(T_KRTradFlow_1));
        
        memset(caTmpBuff, 0x00, sizeof(caTmpBuff));
        sprintf(caTmpBuff, "%s%04d", "6223000000000000", i%4);
        memcpy(stTradFlow1.caOutCustNo, caTmpBuff, sizeof(stTradFlow1.caOutCustNo));
                
        memset(caTmpBuff, 0x00, sizeof(caTmpBuff));
        sprintf(caTmpBuff, "201205%02d", i%7);
        memcpy(stTradFlow1.caOutTransDate, caTmpBuff, sizeof(stTradFlow1.caOutTransDate));
        
        memset(caTmpBuff, 0x00, sizeof(caTmpBuff));
        sprintf(caTmpBuff, "0919%02d", i%60);
        memcpy(stTradFlow1.caOutTransTime, caTmpBuff, sizeof(stTradFlow1.caOutTransTime));

        memset(caTmpBuff, 0x00, sizeof(caTmpBuff));
        sprintf(caTmpBuff, "%015d", i);
        memcpy(stTradFlow1.caOutFlowNo, caTmpBuff, sizeof(stTradFlow1.caOutFlowNo));
        
        memset(caTmpBuff, 0x00, sizeof(caTmpBuff));
        sprintf(caTmpBuff, "%02d", i%2);
        memcpy(stTradFlow1.caOutTransType, caTmpBuff, sizeof(stTradFlow1.caOutTransType));
        
        stTradFlow1.dOutTransAmt = i*9.9;
        
        memset(caTmpBuff, 0x00, sizeof(caTmpBuff));
        sprintf(caTmpBuff, "TRANS_LOCATION:%02d", i);
        memcpy(stTradFlow1.caOutTransLoc, caTmpBuff, sizeof(stTradFlow1.caOutTransLoc));
        
        if (i%10000 == 0)
        {
            /* get current time */
            memset (sLogTime, 0x00, sizeof(sLogTime));
            lTime = time (NULL);
            tTmLocal = localtime (&lTime);
            strftime (sLogTime, sizeof(sLogTime), "%Y-%m-%d %H:%M:%S", tTmLocal);
            printf("Inserting Record :[%d][%s]\n", i, sLogTime);
        }
        
        //Step 2.1:InsertIntoKRDB
        iResult = ERMHandle(1, &stTradFlow1);
        if (iResult != 0)
        {
            KR_LOG(KR_LOGERROR, "ERMHandle [%s] Failed!", stTradFlow1.caOutFlowNo);
            return -1;
        }
        
        //sleep(1);
    }
    
    //Step 3:kr_db_shutdown
    iResult = ERMFinish( );
    if (iResult != 0)
    {
        KR_LOG(KR_LOGERROR, "ERMFinish Failed!");
        return -1;
    }
    
    dbsDbDisconnect();
    

    return 0;
}
#else
int main(void)
{
    printf("kr_rule_test none\n");
    return 0;
}
#endif
