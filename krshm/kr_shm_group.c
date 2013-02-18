#include "kr_shm_group.h"
#include "krutils/kr_utils.h"
#include "dbs/dbs_basopr.h"
#include "dbs/dbs/group_cur.h"

int LoadShmGroup(T_DbsEnv *dbsenv, T_KRShmGroup *ptShmGroup)
{
    int nRet = 0;
    int iResult = 0;
    int iCnt = 0;    
    T_GroupCur stGroupCur = {0};
    T_KRShmGroupDef *ptShmGroupDef = &ptShmGroup->stShmGroupDef[0];
    
    iResult = dbsGroupCur(dbsenv, KR_DBCUROPEN, &stGroupCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsGroupCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsGroupCur(dbsenv, KR_DBCURFETCH, &stGroupCur);
        if (iResult != KR_DBNOTFOUND && iResult != KR_DBOK) {
            KR_LOG(KR_LOGERROR, "dbsGroupCur Fetch Error!");
            nRet = -1;
            break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Load [%d] Groups Totally!", iCnt);
            break;
        }
        
        if (iCnt >= N_MAX_GROUP_NUM) {
            KR_LOG(KR_LOGERROR, "Only [%d] Groups Allowed!", N_MAX_GROUP_NUM);
            nRet = -1;
            break;
        }
        
        memset(ptShmGroupDef, 0x00, sizeof(T_KRShmGroupDef));
        ptShmGroupDef->lGroupId = stGroupCur.lOutGroupId;
        strncpy(ptShmGroupDef->caGroupName, \
                kr_string_rtrim(stGroupCur.caOutGroupName), \
                sizeof(ptShmGroupDef->caGroupName));
        strncpy(ptShmGroupDef->caGroupDesc, \
                kr_string_rtrim(stGroupCur.caOutGroupDesc), \
                sizeof(ptShmGroupDef->caGroupDesc));
        strncpy(ptShmGroupDef->caGroupString, \
                kr_string_rtrim(stGroupCur.caOutGroupString), \
                sizeof(ptShmGroupDef->caGroupString));
        /*load rules of this group*/
        if (LoadShmRule(dbsenv, &ptShmGroupDef->stShmRule, \
                    ptShmGroupDef->lGroupId) != 0) {
            KR_LOG(KR_LOGERROR, "LoadShmRule [%ld] failed!", \
                    ptShmGroupDef->lGroupId);
            nRet = -1;
            break;
        }
        
        ptShmGroup->lGroupDefCnt++;
        ptShmGroupDef++;
        iCnt++;
    }
    ptShmGroup->tLastLoadTime = time(NULL);

    iResult = dbsGroupCur(dbsenv, KR_DBCURCLOSE, &stGroupCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsGroupCur Close Error!");
        return -1;
    }
    
    return nRet;
}


int DumpShmGroup(T_KRShmGroup *ptShmGroup, FILE *fp)
{
    long l;
    T_KRShmGroupDef *ptShmGroupDef = &ptShmGroup->stShmGroupDef[0];
    char            caTimeString[80];
    struct tm *ptmNow = localtime(&ptShmGroup->tLastLoadTime);
    strftime(caTimeString, sizeof(caTimeString), "%c", ptmNow);
    
    fprintf(fp, "Dumping Group...\n");
    fprintf(fp, "Last Load Time[%s]\n", caTimeString);
    for (l=0; l<ptShmGroup->lGroupDefCnt; l++) {
        fprintf(fp, "  lGroupId=[%ld], caGroupName=[%s], ", 
                ptShmGroupDef->lGroupId, ptShmGroupDef->caGroupName);
        fprintf(fp, "caGroupDesc=[%s], caGroupString=[%s] \n", 
                ptShmGroupDef->caGroupDesc, ptShmGroupDef->caGroupString);
        /*dump rules of this group*/
        DumpShmRule(&ptShmGroupDef->stShmRule, fp);
        ptShmGroupDef++;
    }
    
    return 0;
}
