#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

/*sqlext.h --> sql.h --> sqltypes.h*/
#include "sql.h"
#include "sqlext.h"

#define KR_SQL_TYPE_INT     1
#define KR_SQL_TYPE_LONG    2
#define KR_SQL_TYPE_DOUBLE  3
#define KR_SQL_TYPE_CHAR    4

#define MON_SPLIT "MON_SETTLE_DT"
#define DAY_SPLIT "DAY_SETTLE_DT"
static char gcSplitTableFlag; /*'N'-no split, 'M'-Monthly, 'D'-Daily*/
static int  giSplitColInDex; /*split field id*/

static char hszSqlStmt[2048];
static char gszInputSql[2048+1] = {0};
static char gszHeaderName[50+1] = {0};
static char gszHeadDefine[50+1] = {0};
static char gszRegulatedName[50+1] = {0};
static char gszCfgFileName[200+1] = {0};
static char gszHeadFileName[200+1] = {0};
static char gszSrcFileName[200+1] = {0};

static int giInputColumns=0;
static int giOutputColumns=0;

typedef struct _dbs_env_t
{
    SQLHENV    henv;
    SQLHDBC    hdbc;
    SQLHSTMT   hstmt;
}T_DbsEnv;

typedef struct _col_desc_t
{
    int  iDir;             /* 0 - Input ; 1 - Output */
    int  iType;            /* Column Type Number */
    char caColType[10];    /* Column Type Description */
    char caSqlType[20];    /* Column Type SQL Description */
    char caFieldName[100]; /* Column Field Name */
    char caPrefix[10];     /* Column Prefix */
    int  iLength;          /* Column Length */
    int  iScale;           /* Column Scale */
}T_ColDesc;
T_ColDesc gtColumns[256] = {{0}};

/* function declaration */
static T_DbsEnv *ConnectDB(void);
static int DisconnectDB(T_DbsEnv *dbsenv);
static void RegulateName(char *name);
static int ParseParameter(int argc, char *argv[]);
static int GetDynStatement(void);
static int ParseToken(T_ColDesc *ptCol, char *pszToken);
static int RetrieveInputParams(void);
static int RetrieveOutputParams(void);
static int ConstructHostSql(FILE *fp);
static int GenComment(FILE *fp);
static int GenVarDefinition(FILE *fp,int iArea);
static int GenCurHeader(FILE *fp);
static int GenUpdHeader(FILE *fp);
static int GenSelHeader(FILE *fp);
static int GenCurImplementation(FILE *fp);
static int GenUpdImplementation(FILE *fp);
static int GenSelImplementation(FILE *fp);


int main(int argc, char *argv[])
{
    FILE *fpHead=NULL, *fpSrc=NULL;

    /* Parse command line arguments*/
    if (ParseParameter(argc, argv) != 0) {
        fprintf(stderr, "ParseParameter Error! \n\n");
        exit(-1);
    }

    /* Get sql from the .cfg file */
    if (GetDynStatement() != 0) {
        fprintf(stderr, "GetDynStatement Error! \n\n");
        exit(-1);
    }

    /* Retrieve input params with statement */
    if (RetrieveInputParams()) {
        fprintf(stderr, "RetrieveInputParams Error! \n\n");
        exit(-1);
    }

    /* Retrienv out params with sql description */
    if (RetrieveOutputParams()) {
        fprintf(stderr, "RetrieveOutputParams Error! \n\n");
        exit(-1);
    }

    /* Open header file */
    if ((fpHead = fopen(gszHeadFileName, "wt")) == NULL) {
        fprintf(stderr, "open headfile %s error:[%s]\n", \
                gszHeadFileName, strerror(errno));
        return -1;
    }

    /* Open source file */
    if ((fpSrc = fopen(gszSrcFileName, "wt")) == NULL) {
        fprintf(stderr, "open srcfile %s error:[%s]\n", \
                gszSrcFileName, strerror(errno));
        return -1;
    }

    /* Generate files according by config file name */
    if ( strstr(gszCfgFileName, "_cur.cfg") != 0 ) {
        if (GenCurHeader(fpHead))
            exit(-1);
        if (GenCurImplementation(fpSrc))
            exit(-1);
    } else if ( strstr(gszCfgFileName, "_upd.cfg") != 0 ||
            strstr(gszCfgFileName, "_ins.cfg") != 0 ||
            strstr(gszCfgFileName, "_del.cfg") != 0 ) {
        if (GenUpdHeader(fpHead))
            exit(-1);
        if (GenUpdImplementation(fpSrc))
            exit(-1);
    } else if( strstr(gszCfgFileName, "_sel.cfg") != 0 ) {
        if (GenSelHeader(fpHead))
            exit(-1);
        if (GenSelImplementation(fpSrc))
            exit(-1);
    } else {
        fprintf(stderr, "file name:[%s] error\n", gszCfgFileName );
        exit(-1);
    }

    fclose(fpHead);
    fclose(fpSrc);
    exit(0);
}


static T_DbsEnv *ConnectDB(void)
{
    SQLRETURN rc = SQL_SUCCESS; 
    char  hszDBName[128];
    char  hszDBUser[128];
    char  hszDBPass[32];

    if (getenv("DBUSER") != NULL) {    
        strncpy(hszDBUser, getenv("DBUSER"), sizeof(hszDBUser));
    }
    if (getenv("DBPASS") != NULL) {    
        strncpy(hszDBPass, getenv("DBPASS"), sizeof(hszDBPass));
    }
    if (getenv("DBNAME") != NULL) {    
        strncpy(hszDBName, getenv("DBNAME"), sizeof(hszDBName));
    }

    printf("DBNAME:[%s], DBUSER:[%s], DBPASS:[%s].\n", \
            hszDBName, hszDBUser, hszDBPass);

    T_DbsEnv *dbsenv = calloc(1, sizeof(T_DbsEnv));
    if (dbsenv == NULL) {
        fprintf(stderr, "calloc DbsEnv failed:[%d]!\n", rc);
        goto failure;
    }

    rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &dbsenv->henv);
    if (rc != SQL_SUCCESS) {
        fprintf(stderr, "SQLAllocHandle ENV failed:[%d]!\n", rc);
        goto failure;
    }
    
    rc = SQLSetEnvAttr(dbsenv->henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0);
    if (rc != SQL_SUCCESS) {
        fprintf(stderr, "SQLSetEnvAttr failed:[%d]!\n", rc);
        goto failure;
    }

    rc = SQLAllocHandle(SQL_HANDLE_DBC, dbsenv->henv, &dbsenv->hdbc);
    if (rc != SQL_SUCCESS) {
        fprintf(stderr, "SQLAllocHandle DBC failed:[%d]!\n", rc);
        goto failure;
    }

    /*
    rc = SQLSetConnectAttr(dbsenv->hdbc, SQL_ATTR_ODBC_CURSORS, (SQLPOINTER) SQL_CUR_USE_ODBC, 0);
    if (rc != SQL_SUCCESS) {
        fprintf(stderr, "SQLSetConnectAttr failed:[%d]!\n", rc);
        goto failure;
    }
    */

    rc = SQLConnect(dbsenv->hdbc, (SQLCHAR *)hszDBName, SQL_NTS, (SQLCHAR *)hszDBUser, SQL_NTS, (SQLCHAR *)hszDBPass, SQL_NTS );
    if (rc != SQL_SUCCESS) {
        fprintf(stderr, "SQLConnect failed:[%d]!\n", rc);
        goto failure;
    }
    return dbsenv;

failure:
    if (dbsenv) {
        if (dbsenv->hdbc) {
            SQLDisconnect(dbsenv->hdbc);
            SQLFreeHandle(SQL_HANDLE_DBC, dbsenv->hdbc);
        }
        if (dbsenv->henv) SQLFreeHandle(SQL_HANDLE_ENV, dbsenv->henv);
        free(dbsenv);
    }
    return NULL;
}


static int DisconnectDB(T_DbsEnv *dbsenv)
{
    if (dbsenv) {
        if (dbsenv->hdbc) {
            SQLDisconnect(dbsenv->hdbc);
            SQLFreeHandle(SQL_HANDLE_DBC, dbsenv->hdbc);
        }
        if (dbsenv->henv) SQLFreeHandle(SQL_HANDLE_ENV, dbsenv->henv);
        free(dbsenv);
    }
    return 0;
}


static void RegulateName(char *name)
{
    char *p1=name, *p2=name;
    int flag = 1;

    while (*p1)
    {
        if (*p1 == '_') {
            p1++;
            flag = 1;
            continue;
        }
        if (flag)
            *p2 = toupper(*p1);
        else
            *p2 = tolower(*p1);

        flag = 0;
        p2++;
        p1++;
    }

    *p2 = 0;
}


static int ParseParameter(int argc, char *argv[])
{
    int i = 0;
    char szArgName[200+1] = {0};

    /* argument check */
    if (argc != 2) {
        fprintf(stderr, "USAGE: %s <name>.cfg\n", argv[0]);
        return -1;
    }
    strcpy(szArgName, argv[1]);

    /* get header name */
    memset(gszHeaderName, 0x00, sizeof(gszHeaderName));
    strcpy(gszHeaderName, &szArgName[9]);
    for(i = 0; i<strlen(gszHeaderName); i++) {
        gszHeadDefine[i] = toupper(gszHeaderName[i]);
    }

    /* get regulated name */
    strcpy(gszRegulatedName, gszHeaderName);
    RegulateName(gszRegulatedName);

    /* get configure file name */
    memset(gszCfgFileName, 0x00, sizeof(gszCfgFileName));
    strcpy(gszCfgFileName, szArgName);
    strcat(gszCfgFileName, ".cfg");

    /* get header file name */
    memset(gszHeadFileName, 0x00, sizeof(gszHeadFileName));
    sprintf(gszHeadFileName, "%s.h", &szArgName[9]);

    /* get c source file name */
    memset(gszSrcFileName, 0x00, sizeof(gszSrcFileName));
    sprintf(gszSrcFileName, "%s.c", szArgName);

    printf("gszHeaderName=[%s], gszHeadDefine=[%s], gszRegulatedName=[%s]\n", 
            gszHeaderName, gszHeadDefine, gszRegulatedName);
    printf("gszCfgFileName =[%s], gszHeadFileName=[%s], gszSrcFileName=[%s]\n",
            gszCfgFileName, gszHeadFileName, gszSrcFileName);

    return 0;
}


static int GetDynStatement(void)
{
    char *cp;
    char linebuf[1024];
    FILE *fpcfg = NULL;

    if ((fpcfg = fopen(gszCfgFileName, "rt")) == NULL) {
        fprintf(stderr, "fopen configure file [%s] error£º%s\n", \
                gszCfgFileName, strerror(errno));
        return errno;
    }

    memset(gszInputSql, 0x00, sizeof(gszInputSql));
    while (fgets(linebuf, sizeof(linebuf), fpcfg) != NULL)
    {
        for (cp = linebuf; *cp; cp++) {
            if ((*cp == '\r')||(*cp == '\n'))
                *cp = ' ';
            else
                *cp=toupper(*cp);
        }

        strcat(gszInputSql, linebuf);
        strcat(gszInputSql, " ");
    }

    fclose(fpcfg);
    return 0;
}


static int ParseToken(T_ColDesc *ptCol, char *pszType)
{
    char szLen[10]={0};

    ptCol->iDir = 0;
    if (memcmp(pszType, "INT", 3) == 0) {
        ptCol->iType = KR_SQL_TYPE_INT;
        strcpy(ptCol->caColType, "int");
        strcpy(ptCol->caSqlType, "SQL_C_LONG");
        strcpy(ptCol->caPrefix, "i");
    } else if (memcmp(pszType, "LONG", 4) == 0) {
        ptCol->iType = KR_SQL_TYPE_LONG;
        strcpy(ptCol->caColType, "long");
        strcpy(ptCol->caSqlType, "SQL_C_LONG");
        strcpy(ptCol->caPrefix, "l");
    } else if (memcmp(pszType, "DOUBLE", 4) == 0) {
        ptCol->iType = KR_SQL_TYPE_DOUBLE;
        strcpy(ptCol->caColType, "double");
        strcpy(ptCol->caSqlType, "SQL_C_DOUBLE");
        strcpy(ptCol->caPrefix, "d");
    } else if(memcmp(pszType, "CHAR", 4) == 0) {
        ptCol->iType = KR_SQL_TYPE_CHAR;
        strcpy(ptCol->caColType, "char");
        strcpy(ptCol->caSqlType, "SQL_C_CHAR");
        strcpy(ptCol->caPrefix, "ca");

        char *p = strchr(pszType, '(') + 1;
        char *p1 = &szLen[0];
        while(*p != '\0')
        {
            *p1++ = *p++;
            if(*p == ')') {
                *p1 = '\0';
                break;
            }
        }
        ptCol->iLength = atoi(szLen);
        if(ptCol->iLength <=0) {
            fprintf(stderr, "Length=[%s] Error!\n", szLen);
            return -1;
        }
    } else {
        fprintf(stderr, "Unknown Type=[%s]!\n", pszType);
        return -1;
    }
    strcat(ptCol->caPrefix, "In");

    return 0;
}


static int RetrieveInputParams(void)
{
    int iCol = 0;
    char *pszToken = NULL;
    T_ColDesc tCol={0};
    gcSplitTableFlag = 'N';
    memset(hszSqlStmt, 0x00, sizeof(hszSqlStmt));
    
    pszToken = strtok(gszInputSql, ":");
    while (pszToken)
    {
        strcat(hszSqlStmt, pszToken);
        pszToken = strtok(NULL, "#");
        if (!pszToken)
            break;

        /* Get Column Name */
        /*
        strcat(hszSqlStmt, ":");
        strcat(hszSqlStmt, pszToken);
        */
        strcat(hszSqlStmt, " ? ");
         
        /* add for split table*/
        if (memcmp(pszToken, MON_SPLIT, strlen(MON_SPLIT)) == 0) {
            gcSplitTableFlag = 'M';
            giSplitColInDex  = iCol;
            printf("***********Month Splited Table!***********\n");
        } else if (memcmp(pszToken, DAY_SPLIT, strlen(DAY_SPLIT)) == 0) {
            gcSplitTableFlag = 'D';
            giSplitColInDex  = iCol;
            printf("*********** Day  Splited Table!***********\n");
        }

        /* copy and regulate field name */
        strcpy(tCol.caFieldName, pszToken);
        RegulateName(tCol.caFieldName);

        /* Get Type */
        pszToken = strtok(NULL, "#");
        if (!pszToken) {
            fprintf(stderr, "Need #Type# Define.\n");
            return -1;
        }
        if (ParseToken(&tCol, pszToken) != 0) {
            fprintf(stderr, "ParseToken [%s] Error.\n", pszToken);
            return -1;
        }

        /* Get The Next */
        pszToken = strtok(NULL, ":");
        if (!pszToken) {
            fprintf(stderr, "delimiter not match!.\n");
            return -1;
        }

        memcpy(&gtColumns[iCol], &tCol, sizeof(T_ColDesc));
        iCol++;
    }

    giInputColumns = iCol;
    return 0;
}


static int RetrieveOutputParams(void)
{
    int i = 0;
    SQLSMALLINT iNumCols = 0;
    SQLRETURN rc = SQL_SUCCESS;
    T_DbsEnv  *ptDbsEnv;
    T_ColDesc tColDesc;

    struct _col_def_t
    {
        SQLCHAR        caColName[100];
        SQLSMALLINT    iColNameSize;
        SQLSMALLINT    iType;
        SQLULEN        iLength;
        SQLSMALLINT    iScale;
        SQLSMALLINT    iNullable;
    } tColDef;

    /* connect db */
    ptDbsEnv = ConnectDB();
    if (ptDbsEnv == NULL) {
        fprintf(stderr, "ConnectDB Error! \n\n");
        return -1;
    }

    /* alloc statement handler */
    rc = SQLAllocHandle(SQL_HANDLE_STMT, ptDbsEnv->hdbc, &ptDbsEnv->hstmt);
    if (rc != SQL_SUCCESS) {
        fprintf(stderr, "SQLAllocHandle stmt failed:[%d]!\n", rc);
        return -1;
    }
    
    /* prepare sql statement */
    rc = SQLPrepare(ptDbsEnv->hstmt, (SQLCHAR *)hszSqlStmt, SQL_NTS);
    if (rc != SQL_SUCCESS) {
        fprintf(stderr, "SQLPrepare [%s] failed:[%d]!\n", hszSqlStmt, rc);
        return -1;
    }

    /* get output result columns */
    rc = SQLNumResultCols(ptDbsEnv->hstmt, &iNumCols);
    if (rc != SQL_SUCCESS) {
        fprintf(stderr, "SQLNumResultCols %s failed:[%d]!\n", hszSqlStmt, rc);
        return -1;
    }

    giOutputColumns = iNumCols;
    for (i=1; i<=iNumCols; i++)
    {        
        memset(&tColDef, 0x00, sizeof(tColDef));
        rc = SQLDescribeCol(ptDbsEnv->hstmt, i, 
                (SQLCHAR *)tColDef.caColName,
                (SQLSMALLINT )sizeof(tColDef.caColName), 
                (SQLSMALLINT *)&tColDef.iColNameSize,
                (SQLSMALLINT *)&tColDef.iType, 
                (SQLULEN *)&tColDef.iLength, 
                (SQLSMALLINT *)&tColDef.iScale, 
                (SQLSMALLINT *)&tColDef.iNullable);
        if (rc != SQL_SUCCESS) {
            fprintf(stderr, "SQLDescribeCol field [%d] failed:[%d]!\n", i, rc);
            return -1;
        }

        printf("get field [%s] (%d) (%d) (%d) (%s)\n", \
                (char *)tColDef.caColName,
                tColDef.iType, (int )tColDef.iLength, (int )tColDef.iScale,
                (char *)tColDef.caColName);
        /* set column attribute */
        memset(&tColDesc, 0x00, sizeof(tColDesc));
        switch (tColDef.iType)
        {
            case SQL_CHAR:
            case SQL_VARCHAR:
                tColDesc.iType = KR_SQL_TYPE_CHAR;
                strcpy(tColDesc.caColType, "char");
                strcpy(tColDesc.caSqlType, "SQL_C_CHAR");
                strcpy(tColDesc.caPrefix, "ca");    
                strcat(tColDesc.caPrefix, "Out");
                break;
            case SQL_DECIMAL:
            case SQL_NUMERIC:
            case SQL_FLOAT:
            case SQL_REAL:
            case SQL_DOUBLE:
                if ((tColDef.iScale > 0) || (tColDef.iLength > 19)) {
                    tColDesc.iType = KR_SQL_TYPE_DOUBLE;
                    strcpy(tColDesc.caColType, "double");
                    strcpy(tColDesc.caSqlType, "SQL_C_DOUBLE");
                    strcpy(tColDesc.caPrefix, "d");
                    strcat(tColDesc.caPrefix, "Out");
                } else {
                    tColDesc.iType = KR_SQL_TYPE_LONG;
                    strcpy(tColDesc.caColType, "long");
                    strcpy(tColDesc.caSqlType, "SQL_C_LONG");
                    strcpy(tColDesc.caPrefix, "l");
                    strcat(tColDesc.caPrefix, "Out");
                }
                break;
            case SQL_INTEGER:
            case SQL_SMALLINT:
                tColDesc.iType = KR_SQL_TYPE_LONG;     
                strcpy(tColDesc.caColType, "long");
                strcpy(tColDesc.caSqlType, "SQL_C_LONG");
                strcpy(tColDesc.caPrefix, "l");
                strcat(tColDesc.caPrefix, "Out");
                break;
            case -5: /* mysql let count field type -5...*/
            case SQL_DATETIME:
            case SQL_TYPE_DATE:
            case SQL_TYPE_TIME:
            case SQL_TYPE_TIMESTAMP:
                tColDesc.iType = KR_SQL_TYPE_LONG;     
                strcpy(tColDesc.caColType, "long");
                strcpy(tColDesc.caSqlType, "SQL_C_LONG");
                strcpy(tColDesc.caPrefix, "l");
                strcat(tColDesc.caPrefix, "Out");
                break;
            case -1: /*sqlite let varchar(>500) field type -1...*/
            case -8: /* mysql let char field type -8...*/
            case -9: /* mysql let varchar field type -9...*/
                tColDesc.iType = KR_SQL_TYPE_CHAR;
                strcpy(tColDesc.caColType, "char");
                strcpy(tColDesc.caSqlType, "SQL_C_CHAR");
                strcpy(tColDesc.caPrefix, "ca");    
                strcat(tColDesc.caPrefix, "Out");
                break;
            default:
                printf("unsupported type(%d) (%d) (%s)\n", \
                        tColDef.iType, (int )tColDef.iLength, 
                        (char *)tColDef.caColName);
                return -1;
        }
        tColDesc.iDir = 1;
        tColDesc.iLength = tColDef.iLength;
        tColDesc.iScale = tColDef.iScale;
        strcpy(tColDesc.caFieldName, (char *)tColDef.caColName);
        RegulateName(tColDesc.caFieldName);
        memcpy(&gtColumns[giInputColumns+i-1], &tColDesc, sizeof(T_ColDesc));
    }
    
    DisconnectDB(ptDbsEnv);
    return 0;
}


int GenComment(FILE *fp)
{
    char caTime[1024] = {0};
    time_t t;
    struct tm *ptm;
    t = time(NULL);
    ptm = localtime(&t);
    strftime(caTime, sizeof(caTime), "%c", ptm);
    fprintf(fp, "/******************************************************************************\n");
    fprintf(fp, " *             This file was generated by rapodbc automatically.              *\n");
    fprintf(fp, " *                     Don't modify it by yourself!                           *\n");
    fprintf(fp, " *   Author: Tiger(https://github.com/AbsoluteTiger/rapodbc)                  *\n");
    fprintf(fp, " *      Time: %s                               *\n", caTime);
    fprintf(fp, " ******************************************************************************/\n");
    fprintf(fp, "\n");
    return 0;
}


int GenVarDefinition(FILE *fp, int iArea)
{
    int i, c;
    c = giInputColumns + giOutputColumns;
    /* input first, then output*/
    for (i=0; i<c; i++)
    {
        switch(gtColumns[i].iType)
        {
            case KR_SQL_TYPE_CHAR:
                fprintf(fp, "    %-12s%s%s[%d+1];", \
                        gtColumns[i].caColType, 
                        gtColumns[i].caPrefix, 
                        gtColumns[i].caFieldName, 
                        gtColumns[i].iLength);
                break;
            default:
                fprintf(fp, "    %-12s%s%s;", \
                        gtColumns[i].caColType, 
                        gtColumns[i].caPrefix, 
                        gtColumns[i].caFieldName);
                break;
        }
        fprintf(fp,"\n");
    }
    return 0;
}


int GenCurHeader(FILE *fp)
{
    GenComment(fp);
    fprintf(fp, "#ifndef __%s_H__\n", gszRegulatedName);
    fprintf(fp, "#define __%s_H__\n\n", gszRegulatedName);
    fprintf(fp, "#include \"dbs/dbs_basopr.h\"\n\n");

    fprintf(fp, "typedef struct \n");
    fprintf(fp, "{\n");
    fprintf(fp, "    SQLHSTMT    hstmt;\n");
    GenVarDefinition(fp,0);
    fprintf(fp, "}T_%s;\n\n", gszRegulatedName);
    fprintf(fp, "extern int dbs%s(T_DbsEnv *dbsenv, int iFuncCode, T_%s *pt%s);\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "extern void dbs%sInit(T_%s *pt%s);\n",gszRegulatedName , gszRegulatedName, gszRegulatedName);

    fprintf(fp, "#endif\n\n");
    return 0;
}

int GenUpdHeader(FILE *fp)
{
    GenComment(fp);
    fprintf(fp, "#ifndef __%s_H__\n", gszRegulatedName);
    fprintf(fp, "#define __%s_H__\n", gszRegulatedName);
    fprintf(fp, "#include \"dbs/dbs_basopr.h\"\n\n");

    fprintf(fp, "typedef struct \n");
    fprintf(fp, "{\n");
    fprintf(fp, "    SQLHSTMT    hstmt;\n");
    GenVarDefinition(fp,0);
    fprintf(fp, "}T_%s;\n\n", gszRegulatedName);
    fprintf(fp, "extern int dbs%s(T_DbsEnv *dbsenv, T_%s *pt%s);\n\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);

    fprintf(fp, "#endif\n\n");
    return 0;
}


int GenSelHeader(FILE *fp)
{
    GenComment(fp);
    fprintf(fp, "#ifndef __%s_H__\n", gszRegulatedName);
    fprintf(fp, "#define __%s_H__\n", gszRegulatedName);
    fprintf(fp, "#include \"dbs/dbs_basopr.h\"\n\n");

    fprintf(fp, "typedef struct \n");
    fprintf(fp, "{\n");
    fprintf(fp, "    SQLHSTMT    hstmt;\n");
    GenVarDefinition(fp,0);
    fprintf(fp, "}T_%s;\n\n", gszRegulatedName);
    fprintf(fp, "extern int dbs%s(T_DbsEnv *dbsenv, int iFuncCode, T_%s *pt%s);\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "extern void dbs%sInit(T_%s *pt%s);\n",gszRegulatedName , gszRegulatedName, gszRegulatedName);

    fprintf(fp, "#endif\n\n");
    return 0;
}

static int ConstructHostSql(FILE *fp)
{
    char szSqlStatement[5000+1];

    strcpy(szSqlStatement, hszSqlStmt);
    if (gcSplitTableFlag != 'N') {
        char *p = strstr(szSqlStatement, "01 ");
        if (p != NULL) {
            *p++ = '%';
            *p++ = '2';
            *p++ = 's';
        } else {
            fprintf(stderr, "Split-table should terminated with \"01 \"!\n");
            return -1;
        }
    }
    
    fprintf(fp, "          char hca%sSql[2048+1]={0};\n", gszRegulatedName);
    /* Add For Split Table*/
    if (gcSplitTableFlag == 'M') {
        fprintf(fp, "          char caSplitVal[2+1] = {0};\n");
        fprintf(fp, "          memcpy(caSplitVal, &htTemp%s.%s%s[4], 2);\n", 
                gszRegulatedName, gtColumns[giSplitColInDex].caPrefix, 
                gtColumns[giSplitColInDex].caFieldName);
        fprintf(fp, "          snprintf(hca%sSql, sizeof(hca%sSql), \"%s\", caSplitVal);\n", gszRegulatedName, gszRegulatedName, szSqlStatement);
    } else if (gcSplitTableFlag == 'D') {
        fprintf(fp, "          char caSplitVal[2+1] = {0};\n");
        fprintf(fp, "          memcpy(caSplitVal, &htTemp%s.%s%s[6], 2);\n", 
                gszRegulatedName, gtColumns[giSplitColInDex].caPrefix, 
                gtColumns[giSplitColInDex].caFieldName);
        fprintf(fp, "          snprintf(hca%sSql, sizeof(hca%sSql), \"%s\", caSplitVal);\n", gszRegulatedName, gszRegulatedName, szSqlStatement);
    } else {
        fprintf(fp, "          strcpy(hca%sSql, \"%s\");\n", 
                gszRegulatedName, szSqlStatement);
    }

    return 0;
}

int GenCurImplementation(FILE *fp)
{
    GenComment(fp);

    fprintf(fp, "#include <stdio.h> \n");
    fprintf(fp, "#include <stdlib.h> \n");
    fprintf(fp, "#include <string.h> \n\n");
    fprintf(fp, "#include \"%s.h\"\n\n", gszHeaderName);

    /* struct initial */
    fprintf(fp, "void dbs%sInit(T_%s *pt%s)\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "{\n");
    fprintf(fp, "    memset(pt%s, 0x00, sizeof(T_%s));\n", gszRegulatedName, gszRegulatedName);
    fprintf(fp, "}\n\n\n");

    /* cursor operation functions */
    fprintf(fp, "int dbs%s(T_DbsEnv *dbsenv, int iFuncCode, T_%s *pt%s)\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "{\n");
    fprintf(fp, "    SQLRETURN rc = SQL_SUCCESS;\n");
    fprintf(fp, "    SQLLEN iLen = 0, iNTS = SQL_NTS;\n");
    fprintf(fp, "    switch (iFuncCode)\n");
    fprintf(fp, "    {\n");
    /* open cursor */
    fprintf(fp, "      case KR_DBCUROPEN:\n");
    fprintf(fp, "      {\n");
    ConstructHostSql(fp);
    fprintf(fp, "          rc = SQLAllocHandle(SQL_HANDLE_STMT, dbsenv->hdbc, &pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "          if (rc != SQL_SUCCESS) {\n");
    fprintf(fp, "              dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "              return rc;\n");
    fprintf(fp, "          }\n");
    fprintf(fp, "          rc = SQLPrepare(pt%s->hstmt, (SQLCHAR *)hca%sSql, SQL_NTS);\n", gszRegulatedName, gszRegulatedName);
    fprintf(fp, "          if (rc != SQL_SUCCESS) {\n");
    fprintf(fp, "              dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "              return rc;\n");
    fprintf(fp, "          }\n");
    int i;
    for (i=0; i<giInputColumns; i++) {
        fprintf(fp, "          rc = SQLBindParameter(pt%s->hstmt, %d, SQL_PARAM_INPUT, %s, %s, %d, %d, %spt%s->%s%s, %d, &%s);\n", \
                gszRegulatedName, 
                i+1, 
                gtColumns[i].caSqlType, 
                gtColumns[i].caSqlType, 
                gtColumns[i].iLength, 
                gtColumns[i].iScale,  
                gtColumns[i].iType == KR_SQL_TYPE_CHAR?"":"&", \
                gszRegulatedName, 
                gtColumns[i].caPrefix, 
                gtColumns[i].caFieldName, 
                gtColumns[i].iType == KR_SQL_TYPE_CHAR?gtColumns[i].iLength+1:0,
                gtColumns[i].iType == KR_SQL_TYPE_CHAR?"iNTS":"iLen");
        fprintf(fp, "          if (rc != SQL_SUCCESS) {\n");
        fprintf(fp, "              dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
        fprintf(fp, "              return rc;\n");
        fprintf(fp, "          }\n");
    }
    fprintf(fp, "          rc = SQLExecute(pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "          if (rc != SQL_SUCCESS) {\n");
    fprintf(fp, "              dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "              return rc;\n");
    fprintf(fp, "          }\n");
    for (i=giInputColumns; i<giInputColumns + giOutputColumns; i++) {
        fprintf(fp, "          rc = SQLBindCol(pt%s->hstmt, %d, %s, %spt%s->%s%s, %d, &%s);\n", \
                gszRegulatedName, 
                i-giInputColumns+1, 
                gtColumns[i].caSqlType, 
                gtColumns[i].iType == KR_SQL_TYPE_CHAR?"":"&",
                gszRegulatedName, 
                gtColumns[i].caPrefix, 
                gtColumns[i].caFieldName, 
                gtColumns[i].iType == KR_SQL_TYPE_CHAR?gtColumns[i].iLength+1:0,
                gtColumns[i].iType == KR_SQL_TYPE_CHAR?"iNTS":"iLen");
        fprintf(fp, "          if (rc != SQL_SUCCESS) {\n");
        fprintf(fp, "              dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
        fprintf(fp, "              return rc;\n");
        fprintf(fp, "          }\n");
    }
    fprintf(fp, "          return rc;\n");
    fprintf(fp, "      }\n");

    /* fetch */
    fprintf(fp, "      case KR_DBCURFETCH:\n");
    //fprintf(fp, "          memset(pt%s, 0x00, sizeof(T_%s));\n", gszRegulatedName, gszRegulatedName);
    fprintf(fp, "          rc = SQLFetch(pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "          if (rc != SQL_SUCCESS && rc != SQL_NO_DATA) {\n");
    fprintf(fp, "              dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "              return rc;\n");
    fprintf(fp, "          }\n");
    fprintf(fp, "          return rc;\n\n");

    /* close cursor */
    fprintf(fp, "      case KR_DBCURCLOSE:\n");
    fprintf(fp, "          rc = SQLFreeHandle(SQL_HANDLE_STMT, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "          return rc;\n\n");

    /*default*/
    fprintf(fp, "      default:\n");
    fprintf(fp, "          return -1;\n");

    fprintf(fp, "  }\n");
    fprintf(fp, "}\n");

    return 0;
}

int GenUpdImplementation(FILE *fp)
{
    GenComment(fp);

    fprintf(fp, "#include <stdio.h> \n");
    fprintf(fp, "#include <stdlib.h> \n");
    fprintf(fp, "#include <string.h> \n\n");
    fprintf(fp, "#include \"%s.h\"\n\n", gszHeaderName);

    /* update functions */
    fprintf(fp, "int dbs%s(T_DbsEnv *dbsenv, T_%s *pt%s)\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "{\n");
    fprintf(fp, "    SQLRETURN  rc = SQL_SUCCESS;\n");
    fprintf(fp, "    SQLLEN iLen = 0, iNTS = SQL_NTS;\n");
    ConstructHostSql(fp);
    fprintf(fp, "    rc = SQLAllocHandle(SQL_HANDLE_STMT, dbsenv->hdbc, &pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "    if (rc != SQL_SUCCESS) {\n");
    fprintf(fp, "        dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "        SQLFreeHandle(SQL_HANDLE_STMT, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "        return rc;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "    rc = SQLPrepare(pt%s->hstmt, (SQLCHAR *)hca%sSql, SQL_NTS);\n", gszRegulatedName, gszRegulatedName);
    fprintf(fp, "    if (rc != SQL_SUCCESS) {\n");
    fprintf(fp, "        dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "        SQLFreeHandle(SQL_HANDLE_STMT, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "        return rc;\n");
    fprintf(fp, "    }\n");
    int i;
    for (i=0; i<giInputColumns; i++) {
        fprintf(fp, "    rc = SQLBindParameter(pt%s->hstmt, %d, SQL_PARAM_INPUT, %s, %s, %d, %d, %spt%s->%s%s, %d, &%s);\n", \
                gszRegulatedName, 
                i+1, 
                gtColumns[i].caSqlType, 
                gtColumns[i].caSqlType, 
                gtColumns[i].iLength, 
                gtColumns[i].iScale,
                gtColumns[i].iType == KR_SQL_TYPE_CHAR?"":"&",
                gszRegulatedName, 
                gtColumns[i].caPrefix, 
                gtColumns[i].caFieldName, 
                gtColumns[i].iLength, 
                gtColumns[i].iType == KR_SQL_TYPE_CHAR?"iNTS":"iLen");
        fprintf(fp, "    if (rc != SQL_SUCCESS) {\n");
        fprintf(fp, "        dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
        fprintf(fp, "        SQLFreeHandle(SQL_HANDLE_STMT, pt%s->hstmt);\n", gszRegulatedName);
        fprintf(fp, "        return rc;\n");
        fprintf(fp, "    }\n");
    }
    fprintf(fp, "    rc = SQLExecute(pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "    if (rc != SQL_SUCCESS) {\n");
    fprintf(fp, "        dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "        SQLFreeHandle(SQL_HANDLE_STMT, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "        return rc;\n");
    fprintf(fp, "    }\n");

    fprintf(fp, "    SQLFreeHandle(SQL_HANDLE_STMT, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "    return rc;\n");
    fprintf(fp, "}\n\n\n");
    return 0;
}


int GenSelImplementation(FILE *fp)
{
    GenComment(fp);

    fprintf(fp, "#include <stdio.h> \n");
    fprintf(fp, "#include <stdlib.h> \n");
    fprintf(fp, "#include <string.h> \n\n");
    fprintf(fp, "#include \"%s.h\"\n\n", gszHeaderName);

    /* struct initial */
    fprintf(fp, "void dbs%sInit(T_%s *pt%s)\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "{\n");
    fprintf(fp, "    memset(pt%s, 0x00, sizeof(*pt%s));\n", gszRegulatedName, gszRegulatedName);
    fprintf(fp, "}\n\n\n");

    /* operation functions */
    fprintf(fp, "int dbs%s(T_DbsEnv *dbsenv, int iFuncCode, T_%s *pt%s)\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "{\n");
    fprintf(fp, "    SQLRETURN rc = SQL_SUCCESS;\n");
    fprintf(fp, "    SQLLEN iLen = 0, iNTS = SQL_NTS;\n");
    fprintf(fp, "    switch (iFuncCode)\n");
    fprintf(fp, "    {\n");
    /* select */
    fprintf(fp, "      case KR_DBSELECT:\n");
    fprintf(fp, "      {\n");
    ConstructHostSql(fp);
    fprintf(fp, "           rc = SQLAllocHandle(SQL_HANDLE_STMT, dbsenv->hdbc, &pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "           if (rc != SQL_SUCCESS) {\n");
    fprintf(fp, "               dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "               SQLFreeHandle(SQL_HANDLE_STMT, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "               return rc;\n");
    fprintf(fp, "           }\n");
    fprintf(fp, "           rc = SQLPrepare(pt%s->hstmt, (SQLCHAR *)hca%sSql, SQL_NTS);\n", gszRegulatedName, gszRegulatedName);
    fprintf(fp, "           if (rc != SQL_SUCCESS) {\n");
    fprintf(fp, "               dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "               SQLFreeHandle(SQL_HANDLE_STMT, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "               return rc;\n");
    fprintf(fp, "           }\n");
    int i;
    for (i=0; i<giInputColumns; i++) {
        fprintf(fp, "           rc = SQLBindParameter(pt%s->hstmt, %d, SQL_PARAM_INPUT, %s, %s, %d, %d, %spt%s->%s%s, %d, &%s);\n", \
                gszRegulatedName, 
                i+1, 
                gtColumns[i].caSqlType,
                gtColumns[i].caSqlType, 
                gtColumns[i].iLength, 
                gtColumns[i].iScale, 
                gtColumns[i].iType == KR_SQL_TYPE_CHAR?"":"&", 
                gszRegulatedName, 
                gtColumns[i].caPrefix, 
                gtColumns[i].caFieldName, 
                gtColumns[i].iType == KR_SQL_TYPE_CHAR?gtColumns[i].iLength+1:0,
                gtColumns[i].iType == KR_SQL_TYPE_CHAR?"iNTS":"iLen");
        fprintf(fp, "           if (rc != SQL_SUCCESS) {\n");
        fprintf(fp, "               dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
        fprintf(fp, "               SQLFreeHandle(SQL_HANDLE_STMT, pt%s->hstmt);\n", gszRegulatedName);
        fprintf(fp, "               return rc;\n");
        fprintf(fp, "           }\n");
    }
    fprintf(fp, "           rc = SQLExecute(pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "           if (rc != SQL_SUCCESS) {\n");
    fprintf(fp, "               dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "               SQLFreeHandle(SQL_HANDLE_STMT, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "               return rc;\n");
    fprintf(fp, "           }\n");
    for (i=giInputColumns; i<giInputColumns + giOutputColumns; i++) {
        fprintf(fp, "           rc = SQLBindCol(pt%s->hstmt, %d, %s, %spt%s->%s%s, %d, &%s);\n", \
                gszRegulatedName, 
                i-giInputColumns+1, 
                gtColumns[i].caSqlType, 
                gtColumns[i].iType == KR_SQL_TYPE_CHAR?"":"&", 
                gszRegulatedName, 
                gtColumns[i].caPrefix, 
                gtColumns[i].caFieldName, 
                gtColumns[i].iType == KR_SQL_TYPE_CHAR?gtColumns[i].iLength+1:0,
                gtColumns[i].iType == KR_SQL_TYPE_CHAR?"iNTS":"iLen");
        fprintf(fp, "           if (rc != SQL_SUCCESS) {\n");
        fprintf(fp, "               dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
        fprintf(fp, "               SQLFreeHandle(SQL_HANDLE_STMT, pt%s->hstmt);\n", gszRegulatedName);
        fprintf(fp, "               return rc;\n");
        fprintf(fp, "           }\n");
    }
    fprintf(fp, "           rc = SQLFetch(pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "           if (rc != SQL_SUCCESS) {\n");
    fprintf(fp, "               dbsGetError(dbsenv, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "               SQLFreeHandle(SQL_HANDLE_STMT, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "               return rc;\n");
    fprintf(fp, "           }\n");

    fprintf(fp, "           SQLFreeHandle(SQL_HANDLE_STMT, pt%s->hstmt);\n", gszRegulatedName);
    fprintf(fp, "           return rc;\n");
    fprintf(fp, "       }\n");
    /*default*/
    fprintf(fp, "       default:\n");
    fprintf(fp, "           return -1;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "}\n");

    return 0;
}


