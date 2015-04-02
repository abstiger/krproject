#include "kr_db_define.h"

#include "dbs/dbs_basopr.h"
#include "dbs/dbs/datasrc_field_cur.h"
#include "dbs/dbs/datasrc_field_cnt_sel.h"
#include "dbs/dbs/datasrc_cur.h"
#include "dbs/dbs/index_cur.h"
#include "dbs/dbs/index_table_cur.h"

#define KR_MEMALIGN(size)   (((size) + 0xf) & ~(size_t) 0xf)


int _kr_table_load_field_def(T_KRTable *ptTable, T_DbsEnv *dbsenv)
{
    int iResult = 0, iFlag = 0, iCnt = 0;

    /*get field count of this table*/
    T_DatasrcFieldCntSel stDatasrcFieldCntSel = {0};
    dbsDatasrcFieldCntSelInit(&stDatasrcFieldCntSel);
    stDatasrcFieldCntSel.lInDatasrcId = ptTable->iTableId;
    iResult = dbsDatasrcFieldCntSel(dbsenv, \
            KR_DBSELECT, &stDatasrcFieldCntSel);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDatasrcFieldCntSel Error[%d]![%s]:[%s]",
                iResult, dbsenv->sqlstate, dbsenv->sqlerrmsg);
        return -1;
    }
    /*FIXME:caOutFieldCnt should be lOutFieldCnt, it's sqlite3 odbcdriver bug */
    ptTable->iFieldCnt = atoi(stDatasrcFieldCntSel.caOutFieldCnt);
    ptTable->ptFieldDef = kr_calloc(ptTable->iFieldCnt*sizeof(T_KRFieldDef));
    if (ptTable->ptFieldDef == NULL) {
        fprintf(stderr, "kr_calloc ptTable->ptFieldDef failed!\n");
        return -1;
    }

    /*load field definition of this table*/
    size_t ulFieldOffset = 0;
    T_DatasrcFieldCur stDatasrcFieldCur = {0};
    stDatasrcFieldCur.lInDatasrcId = ptTable->iTableId;
    iResult = dbsDatasrcFieldCur(dbsenv, KR_DBCUROPEN, &stDatasrcFieldCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDatasrcFieldCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsDatasrcFieldCur(dbsenv, KR_DBCURFETCH, &stDatasrcFieldCur);
        if (iResult != KR_DBNOTFOUND && iResult != KR_DBOK) {
            KR_LOG(KR_LOGERROR, "dbsDatasrcFieldCur Fetch Error[%d]![%s]:[%s]", 
                    iResult, dbsenv->sqlstate, dbsenv->sqlerrmsg);
            iFlag = -1; break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Load [%d]Fields Of Table[%ld] Totally!", \
                    iCnt, stDatasrcFieldCur.lInDatasrcId);
            break;
        }
        
        kr_string_rtrim(stDatasrcFieldCur.caOutFieldName);
        kr_string_rtrim(stDatasrcFieldCur.caOutFieldDesc);

        T_KRFieldDef *ptFieldDef = &ptTable->ptFieldDef[iCnt];
        ptFieldDef->id = stDatasrcFieldCur.lOutFieldId;
        strncpy(ptFieldDef->name, stDatasrcFieldCur.caOutFieldName, \
                sizeof(ptFieldDef->name));
        ptFieldDef->type = stDatasrcFieldCur.caOutFieldType[0];
        ptFieldDef->length = stDatasrcFieldCur.lOutFieldLength;
        ptFieldDef->offset = ulFieldOffset;
        ulFieldOffset += ptFieldDef->length;
        /* String terminated with '\0' */
        if (ptFieldDef[iCnt].type == KR_TYPE_STRING) {
            ulFieldOffset += 1;
        }
        
        iCnt++;
    }

    iResult = dbsDatasrcFieldCur(dbsenv, KR_DBCURCLOSE, &stDatasrcFieldCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDatasrcFieldCur Close Error!");
        return -1;
    }

    /*compute record size, alloc record buffer*/
    ptTable->iRecordSize = sizeof(T_KRRecord)+ulFieldOffset;
    ptTable->iRecordSize = KR_MEMALIGN(ptTable->iRecordSize);
    ptTable->pRecordBuff = \
        (char *)kr_calloc(ptTable->iRecordSize*ptTable->lKeepValue);
    if (ptTable->pRecordBuff == NULL) {
        fprintf(stderr, "kr_calloc ptTable->pRecordBuff failed!\n");
        return -1;
    }
    
    return iFlag;
}


int _kr_db_build_table(T_KRDB *ptDB)
{
    int iResult = 0, iFlag = 0, iCnt = 0;
    T_DbsEnv *dbsenv = ptDB->dbsenv;

    T_DatasrcCur stDatasrcCur = {0};
    iResult = dbsDatasrcCur(dbsenv, KR_DBCUROPEN, &stDatasrcCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDatasrcCur Open Error[%d]!", iResult);
        return -1;
    }
    
    while(1)
    {
        iResult = dbsDatasrcCur(dbsenv, KR_DBCURFETCH, &stDatasrcCur);
        if (iResult != KR_DBNOTFOUND && iResult != KR_DBOK) {
            KR_LOG(KR_LOGERROR, "dbsDatasrcCur Fetch Error[%d]![%s]:[%s]", 
                    iResult, dbsenv->sqlstate, dbsenv->sqlerrmsg);
            iFlag = -1;
            break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Create [%d] Tables Totally!", iCnt);
            break;
        }
        
        kr_string_rtrim(stDatasrcCur.caOutDatasrcName);
        kr_string_rtrim(stDatasrcCur.caOutDatasrcDesc);

        KR_LOG(KR_LOGDEBUG, "kr_table_create:"
                "Id[%ld], Name[%s], Desc[%s], Usage[%s],"
                "SizeKeepMode[%s], SizeKeepValue[%ld]",
                stDatasrcCur.lOutDatasrcId, 
                stDatasrcCur.caOutDatasrcName, 
                stDatasrcCur.caOutDatasrcDesc, 
                stDatasrcCur.caOutDatasrcUsage, 
                stDatasrcCur.caOutSizeKeepMode, 
                stDatasrcCur.lOutSizeKeepValue);        

        T_KRTable *ptTable = kr_table_create(ptDB,
                stDatasrcCur.lOutDatasrcId, 
                stDatasrcCur.caOutDatasrcName, 
                stDatasrcCur.caOutSizeKeepMode[0], 
                stDatasrcCur.lOutSizeKeepValue);
        if (ptTable == NULL) {
            KR_LOG(KR_LOGERROR, "kr_table_create [%ld] Error!", \
                    stDatasrcCur.lOutDatasrcId);
            iFlag = -1; 
            break;
        }
        _kr_table_load_field_def(ptTable, dbsenv);

        iCnt++;
    }

    iResult = dbsDatasrcCur(dbsenv, KR_DBCURCLOSE, &stDatasrcCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDatasrcCur Close Error!");
        return -1;
    }

    return iFlag;
}


int _kr_db_build_index(T_KRDB *ptDB)
{
    int iResult = 0, iFlag = 0, iCnt = 0;
    T_DbsEnv *dbsenv = ptDB->dbsenv;

    T_IndexCur stIndexCur = {0};
    iResult = dbsIndexCur(dbsenv, KR_DBCUROPEN, &stIndexCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsIndexCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsIndexCur(dbsenv, KR_DBCURFETCH, &stIndexCur);
        if (iResult != KR_DBNOTFOUND && iResult != KR_DBOK) {
            KR_LOG(KR_LOGERROR, "dbsIndexCur Fetch Error[%d]![%s]:[%s]",
                    iResult, dbsenv->sqlstate, dbsenv->sqlerrmsg);
            iFlag = -1; break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Build [%d] Indexes Totally!", iCnt);
            break;
        }

        kr_string_rtrim(stIndexCur.caOutIndexName);
        kr_string_rtrim(stIndexCur.caOutIndexDesc);
        
        KR_LOG(KR_LOGDEBUG, "kr_index_create:"
                "Id[%ld], Name[%s], Desc[%s], IndexFieldType[%s]",
                stIndexCur.lOutIndexId, 
                stIndexCur.caOutIndexName, 
                stIndexCur.caOutIndexDesc, 
                stIndexCur.caOutIndexFieldType);        
                
        T_KRIndex *ptIndex = kr_index_create(ptDB,
                stIndexCur.lOutIndexId, \
                stIndexCur.caOutIndexName, \
                stIndexCur.caOutIndexFieldType[0]);
        if (ptIndex == NULL) {
            KR_LOG(KR_LOGERROR, "kr_index_create [%ld] [%s] failed!", \
                    stIndexCur.lOutIndexId, stIndexCur.caOutIndexName);
            iFlag = -1; break;
        }

        iCnt++;
    }

    iResult = dbsIndexCur(dbsenv, KR_DBCURCLOSE, &stIndexCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsIndexCur Close Error!");
        return -1;
    }
    
    return iFlag;
}


int _kr_db_build_index_table(T_KRDB *ptDB)
{
    int iResult = 0, iFlag = 0, iCnt = 0;
    T_DbsEnv *dbsenv = ptDB->dbsenv;

    T_IndexTableCur stIndexTableCur = {0};
    iResult = dbsIndexTableCur(dbsenv, KR_DBCUROPEN, &stIndexTableCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsIndexTableCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsIndexTableCur(dbsenv, KR_DBCURFETCH, &stIndexTableCur);
        if (iResult != KR_DBNOTFOUND && iResult != KR_DBOK) {
            KR_LOG(KR_LOGERROR, "dbsIndexTableCur Fetch Error[%d]![%s]:[%s]",
                    iResult, dbsenv->sqlstate, dbsenv->sqlerrmsg);
            iFlag = -1; break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Build [%d] Indexes Totally!", iCnt);
            break;
        }

        KR_LOG(KR_LOGDEBUG, "kr_index_table_create:"
                "IndexId[%ld], DatasrcId[%ld], "
                "IndexFieldId[%ld], SortFieldId[%ld]",
                stIndexTableCur.lOutIndexId, \
                stIndexTableCur.lOutDatasrcId, \
                stIndexTableCur.lOutIndexFieldId,
                stIndexTableCur.lOutSortFieldId);        

        T_KRIndexTable *ptIndexTable = kr_index_table_create(ptDB,
                stIndexTableCur.lOutIndexId, \
                stIndexTableCur.lOutDatasrcId, \
                stIndexTableCur.lOutIndexFieldId, \
                stIndexTableCur.lOutSortFieldId);
        if (ptIndexTable == NULL) {
            KR_LOG(KR_LOGERROR, "kr_index_table_create [%ld] [%ld] failed!", \
                    stIndexTableCur.lOutIndexId, 
                    stIndexTableCur.lOutDatasrcId);
            iFlag = -1; break;
        }
        
        iCnt++;
    }

    iResult = dbsIndexTableCur(dbsenv, KR_DBCURCLOSE, &stIndexTableCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsIndexTableCur Close Error!");
        return -1;
    }
    
    return iFlag;
}
