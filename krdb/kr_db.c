#include "kr_db.h"
#include "kr_db_mmap.h"

#include "dbs/dbs_basopr.h"
#include "dbs/dbs/datasrc_def_cur.h"
#include "dbs/dbs/datasrc_field_cur.h"
#include "dbs/dbs/datasrc_index_cur.h"
#include "dbs/dbs/datasrc_field_cnt_sel.h"
#include "dbs/dbs/datasrc_field_def_sel.h"


static void _kr_db_load_field_def(T_DbsEnv *dbsenv, T_KRFieldDef *ptFieldDef, int *piTableId);
static int  _kr_db_build_index(T_DbsEnv *dbsenv, T_KRDB *ptKRDB, T_KRTable *ptTable);
static void kr_db_dump_node(void *key, void *value, void *user_data);
static void kr_db_dump_index(void *p1, void *p2);
static void kr_db_dump_table_with_index (void *p1, void *p2);
static void kr_db_dump_table_only_record (void *p1, void *p2);

int kr_db_set_field_def(T_DbsEnv *dbsenv, T_KRFieldDef *ptFieldDef, int iTableId, int iFieldId)
{
    int iResult = 0;
    int iFlag = 0;
    int iCnt = 0;
    T_DatasrcFieldDefSel stDatasrcFieldDefSel = {0};
    
    stDatasrcFieldDefSel.lInDatasrcId = iTableId;
    stDatasrcFieldDefSel.lInFieldId = iFieldId;
    iResult = dbsDatasrcFieldDefSel(dbsenv, KR_DBSELECT, &stDatasrcFieldDefSel);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDatasrcFieldDefSel [%d],[%d] Error!",\
                stDatasrcFieldDefSel.lInDatasrcId, stDatasrcFieldDefSel.lInFieldId);
        return -1;
    }
    
    ptFieldDef->id = stDatasrcFieldDefSel.lInFieldId;
    strncpy(ptFieldDef->name, kr_string_rtrim(stDatasrcFieldDefSel.caOutFieldName), \
            sizeof(ptFieldDef->name));
    ptFieldDef->type = stDatasrcFieldDefSel.caOutFieldType[0];
    ptFieldDef->length = stDatasrcFieldDefSel.lOutFieldLength;

    return 0;
}


static void _kr_db_load_field_def(T_DbsEnv *dbsenv, T_KRFieldDef *ptFieldDef, int *piTableId)
{
    int iResult = 0;
    int iFlag = 0;
    int iCnt = 0;
    int iOffset = 0;
    T_DatasrcFieldCur stDatasrcFieldCur = {0};
    
    stDatasrcFieldCur.lInDatasrcId = *piTableId;
    iResult = dbsDatasrcFieldCur(dbsenv, KR_DBCUROPEN, &stDatasrcFieldCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDatasrcFieldCur Open Error!");
        return;
    }
    
    while(1)
    {
        iResult=dbsDatasrcFieldCur(dbsenv, KR_DBCURFETCH, &stDatasrcFieldCur);
        if (iResult != KR_DBNOTFOUND && iResult != KR_DBOK) {
            KR_LOG(KR_LOGERROR, "dbsDatasrcFieldCur Fetch Error[%d]!", iResult);
            iFlag = -1;
            break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Load [%d]Fields Of Table[%ld] Totally!", \
                    iCnt, stDatasrcFieldCur.lInDatasrcId);
            break;
        }
        
        ptFieldDef[iCnt].id = stDatasrcFieldCur.lOutFieldId;
        strncpy(ptFieldDef[iCnt].name, \
                kr_string_rtrim(stDatasrcFieldCur.caOutFieldName), \
                sizeof(ptFieldDef[iCnt].name));
        ptFieldDef[iCnt].type = stDatasrcFieldCur.caOutFieldType[0];
        ptFieldDef[iCnt].length = stDatasrcFieldCur.lOutFieldLength;
        ptFieldDef[iCnt].offset = iOffset;
        iOffset += ptFieldDef[iCnt].length;
        
        iCnt++;
    }

    iResult = dbsDatasrcFieldCur(dbsenv, KR_DBCURCLOSE, &stDatasrcFieldCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDatasrcFieldCur Close Error!");
        return;
    }
}


static int _kr_db_build_index(T_DbsEnv *dbsenv, T_KRDB *ptKRDB, T_KRTable *ptTable)
{
    int iFlag = 0;
    int iResult = 0;
    int iCnt = 0;
    T_DatasrcIndexCur stDatasrcIndexCur = {0};
    
    
    stDatasrcIndexCur.lInDatasrcId = ptTable->iTableId;
    iResult = dbsDatasrcIndexCur(dbsenv, KR_DBCUROPEN, &stDatasrcIndexCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDatasrcIndexCur Open Error!");
        return -1;
    }
    
    while(1)
    {
        iResult=dbsDatasrcIndexCur(dbsenv, KR_DBCURFETCH, &stDatasrcIndexCur);
        if (iResult != KR_DBNOTFOUND && iResult != KR_DBOK) {
            KR_LOG(KR_LOGERROR, "dbsDatasrcIndexCur Fetch Error[%d]!", iResult);
            iFlag = -1;
            break;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Build [%d] Indexes Totally!", iCnt);
            break;
        }
                           
        T_KRIndex *ptIndex=kr_create_index(ptKRDB, ptTable, \
                        stDatasrcIndexCur.lOutIndexId, \
                        kr_string_rtrim(stDatasrcIndexCur.caOutIndexName), \
                        stDatasrcIndexCur.caOutIndexType[0], \
                        stDatasrcIndexCur.lOutIndexFieldId,
                        stDatasrcIndexCur.lOutSortFieldId);
        if (ptIndex == NULL) {
            KR_LOG(KR_LOGERROR, "kr_create_index [%ld] [%s] failed!", \
                    stDatasrcIndexCur.lOutIndexId, stDatasrcIndexCur.caOutIndexType);
            break;
        }
        
        iCnt++;
    }

    iResult = dbsDatasrcIndexCur(dbsenv, KR_DBCURCLOSE, &stDatasrcIndexCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDatasrcIndexCur Close Error!");
        return -1;
    }
    
    return iFlag;
}

T_KRDB* kr_db_startup(T_DbsEnv *dbsenv, char *dbname, T_KRModule *krdbmodule)
{
    int iFlag = 0;
    int iResult = 0;
    int iCnt = 0;
    T_KRDB *ptKRDB=NULL;
    T_KRTable *ptTable = NULL;
    T_DatasrcDefCur stDatasrcCur = {0};
    T_DatasrcFieldCntSel stDatasrcFieldCntSel = {0};
    KRMapFuncPre MapFuncPre = NULL;
    KRMapFunc MapFunc = NULL;
    KRMapFuncPost MapFuncPost = NULL;
    
    /*create db first*/
    ptKRDB = kr_create_db(dbname, krdbmodule);
    if (ptKRDB == NULL) {
        KR_LOG(KR_LOGERROR, "kr_create_db %s error!", dbname);
        return NULL;
    }
    
    /*create tables and indexes then*/
    iResult = dbsDatasrcDefCur(dbsenv, KR_DBCUROPEN, &stDatasrcCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDatasrcDefCur Open Error[%d]!", iResult);
        return NULL;
    }
    
    while(1)
    {
        iResult=dbsDatasrcDefCur(dbsenv, KR_DBCURFETCH, &stDatasrcCur);
        if (iResult != KR_DBNOTFOUND && iResult != KR_DBOK) {
            KR_LOG(KR_LOGERROR, "dbsDatasrcDefCur Fetch Error[%d]!", iResult);
            return NULL;
        } else if (iResult == KR_DBNOTFOUND) {
            KR_LOG(KR_LOGDEBUG, "Create [%d] Tables Totally!", iCnt);
            break;
        }
        
        dbsDatasrcFieldCntSelInit(&stDatasrcFieldCntSel);
        stDatasrcFieldCntSel.lInDatasrcId = stDatasrcCur.lOutDatasrcId;
        iResult=dbsDatasrcFieldCntSel(dbsenv, KR_DBSELECT, &stDatasrcFieldCntSel);
        if (iResult != KR_DBOK) {
            KR_LOG(KR_LOGERROR, "dbsDatasrcFieldCntSel Error!");
            return NULL;
        }

        KR_LOG(KR_LOGDEBUG, "Datasrc:Id[%ld], Name[%s], Desc[%s], Usage[%s],"
                "MmapFileName[%s], MapFuncPre[%s], MapFunc[%s], "
                "MapFuncPost[%s], SizeKeepMode[%s], "
                "SizeKeepValue[%ld] FieldCnt[%ld]\n",\
                stDatasrcCur.lOutDatasrcId, \
                stDatasrcCur.caOutDatasrcName, \
                stDatasrcCur.caOutDatasrcDesc, \
                stDatasrcCur.caOutDatasrcUsage, \
                stDatasrcCur.caOutMmapFileName, \
                stDatasrcCur.caOutMapFuncPre, \
                stDatasrcCur.caOutMapFunc, \
                stDatasrcCur.caOutMapFuncPost, \
                stDatasrcCur.caOutSizeKeepMode, \
                stDatasrcCur.lOutSizeKeepValue, \
                //TODO:sqlite3 let lOutFieldCnt->caOutFieldCnt
                stDatasrcFieldCntSel.lOutFieldCnt);        

        kr_string_rtrim(stDatasrcCur.caOutDatasrcName);
        kr_string_rtrim(stDatasrcCur.caOutMapFuncPre);
        kr_string_rtrim(stDatasrcCur.caOutMapFunc);
        kr_string_rtrim(stDatasrcCur.caOutMapFuncPost);
        kr_string_rtrim(stDatasrcCur.caOutMmapFileName);
        if (stDatasrcCur.caOutMapFuncPre[0] != '\0') {
            MapFuncPre = (KRMapFuncPre )kr_module_symbol(
                    ptKRDB->ptModule, stDatasrcCur.caOutMapFuncPre);
            if (MapFuncPre == NULL) {
                KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", \
                        stDatasrcCur.caOutMapFuncPre);
                return NULL;
            }
        }
        if (stDatasrcCur.caOutMapFuncPost[0] != '\0') {
            MapFuncPost = (KRMapFuncPost )kr_module_symbol(
                    ptKRDB->ptModule, stDatasrcCur.caOutMapFuncPost);
            if (MapFuncPost == NULL) {
                KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", \
                        stDatasrcCur.caOutMapFuncPost);
                return NULL;
            }
        }
        if (stDatasrcCur.caOutMapFunc[0] != '\0') {
            MapFunc = (KRMapFunc )kr_module_symbol(
                    ptKRDB->ptModule, stDatasrcCur.caOutMapFunc);
            if (MapFunc == NULL) {
                KR_LOG(KR_LOGERROR, "kr_module_symbol [%s] error!", \
                        stDatasrcCur.caOutMapFunc);
                return NULL;
            }
        } else {
            KR_LOG(KR_LOGERROR, "datasrc[%ld] map_func should be specified!", \
                    stDatasrcCur.lOutDatasrcId);
            return NULL;
        }

        ptTable = kr_create_table(ptKRDB, 
                stDatasrcCur.lOutDatasrcId, \
                stDatasrcCur.caOutDatasrcName, \
                stDatasrcCur.caOutMmapFileName, \
                stDatasrcCur.caOutSizeKeepMode[0], \
                stDatasrcCur.lOutSizeKeepValue, \
                //TODO:sqlite3 let lOutFieldCnt->caOutFieldCnt
                stDatasrcFieldCntSel.lOutFieldCnt, \
                (KRLoadDefFunc)_kr_db_load_field_def, \
                (KRMapFuncPre)MapFuncPre,
                (KRMapFunc)MapFunc,
                (KRMapFuncPost)MapFuncPost,
                dbsenv);
        if (ptTable == NULL) {
            KR_LOG(KR_LOGERROR, "kr_create_table [%ld] Error!", \
                    stDatasrcCur.lOutDatasrcId);
            return NULL;
        }
                           
        iResult = _kr_db_build_index(dbsenv, ptKRDB, ptTable);
        if (iResult != 0) {
            KR_LOG(KR_LOGERROR, "_kr_db_build_index [%ld] Error!", \
                    stDatasrcCur.lOutDatasrcId);
            return NULL;
        }
        
        iCnt++;
    }

    iResult = dbsDatasrcDefCur(dbsenv, KR_DBCURCLOSE, &stDatasrcCur);
    if (iResult != KR_DBOK) {
        KR_LOG(KR_LOGERROR, "dbsDatasrcDefCur Close Error!");
        return NULL;
    }
    
    return ptKRDB;
}


static int _kr_db_load_record(T_KRRecord *ptMMapRec, T_KRTable *ptTable)
{
    /*load check*/
    T_KRTable *ptMMapTable = (T_KRTable *)ptMMapRec->ptTable;
    if (ptMMapTable->iRecordSize != ptTable->iRecordSize) {
        KR_LOG(KR_LOGERROR, "Cant't load record, table size different!");
        return -1;
    }

    T_KRRecord *ptRecord = kr_create_record_from_mmap(ptTable, ptMMapRec);
    if (kr_insert_record(ptRecord, ptTable) != 0) {
        KR_LOG(KR_LOGERROR, "kr_insert_record Error!");
        return -1;
    }
    
    return 0;
}


static int _kr_db_load_table(T_KRTable *ptTable, void *user_data)
{
    int nRet = 0;
    /*load mmap file if it exists*/
    if (access(ptTable->caMMapFile, R_OK) == 0) {
        nRet = kr_db_mmap_file_handle(ptTable->caMMapFile, _kr_db_load_record, ptTable);
        if (nRet != 0) {
            KR_LOG(KR_LOGERROR, "load mmap file [%s] Error!", ptTable->caMMapFile);
            return -1;
        }
    }
    return 0;
}


int kr_db_load(T_KRDB *ptKRDB)
{
    kr_list_foreach(ptKRDB->pTableList, (KRForEachFunc )_kr_db_load_table, NULL);
    return 0;
}


T_KRList* kr_db_select(T_KRDB *ptKRDB, int iTableId, int iIndexId, void *key)
{
    T_KRTable       *ptTable = NULL;
    T_KRIndex       *ptIndex = NULL;
    
    ptTable = kr_get_table(ptKRDB, iTableId);
    if (ptTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_get_table [%d] Error!", iTableId);
        return NULL;
    }
    
    ptIndex = kr_get_table_index(ptTable, iIndexId);
    if (ptIndex == NULL) {
        KR_LOG(KR_LOGERROR, "kr_get_table_index[%d] Error!", iIndexId);
        return NULL;
    }
    
    return kr_get_record_list(ptIndex, key);
}


T_KRRecord *kr_db_insert(T_KRDB *ptKRDB, int iTableId, void *ptReqData)
{
    T_KRTable *ptTable = kr_get_table(ptKRDB, iTableId);
    if (ptTable == NULL) {
        KR_LOG(KR_LOGERROR, "kr_get_table [%d] Error!", iTableId);
        return NULL;
    }
    
    T_KRRecord *ptRecord = kr_create_record_from_data(ptTable, ptReqData);
    if (kr_insert_record(ptRecord, ptTable) != 0) {
        KR_LOG(KR_LOGERROR, "kr_insert_record [%d] Error!", iTableId);
        return NULL;
    }
    
    return ptRecord;
}


int kr_db_shutdown(T_KRDB *ptKRDB)
{
    kr_drop_db(ptKRDB);
    return 0;
}


void kr_db_dump_record(T_KRRecord *ptRecord, FILE *fp)
{
    T_KRTable  *ptTable = (T_KRTable *)ptRecord->ptTable;
    void *pFieldVal = NULL;
    int i = 0;
    fprintf(fp, "    Record: FieldCnt[%d] \n", ptTable->iFieldCnt);
    for (i = 0; i<ptTable->iFieldCnt; i++)
    {
        pFieldVal = kr_get_field_value(ptRecord, i);
        switch(ptTable->ptFieldDef[i].type)
        {
            case KR_FIELDTYPE_INT:
                fprintf(fp, "      Field:id[%3d], name[%30s], value[%d]\n", \
                        ptTable->ptFieldDef[i].id, ptTable->ptFieldDef[i].name, 
                        *(int *)pFieldVal);
                break;
            case KR_FIELDTYPE_LONG:
                fprintf(fp, "      Field:id[%3d], name[%30s], value[%ld]\n", \
                        ptTable->ptFieldDef[i].id, ptTable->ptFieldDef[i].name,
                        *(long *)pFieldVal);
                break;    
            case KR_FIELDTYPE_DOUBLE:
                fprintf(fp, "      Field:id[%3d], name[%30s], value[%f]\n", \
                        ptTable->ptFieldDef[i].id, ptTable->ptFieldDef[i].name, 
                        *(double *)pFieldVal);
                break;
            case KR_FIELDTYPE_STRING:
                fprintf(fp, "      Field:id[%3d], name[%30s], value[%s]\n", \
                        ptTable->ptFieldDef[i].id, ptTable->ptFieldDef[i].name, 
                        (char *)pFieldVal);
                break;
            case KR_FIELDTYPE_POINTER:
                fprintf(fp, "      Field:id[%3d], name[%30s], value[%p]\n", \
                        ptTable->ptFieldDef[i].id, ptTable->ptFieldDef[i].name, 
                        pFieldVal);
                break;
            default:
                break;           
        }
    }
}


void kr_db_dump_node(void *key, void *value, void *user_data)
{
    T_KRList       *pHashList=value;
    FILE        *fp = user_data;
    fprintf(fp, "  Node:key=[%s]\n", (char *)key);
    
    kr_list_foreach(pHashList, (KRForEachFunc )kr_db_dump_record, fp);
}


static void kr_db_dump_index(void *p1, void *p2)
{
    T_KRIndex     *ptIndex=p1;
    FILE          *fp = p2;
    fprintf(fp, "  Dump Index [%d]......\n", ptIndex->iIndexId);
    fprintf(fp, "  Index: iIndexId[%d], caIndexName[%s], eIndexType[%c], iIndexFieldId[%d], iSortFieldId[%d]\n", \
            ptIndex->iIndexId, ptIndex->caIndexName, ptIndex->eIndexType, \
            ptIndex->iIndexFieldId, ptIndex->iSortFieldId);
    
    kr_hashtable_foreach(ptIndex->pHashTable, kr_db_dump_node, fp);
}


static void kr_db_dump_table_with_index(void *p1, void *p2)
{
    T_KRTable     *ptTable=p1;
    FILE          *fp = p2;
    fprintf(fp, "Dump Table [%d] With Index......\n", ptTable->iTableId);
    fprintf(fp, "Table: iTableId[%d], caTableName[%s], eSizeKeepMode[%c], lSizeKeepValue[%ld], iFieldCnt[%d], iRecordSize=[%d]\n", \
            ptTable->iTableId, ptTable->caTableName, ptTable->eSizeKeepMode, \
            ptTable->lSizeKeepValue, ptTable->iFieldCnt, ptTable->iRecordSize);
    
    kr_list_foreach(ptTable->pIndexList, kr_db_dump_index, fp);
}


static void kr_db_dump_table_only_record(void *p1, void *p2)
{
    int           i = 0;
    T_KRTable     *ptTable=p1;
    FILE          *fp = p2;
    T_KRRecord    *ptRecord = NULL;
    fprintf(fp, "Dump Table[%d] Only Record......\n", ptTable->iTableId);
    fprintf(fp, "Table:iTableId[%d], caTableName[%s], eSizeKeepMode[%c], lSizeKeepValue[%ld], iFieldCnt[%d], iRecordSize=[%d]\n", \
            ptTable->iTableId, ptTable->caTableName, ptTable->eSizeKeepMode, \
            ptTable->lSizeKeepValue, ptTable->iFieldCnt, ptTable->iRecordSize);

    for (i=0; i<ptTable->uiRecordNum; i++) {
        ptRecord = (T_KRRecord *)&ptTable->pRecordBuff[i*ptTable->iRecordSize];
        kr_db_dump_record(ptRecord, fp);
    }
}


void kr_db_dump(T_KRDB *ptKRDB, int iInd, FILE *fp)
{
    fprintf(fp, "Dump KRDB[%s]......\n", ptKRDB->caDBName);
    if (iInd == 0)
        kr_list_foreach(ptKRDB->pTableList, kr_db_dump_table_with_index, fp);
    else
        kr_list_foreach(ptKRDB->pTableList, kr_db_dump_table_only_record, fp);
}


void kr_db_dump_field_def(T_KRDB *ptKRDB, int iTableId, FILE *fp)
{
    int i = 0;
    T_KRTable *ptTable = NULL;
    T_KRFieldDef *ptFieldDef = NULL;
    
    fprintf(fp, "Dump FieldDef of Table[%d]......\n", iTableId);
    
    ptTable = kr_get_table(ptKRDB, iTableId);
    if (ptTable == NULL) {
        fprintf(fp, "Can't Find Table!\n");
        return;
    }
    
    ptFieldDef = &ptTable->ptFieldDef[0];
    for(i = 0; i<ptTable->iFieldCnt; i++, ptFieldDef++) {
         fprintf(fp, "    Field:id[%-3d], name[%-30s], type[%c], length[%-3d]\n", \
         ptFieldDef->id, ptFieldDef->name, ptFieldDef->type, ptFieldDef->length);
    }
    
}

