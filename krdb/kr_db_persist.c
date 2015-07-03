#include "kr_db_persist.h"
/*
#include "dbs/dbs_basopr.h"
#include "dbs/dbs/record_ins.h"
#include "dbs/dbs/record_proctime_cur.h"
#include "dbs/dbs/record_transtime_cur.h"


static int kr_db_external_select_with_transtime(T_KRList *pRecList, 
        T_KRTable *ptTable, time_t tBeginTransTime, time_t tEndTransTime, 
        int iSortFieldId, T_DbsEnv *dbsenv)
{
    int iResult = 0, iFlag = 0, iCnt = 0;

    T_RecordTranstimeCur stRecordTranstimeCur = {0};
    stRecordTranstimeCur.lInDatasrcId = ptTable->iTableId;
    stRecordTranstimeCur.lInBeginTransTime = tBeginTransTime;
    stRecordTranstimeCur.lInEndTransTime = tEndTransTime;
    iResult = dbsRecordTranstimeCur(dbsenv, KR_DBCUROPEN, &stRecordTranstimeCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsRecordTranstimeCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsRecordTranstimeCur(dbsenv, KR_DBCURFETCH, &stRecordTranstimeCur);
        if (iResult != KR_DBNOTFOUND && iResult != KR_DBOK) {
            KR_LOG(KR_LOGERROR, "dbsRecordTranstimeCur Fetch Error[%d]![%s]:[%s]", 
                    iResult, dbsenv->sqlstate, dbsenv->sqlerrmsg);
            iFlag = -1; break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Load [%d]Fields Of Table[%ld] Totally!", \
                    iCnt, stRecordTranstimeCur.lInDatasrcId);
            break;
        }
        
        char *psRecAddr = kr_calloc(ptTable->iRecordSize);
        if (psRecAddr == NULL) {
            KR_LOG(KR_LOGERROR, "kr_calloc psRecAddr error!");
            iFlag = -1; break;
        }
        T_KRRecord *ptRecord = (T_KRRecord *)psRecAddr;
        ptRecord->pfFree = kr_free;
        ptRecord->ptTable = ptTable;
        ptRecord->pRecBuf = psRecAddr+sizeof(T_KRRecord);
        memcpy(ptRecord->pRecBuf, stRecordTranstimeCur.caOutRecordBuffer, 
                stRecordTranstimeCur.lOutRecordLength);

        //append to record list
        if (iSortFieldId >= 0) {
            kr_list_add_sorted(pRecList, ptRecord, &iSortFieldId);
        } else {
            kr_list_add_tail(pRecList, ptRecord);
        }
        
        iCnt++;
    }

    iResult = dbsRecordTranstimeCur(dbsenv, KR_DBCURCLOSE, &stRecordTranstimeCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsRecordTranstimeCur Close Error!");
        return -1;
    }
    
    return iFlag;
}


static int kr_db_external_select_with_proctime(T_KRList *pRecList, 
        T_KRTable *ptTable, time_t tBeginProctime, time_t tEndProctime, 
        int iSortFieldId, T_DbsEnv *dbsenv)
{
    int iResult = 0, iFlag = 0, iCnt = 0;

    T_RecordProctimeCur stRecordProctimeCur = {0};
    stRecordProctimeCur.lInDatasrcId = ptTable->iTableId;
    stRecordProctimeCur.lInBeginProcTime = tBeginProctime;
    stRecordProctimeCur.lInEndProcTime = tEndProctime;
    iResult = dbsRecordProctimeCur(dbsenv, KR_DBCUROPEN, &stRecordProctimeCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsRecordProctimeCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsRecordProctimeCur(dbsenv, KR_DBCURFETCH, &stRecordProctimeCur);
        if (iResult != KR_DBNOTFOUND && iResult != KR_DBOK) {
            KR_LOG(KR_LOGERROR, "dbsRecordProctimeCur Fetch Error[%d]![%s]:[%s]", 
                    iResult, dbsenv->sqlstate, dbsenv->sqlerrmsg);
            iFlag = -1; break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Load [%d]Fields Of Table[%ld] Totally!", \
                    iCnt, stRecordProctimeCur.lInDatasrcId);
            break;
        }
        
        char *psRecAddr = kr_calloc(ptTable->iRecordSize);
        if (psRecAddr == NULL) {
            KR_LOG(KR_LOGERROR, "kr_calloc psRecAddr error!");
            iFlag = -1; break;
        }
        T_KRRecord *ptRecord = (T_KRRecord *)psRecAddr;
        ptRecord->pfFree = kr_free;
        ptRecord->ptTable = ptTable;
        ptRecord->pRecBuf = psRecAddr+sizeof(T_KRRecord);
        memcpy(ptRecord->pRecBuf, stRecordProctimeCur.caOutRecordBuffer, 
                stRecordProctimeCur.lOutRecordLength);

        //append to record list
        if (iSortFieldId >= 0) {
            kr_list_add_sorted(pRecList, ptRecord, &iSortFieldId);
        } else {
            kr_list_add_tail(pRecList, ptRecord);
        }
        
        iCnt++;
    }

    iResult = dbsRecordProctimeCur(dbsenv, KR_DBCURCLOSE, &stRecordProctimeCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsRecordProctimeCur Close Error!");
        return -1;
    }
    
    return iFlag;
}


int kr_db_external_insert(T_KRRecord *ptRecord, T_DbsEnv *dbsenv)
{
    int iResult = 0;

    T_RecordIns stRecordIns = {0};
    stRecordIns.lInDatasrcId = ptRecord->ptTable->iTableId;
    stRecordIns.lInProcTime = kr_get_proctime(ptRecord);
    stRecordIns.lInTransTime = kr_get_transtime(ptRecord);
    stRecordIns.lInRecordLength = \
        ptRecord->ptTable->iRecordSize-sizeof(T_KRRecord);
    memcpy(stRecordIns.caInRecordBuffer, ptRecord->pRecBuf, 
            stRecordIns.lInRecordLength);
    iResult = dbsRecordIns(dbsenv, &stRecordIns);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsRecordIns Error[%d]![%s]:[%s]",
                iResult, dbsenv->sqlstate, dbsenv->sqlerrmsg);
        return -1;
    }

    return 0;
}


int kr_db_external_select(T_KRList *pRecList, T_KRTable *ptTable, 
        E_KRPublicFieldId eQueryFieldId, time_t tBeginTime, time_t tEndTime, 
        int iSortFieldId, T_DbsEnv *dbsenv)
{
    int iRet = 0;

    switch(eQueryFieldId) {
    case KR_FIELDID_PROCTIME:
        iRet = kr_db_external_select_with_proctime(pRecList, 
                ptTable, tBeginTime, tEndTime, iSortFieldId, dbsenv);
        break;
    case KR_FIELDID_TRANSTIME:
        iRet = kr_db_external_select_with_transtime(pRecList, 
                ptTable, tBeginTime, tEndTime, iSortFieldId, dbsenv);
        break;
    default:
        KR_LOG(KR_LOGERROR, "unsupported public field:[%d]", eQueryFieldId);
        iRet = -1;
        break;
    }

    return iRet;
}

*/
