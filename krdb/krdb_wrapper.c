#include "bas/bas_global.h"
#include "krdb/krdb_wrapper.h"
#include "dbs/dbs/datasrc_def_cur.h"
#include "dbs/dbs/datasrc_field_cur.h"
#include "dbs/dbs/datasrc_index_cur.h"
#include "dbs/dbs/datasrc_field_cnt_sel.h"
#include "dbs/dbs/datasrc_field_def_sel.h"


extern void AssignFldValFunc(gpointer p1, gpointer p2);

static void LoadFldDefFunc(T_KRFieldDef *ptFieldDef, int *piTableId);
static int BuildTableIndex(T_KRDB *ptKRDB, T_KRTable *ptTable);
static void PrintNode(gpointer key, gpointer value, gpointer user_data);
static void PrintIndex(gpointer p1, gpointer p2);
static void DumpTableWithIndex (gpointer p1, gpointer p2);
static void DumpTableOnlyRecord (gpointer p1, gpointer p2);

int SetFieldDef(T_KRFieldDef *ptFieldDef, int iTableId, int iFieldId)
{
    int iResult = 0;
    int iFlag = 0;
    int iCnt = 0;
    T_DatasrcFieldDefSel stDatasrcFieldDefSel = {0};
    
    stDatasrcFieldDefSel.lInDatasrcId = iTableId;
    stDatasrcFieldDefSel.lInFieldId = iFieldId;
    iResult = dbsDatasrcFieldDefSel(BAS_DBSELECT, &stDatasrcFieldDefSel);
    if (iResult != BAS_DBOK)
    {
        BAS_LOG(BAS_LOGERROR, iResult, 0, "dbsDatasrcFieldDefSel [%d],[%d] Error!",\
                stDatasrcFieldDefSel.lInDatasrcId, stDatasrcFieldDefSel.lInFieldId);
        return BAS_FAILURE;
    }
    
    ptFieldDef->id = stDatasrcFieldDefSel.lInFieldId;
    strncpy(ptFieldDef->name, basRTrim(stDatasrcFieldDefSel.caOutFieldName), \
            sizeof(ptFieldDef->name));
    ptFieldDef->type = stDatasrcFieldDefSel.caOutFieldType[0];
    ptFieldDef->length = stDatasrcFieldDefSel.lOutFieldLength;
    ptFieldDef->precision = stDatasrcFieldDefSel.lOutFieldPrecision;
    ptFieldDef->scale = stDatasrcFieldDefSel.lOutFieldScale;

    return BAS_SUCCESS;
}

static void LoadFldDefFunc(T_KRFieldDef *ptFieldDef, int *piTableId)
{
    int iResult = 0;
    int iFlag = 0;
    int iCnt = 0;
    int iOffset = 0;
    T_DatasrcFieldCur stDatasrcFieldCur = {0};
    
    stDatasrcFieldCur.lInDatasrcId = *piTableId;
    iResult = dbsDatasrcFieldCur(BAS_DBCUROPEN, &stDatasrcFieldCur);
    if (iResult != BAS_DBOK)
    {
        BAS_LOG(BAS_LOGERROR, iResult, 0, "dbsDatasrcFieldCur Open Error!");
        return;
    }
    
    while(1)
    {
        iResult=dbsDatasrcFieldCur(BAS_DBCURFETCH, &stDatasrcFieldCur);
        if (iResult != BAS_DBNOTFOUND && iResult != BAS_DBOK)
        {
            BAS_LOG(BAS_LOGERROR, iResult, 0, "dbsDatasrcFieldCur Fetch Error!");
            iFlag = -1;
            break;
        }
        else if (iResult == BAS_DBNOTFOUND)
        {
            BAS_LOG(BAS_LOGDEBUG, 0, 0, "Load [%d]Fields Of Table[%ld] Totally!", \
                    iCnt, stDatasrcFieldCur.lInDatasrcId);
            break;
        }
        
        ptFieldDef[iCnt].id = stDatasrcFieldCur.lOutFieldId;
        strncpy(ptFieldDef[iCnt].name, basRTrim(stDatasrcFieldCur.caOutFieldName), \
                sizeof(ptFieldDef[iCnt].name));
        ptFieldDef[iCnt].type = stDatasrcFieldCur.caOutFieldType[0];
        ptFieldDef[iCnt].length = stDatasrcFieldCur.lOutFieldLength;
        ptFieldDef[iCnt].precision = stDatasrcFieldCur.lOutFieldPrecision;
        ptFieldDef[iCnt].scale = stDatasrcFieldCur.lOutFieldScale;
        ptFieldDef[iCnt].offset = iOffset;
        iOffset += ptFieldDef[iCnt].length;
        
        iCnt++;
    }

    iResult = dbsDatasrcFieldCur(BAS_DBCURCLOSE, &stDatasrcFieldCur);
    if (iResult != BAS_DBOK)
    {
        BAS_LOG(BAS_LOGERROR, iResult, 0, "dbsDatasrcFieldCur Close Error!");
        return;
    }
}


static int BuildTableIndex(T_KRDB *ptKRDB, T_KRTable *ptTable)
{
    int iFlag = BAS_SUCCESS;
    int iResult = 0;
    int iCnt = 0;
    T_DatasrcIndexCur stDatasrcIndexCur = {0};
    
    
    stDatasrcIndexCur.lInDatasrcId = ptTable->iTableId;
    iResult = dbsDatasrcIndexCur(BAS_DBCUROPEN, &stDatasrcIndexCur);
    if (iResult != BAS_DBOK)
    {
        BAS_LOG(BAS_LOGERROR, iResult, 0, "dbsDatasrcIndexCur Open Error!");
        return BAS_FAILURE;
    }
    
    while(1)
    {
        iResult=dbsDatasrcIndexCur(BAS_DBCURFETCH, &stDatasrcIndexCur);
        if (iResult != BAS_DBNOTFOUND && iResult != BAS_DBOK)
        {
            BAS_LOG(BAS_LOGERROR, iResult, 0, "dbsDatasrcIndexCur Fetch Error!");
            iFlag = BAS_FAILURE;
            break;
        }
        else if (iResult == BAS_DBNOTFOUND)
        {
            BAS_LOG(BAS_LOGDEBUG, 0, 0, "Build [%d] Indexes Totally!", iCnt);
            break;
        }
                           
        T_KRIndex *ptIndex=kr_create_index(ptKRDB, ptTable, \
                        stDatasrcIndexCur.lOutIndexId, \
                        basRTrim(stDatasrcIndexCur.caOutIndexName), \
                        stDatasrcIndexCur.caOutIndexType[0], \
                        stDatasrcIndexCur.lOutIndexFieldId, NULL, NULL);
        if (ptIndex == NULL)
        {
            BAS_LOG(BAS_LOGERROR, 0, 0, "kr_create_index [%ld] [%s] failed!", \
                    stDatasrcIndexCur.lOutIndexId, stDatasrcIndexCur.caOutIndexType);
            break;
        }
        
        iCnt++;
    }

    iResult = dbsDatasrcIndexCur(BAS_DBCURCLOSE, &stDatasrcIndexCur);
    if (iResult != BAS_DBOK)
    {
        BAS_LOG(BAS_LOGERROR, iResult, 0, "dbsDatasrcIndexCur Close Error!");
        return BAS_FAILURE;
    }
    
    return iFlag;
}

T_KRDB* KRDBStartup(char *dbname)
{
    int iFlag = BAS_SUCCESS;
    int iResult = 0;
    int iCnt = 0;
    T_KRDB *ptKRDB=NULL;
    T_KRTable *ptTable = NULL;
    T_DatasrcDefCur stDatasrcCur = {0};
    T_DatasrcFieldCntSel stDatasrcFieldCntSel = {0};
    
    /*create db first*/
    ptKRDB = kr_create_db(dbname);
    if (ptKRDB == NULL)
    {
        BAS_LOG(BAS_LOGERROR, iResult, 0, "kr_create_db Error!");
        return NULL;
    }
    
    /*create tables and indexes then*/
    iResult = dbsDatasrcDefCur(BAS_DBCUROPEN, &stDatasrcCur);
    if (iResult != BAS_DBOK)
    {
        BAS_LOG(BAS_LOGERROR, iResult, 0, "dbsDatasrcDefCur Open Error!");
        return NULL;
    }
    
    while(1)
    {
        iResult=dbsDatasrcDefCur(BAS_DBCURFETCH, &stDatasrcCur);
        if (iResult != BAS_DBNOTFOUND && iResult != BAS_DBOK)
        {
            BAS_LOG(BAS_LOGERROR, iResult, 0, "dbsDatasrcDefCur Fetch Error!");
            return NULL;
        }
        else if (iResult == BAS_DBNOTFOUND)
        {
            BAS_LOG(BAS_LOGDEBUG, 0, 0, "Create [%d] Tables Totally!", iCnt);
            break;
        }
        
        dbsDatasrcFieldCntSelInit(&stDatasrcFieldCntSel);
        stDatasrcFieldCntSel.lInDatasrcId = stDatasrcCur.lOutDatasrcId;
        iResult=dbsDatasrcFieldCntSel(BAS_DBSELECT, &stDatasrcFieldCntSel);
        if (iResult != BAS_DBOK)
        {
            BAS_LOG(BAS_LOGERROR, iResult, 0, "dbsDatasrcFieldCntSel Error!");
            return NULL;
        }

        ptTable = kr_create_table(ptKRDB, stDatasrcCur.lOutDatasrcId, \
                                  basRTrim(stDatasrcCur.caOutDatasrcName), \
                                  stDatasrcCur.caOutSizeKeepMode[0], \
                                  stDatasrcCur.dOutSizeKeepValue, \
                                  stDatasrcFieldCntSel.lOutFieldCnt, \
                                  (GFunc)LoadFldDefFunc);
        if (ptTable == NULL)
        {
            BAS_LOG(BAS_LOGERROR, 0, 0, "kr_create_table [%ld] Error!", \
                    stDatasrcCur.lOutDatasrcId);
            return NULL;
        }
                           
        iResult = BuildTableIndex(ptKRDB, ptTable);
        if (iResult != 0)
        {
            BAS_LOG(BAS_LOGERROR, 0, 0, "BuildTableIndex [%ld] Error!", \
                    stDatasrcCur.lOutDatasrcId);
            return NULL;
        }
        
        iCnt++;
    }

    iResult = dbsDatasrcDefCur(BAS_DBCURCLOSE, &stDatasrcCur);
    if (iResult != BAS_DBOK)
    {
        BAS_LOG(BAS_LOGERROR, iResult, 0, "dbsDatasrcDefCur Close Error!");
        return NULL;
    }
    
    return ptKRDB;
}


GList* KRDBSelect(T_KRDB *ptKRDB, int iTableId, int iIndexId, void *key)
{
    T_KRTable       *ptTable = NULL;
    T_KRIndex       *ptIndex = NULL;
    
    ptTable = kr_get_table_by_id(ptKRDB, iTableId);
    if (ptTable == NULL)
    {
        BAS_LOG(BAS_LOGERROR, 0, 0, "kr_get_table_by_id [%d] Error!", iTableId);
        return NULL;
    }
    
    ptIndex = kr_get_table_index_by_id(ptTable, iIndexId);
    if (ptIndex == NULL)
    {
        BAS_LOG(BAS_LOGERROR, 0, 0, "kr_get_table_index_by_id [%d] Error!", iIndexId);
        return NULL;
    }
    
    return kr_get_record_list(ptIndex, key);
}

int KRDBInsert(T_KRDB *ptKRDB, int iTableId, void *ptReqData)
{
    T_KRTable       *ptTable = NULL;
    T_KRRecord      *ptRecord = NULL;
    
    ptTable = kr_get_table_by_id(ptKRDB, iTableId);
    if (ptTable == NULL)
    {
        BAS_LOG(BAS_LOGERROR, 0, 0, "kr_get_table_by_id [%d] Error!", iTableId);
        return BAS_FAILURE;
    }
    ptKRDB->ptCurrTable = ptTable;
    
    ptRecord=kr_insert_record(ptTable, ptReqData);
    kr_keep_table_size(ptTable);
    ptKRDB->ptCurrRecord = ptRecord;
    
    return BAS_SUCCESS;
}

int KRDBShutdown(T_KRDB *ptKRDB)
{
    kr_drop_db(ptKRDB);
    return BAS_SUCCESS;
}


static void PrintRecord(gpointer p1, gpointer p2)
{
    T_KRRecord *ptRecord=p1;
    T_KRTable  *ptTable = (T_KRTable *)ptRecord->ptTable;
    FILE       *fp = p2;
    void *pFieldVal = NULL;
    int i = 0;
    fprintf(fp, "    Record: FieldCnt[%d] \n", ptTable->iFieldCnt);
    for (i = 0; i<ptTable->iFieldCnt; i++)
    {
        pFieldVal = kr_get_field_value(ptRecord, i);
        switch(ptTable->ptFieldDef[i].type)
        {
            case KR_INTEGER:
                fprintf(fp, "      Field:id[%3d], name[%30s], value[%d]\n", \
                        ptTable->ptFieldDef[i].id, ptTable->ptFieldDef[i].name, \
                        *(int *)pFieldVal);
                break;
            case KR_DOUBLE:
                fprintf(fp, "      Field:id[%3d], name[%30s], value[%f]\n", \
                        ptTable->ptFieldDef[i].id, ptTable->ptFieldDef[i].name, \
                        *(double *)pFieldVal);
                break;
            case KR_CHAR:
                fprintf(fp, "      Field:id[%3d], name[%30s], value[%s]\n", \
                        ptTable->ptFieldDef[i].id, ptTable->ptFieldDef[i].name, \
                        (char *)pFieldVal);
                break;
            default:
                break;           
        }
    }
}

static void PrintNode(gpointer key, gpointer value, gpointer user_data)
{
    GList       *pHashList=value;
    FILE        *fp = user_data;
    fprintf(fp, "  Node:key=[%s]\n", (char *)key);
    
    g_list_foreach(pHashList, PrintRecord, fp);
}

static void PrintIndex(gpointer p1, gpointer p2)
{
    T_KRIndex     *ptIndex=p1;
    FILE          *fp = p2;
    fprintf(fp, "  Dump Index [%d]......\n", ptIndex->iIndexId);
    fprintf(fp, "  Index: iIndexId[%d], caIndexName[%s], cIndexType[%c], iIndexFieldId[%d]\n", \
            ptIndex->iIndexId, ptIndex->caIndexName, ptIndex->cIndexType, ptIndex->iIndexFieldId);
    
    g_hash_table_foreach(ptIndex->pHashIndex, PrintNode, fp);
}

static void DumpTableWithIndex(gpointer p1, gpointer p2)
{
    T_KRTable     *ptTable=p1;
    FILE          *fp = p2;
    fprintf(fp, "Dump Table [%d] With Index......\n", ptTable->iTableId);
    fprintf(fp, "Table: iTableId[%d], caTableName[%s], cSizeKeepMode[%c], dSizeKeepValue[%f], iFieldCnt[%d], iRecordSize=[%d]\n", \
                 ptTable->iTableId, ptTable->caTableName, ptTable->cSizeKeepMode, \
                 ptTable->dSizeKeepValue, ptTable->iFieldCnt, ptTable->iRecordSize);
    
    g_list_foreach(ptTable->pIndexList, PrintIndex, fp);
}

static void DumpTableOnlyRecord(gpointer p1, gpointer p2)
{
    int           i = 0;
    T_KRTable     *ptTable=p1;
    FILE          *fp = p2;
    T_KRRecord    *ptRecord = NULL;
    fprintf(fp, "Dump Table[%d] Only Record......\n", ptTable->iTableId);
    fprintf(fp, "Table:iTableId[%d], caTableName[%s], cSizeKeepMode[%c], dSizeKeepValue[%f], iFieldCnt[%d], iRecordSize=[%d]\n", \
                 ptTable->iTableId, ptTable->caTableName, ptTable->cSizeKeepMode, \
                 ptTable->dSizeKeepValue, ptTable->iFieldCnt, ptTable->iRecordSize);

    for (i=0; i<ptTable->uiRecordNum; i++)
    {
        ptRecord = (T_KRRecord *)&ptTable->pRecordBuff[i * ptTable->iRecordSize];
        PrintRecord(ptRecord, fp);
    }
}

void DumpKRDB(FILE *fp, T_KRDB *ptKRDB, int iInd)
{
    fprintf(fp, "Dump KRDB[%s]......\n", ptKRDB->caDBName);
    if (iInd == 0)
        g_list_foreach(ptKRDB->pTableList, DumpTableWithIndex, fp);
    else
        g_list_foreach(ptKRDB->pTableList, DumpTableOnlyRecord, fp);
}


void DumpFieldDef(FILE *fp, T_KRDB *ptKRDB, int iTableId)
{
    int i = 0;
    T_KRTable *ptTable = NULL;
    T_KRFieldDef *ptFieldDef = NULL;
    
    fprintf(fp, "Dump FieldDef of Table[%d]......\n", iTableId);
    
    ptTable = kr_get_table_by_id(ptKRDB, iTableId);
    if (ptTable == NULL)
    {
        fprintf(fp, "Can't Find Table!\n");
        return;
    }
    
    ptFieldDef = &ptTable->ptFieldDef[0];
    for(i = 0; i<ptTable->iFieldCnt; i++, ptFieldDef++)
    {
         fprintf(fp, "    Field:id[%-3d], name[%-30s], type[%c], length[%-3d], precision[%-3d],scale[%3d]\n", \
         ptFieldDef->id, ptFieldDef->name, ptFieldDef->type, ptFieldDef->length, \
         ptFieldDef->precision, ptFieldDef->scale);
    }
    
}

