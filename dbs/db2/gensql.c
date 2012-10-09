static char sqla_program_id[292] = 
{
 172,0,65,69,65,78,65,73,68,66,116,80,78,98,74,99,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,84,73,71,69,82,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,71,69,78,83,81,76,32,32,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0
};

#include "sqladef.h"

static struct sqla_runtime_info sqla_rtinfo = 
{{'S','Q','L','A','R','T','I','N'}, sizeof(wchar_t), 0, {' ',' ',' ',' '}};


static const short sqlIsLiteral   = SQL_IS_LITERAL;
static const short sqlIsInputHvar = SQL_IS_INPUT_HVAR;


#line 1 "gensql.sqc"
/*****************************************************************************/
/* PROGRAM NAME: gensql.sqc                                                  */
/* DESCRIPTIONS: gensql tool for db2                                         */
/* AUTHOR      : Tiger                                                       */
/* CREATE DATE : 2012-04-13                                                  */
/*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include <sqlenv.h>
#include <sqlcodes.h>

/*
EXEC SQL INCLUDE SQLCA;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 16 "gensql.sqc"


/*
EXEC SQL INCLUDE SQLDA;
*/

/* SQL Descriptor Area - Variable descriptor */
#include "sqlda.h"


#line 17 "gensql.sqc"



/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 19 "gensql.sqc"


/*
EXEC SQL WHENEVER SQLWARNING CONTINUE;
*/

#line 20 "gensql.sqc"


/*
EXEC SQL WHENEVER NOT FOUND CONTINUE;
*/

#line 21 "gensql.sqc"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 23 "gensql.sqc"

    char  hszDBName[128];
    char  hszDBUser[128];
    char  hszDBPass[32];
    char  hszSqlStmt[2048];

/*
EXEC SQL END DECLARE SECTION;
*/

#line 28 "gensql.sqc"


/* Maximum number of select-list items or bind variables. */
#define MAX_ITEMS         200

/* Maximum lengths of the _names_ of the
   select-list items or indicator variables. */
#define MAX_VNAME_LEN     50
#define MAX_INAME_LEN     50

#ifndef NULL
#define NULL  0
#endif

#define KR_SQL_TYPE_INT     1
#define KR_SQL_TYPE_LONG    2
#define KR_SQL_TYPE_DOUBLE  3
#define KR_SQL_TYPE_CHAR    4

static char gszInputSql[2048+1] = {0};
static char gszModuleName[3+1] = {0};
static char gszHeaderName[50+1] = {0};
static char gszHeadDefine[50+1] = {0};
static char gszRegulatedName[50+1] = {0};
static char gszCfgFileName[200+1] = {0};
static char gszHeadFileName[200+1] = {0};
static char gszSrcFileName[200+1] = {0};

static int giInputColumns=0;
static int giOutputColumns=0;

typedef struct col_def_t
{
    int  iDir;             /* 0 - Input ; 1 - Output */
    int  iType;            /* Column Type Number */
    char caColType[10];    /* Column Type Description */
    char caFieldName[100]; /* Column Field Name */
    char caPrefix[10];     /* Column Prefix */
    int  iLength;          /* Column Length */
    int  iScale;           /* Column Scale */
}T_ColDesc;

T_ColDesc gtColumns[100] = {0};

static char gcSplitTableFlag; /*'N'-非拆分表, 'M'-月拆分表, 'D'-日拆分表*/
static int  giSplitColInDex; /*拆分字段编号*/
#define MON_SPLIT "MON_SETTLE_DT"
#define DAY_SPLIT "DAY_SETTLE_DT"


static int ConnectDB(void);
static void DisconnectDB(void);
static void RegulateName(char *name);
static int ParseParameter(int argc, char *argv[]);
static int GetDynStatement(void);
static int ParseToken(T_ColDesc *ptCol, char *pszToken);
static int RetrieveInputParams(void);
static int RetrieveOutputParams(void);
static int GenVarDefinition(FILE *fp,int iArea);
static int GenCurHeader(FILE *fp);
static int GenUpdHeader(FILE *fp);
static int GenSeqHeader(FILE *fp);
static int GenSelHeader(FILE *fp);
static int GenCurImplementation(FILE *fp);
static int GenUpdImplementation(FILE *fp);
static int GenSeqImplementation(FILE *fp);
static int GenSelImplementation(FILE *fp);
static int GenComment(FILE *fp);


int main(int argc, char *argv[])
{
    FILE *fpHead=NULL,*fpSqc=NULL;

    /* Parse command line arguments*/
    if (ParseParameter(argc, argv) != 0)
    {
        fprintf(stderr, "ParseParameter Error! \n\n");
        exit(-1);
    }

    /* Get sql from the .cfg file */
    if (GetDynStatement() != 0)
    {
        fprintf(stderr, "GetDynStatement Error! \n\n");
        exit(-1);
    }

    /* Retrieve input params with statement */
    if (RetrieveInputParams())
    {
        fprintf(stderr, "RetrieveInputParams Error! \n\n");
        exit(-1);
    }

    /* Retrienv out params with sql description */
    if (RetrieveOutputParams())
    {
        fprintf(stderr, "RetrieveOutputParams Error! \n\n");
        exit(-1);
    }

    /* Open header file */
    if ((fpHead = fopen(gszHeadFileName, "wt")) == NULL)
    {
        fprintf(stderr, "open headfile %s error:[%s]\n", gszHeadFileName, strerror(errno));
        return -1;
    }

    /* Open ource file */
    if ((fpSqc = fopen(gszSrcFileName, "wt")) == NULL)
    {
        fprintf(stderr, "open srcfile %s error:[%s]\n", gszSrcFileName, strerror(errno));
        return -1;
    }

    /* Generate files according by config file name */
    if ( strstr(gszCfgFileName, "_cur.cfg") != 0 )
    {
        if (GenCurHeader(fpHead))
            exit(-1);

        if (GenCurImplementation(fpSqc))
            exit(-1);
    }
    else if ( strstr(gszCfgFileName, "_upd.cfg") != 0 ||
              strstr(gszCfgFileName, "_ins.cfg") != 0 ||
              strstr(gszCfgFileName, "_del.cfg") != 0 )
    {
        if (GenUpdHeader(fpHead))
            exit(-1);

        if (GenUpdImplementation(fpSqc))
            exit(-1);
    }
    else if ( strstr(gszCfgFileName, "_seq.cfg") != 0 )
    {
        if (GenSeqHeader(fpHead))
            exit(-1);

        if (GenSeqImplementation(fpSqc))
            exit(-1);
    }
    else if( strstr(gszCfgFileName, "_sel.cfg") != 0 )
    {
        if (GenSelHeader(fpHead))
            exit(-1);

        if (GenSelImplementation(fpSqc))
            exit(-1);
    }
    else
    {
        fprintf(stderr, "file name:[%s] error\n",gszCfgFileName );
        exit(-1);
    }

    fclose(fpHead);
    fclose(fpSqc);

    exit(0);
}

static int ConnectDB(void)
{
//	strncpy(hszDBUser, getenv("DBUSER"), sizeof(hszDBUser));
//	strncpy(hszDBPass, getenv("DBPASS"), sizeof(hszDBPass));
	strncpy(hszDBName, getenv("DBNAME"), sizeof(hszDBName));

    
/*
EXEC SQL CONNECT TO :hszDBName;
*/

{
#line 197 "gensql.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 197 "gensql.sqc"
  sqlaaloc(2,1,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 197 "gensql.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 128;
#line 197 "gensql.sqc"
      sql_setdlist[0].sqldata = (void*)hszDBName;
#line 197 "gensql.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 197 "gensql.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 197 "gensql.sqc"
  sqlacall((unsigned short)29,4,2,0,0L);
#line 197 "gensql.sqc"
  sqlastop(0L);
}

#line 197 "gensql.sqc"

    if (sqlca.sqlcode != 0)
    {
        fprintf(stderr, "Cannot connect to DB2 %s,[%d]\n", \
                hszDBName, (int )sqlca.sqlcode);
        return -1;
    }

    return 0;
}

static void DisconnectDB(void)
{
	
/*
EXEC SQL DISCONNECT ALL;
*/

{
#line 210 "gensql.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 210 "gensql.sqc"
  sqlacall((unsigned short)40,25,0,0,0L);
#line 210 "gensql.sqc"
  sqlastop(0L);
}

#line 210 "gensql.sqc"

}


static void RegulateName(char *name)
{
    char *p1, *p2;
    int flag = 1;
    p1 = p2 = name;

    while (*p1)
    {
        if (*p1 == '_')
        {
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
    char szAppHome[200+1] = {0};

    /* 输入参数合法性检查 */
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: write your <name>.cfg file, then run: gensql <name> \n");
        return -1;
    }
    strcpy(szArgName, argv[1]);

    /* 获取模块名 */
    memset(gszModuleName, 0x00, sizeof(gszModuleName));
    memcpy(gszModuleName, &szArgName[2], 3);

    /* 获取头文件定义名 */
    memset(gszHeaderName, 0x00, sizeof(gszHeaderName));
    strcpy(gszHeaderName, &szArgName[9]);

    for(i = 0; i<strlen(gszHeaderName); i++)
    {
        gszHeadDefine[i] = toupper(gszHeaderName[i]);
    }

    strcpy(gszRegulatedName, gszHeaderName);
    RegulateName(gszRegulatedName);

    /* 获取配置文件名 */
    memset(gszCfgFileName, 0x00, sizeof(gszCfgFileName));
    strcpy(gszCfgFileName, szArgName);
    strcat(gszCfgFileName, ".cfg");

    /* 获取头文件名 */
    memset(gszHeadFileName, 0x00, sizeof(gszHeadFileName));
    sprintf(gszHeadFileName, "%s.h", &szArgName[9]);

    /* 获取sql-c源文件名 */
    memset(gszSrcFileName, 0x00, sizeof(gszSrcFileName));
    sprintf(gszSrcFileName, "%s.sqc", szArgName);


    printf("gszModuleName=[%s], gszHeaderName=[%s]\n", gszModuleName, gszHeaderName);
    printf("gszHeadDefine=[%s], gszRegulatedName=[%s]\n", gszHeadDefine, gszRegulatedName);
    printf("gszCfgFileName =[%s]\n", gszCfgFileName);
    printf("gszHeadFileName=[%s]\n", gszHeadFileName);
    printf("gszSrcFileName =[%s]\n", gszSrcFileName);

    return 0;
}

static int GetDynStatement( void )
{
    char *cp;
    char linebuf[256];
    FILE *fpcfg = NULL;

    /*打开配置文件*/
    if ((fpcfg = fopen(gszCfgFileName, "rt")) == NULL)
    {
        fprintf(stderr, "打开sql文件[%s]出错：%s\n", gszCfgFileName, strerror(errno));
        return errno;
    }

    /* 读取语句 */
    memset(gszInputSql, 0x00, sizeof(gszInputSql));
    while (fgets(linebuf, 256, fpcfg) != NULL)
    {
        /* 除去换行，变为大写 */
        for (cp = linebuf; *cp; cp++)
        {
            if ((*cp == '\r')||(*cp == '\n'))
                *cp = ' ';
            else
                *cp=toupper(*cp);
        }

        strcat(gszInputSql, linebuf);
        strcat(gszInputSql, " ");
    }

    /*关闭配置文件*/
    fclose(fpcfg);

    return 0;
}

static int ParseToken(T_ColDesc *ptCol, char *pszType)
{
    char szLen[10]={0};

    ptCol->iDir = 0;
    if (memcmp(pszType, "INT", 3) == 0)
    {
        ptCol->iType = KR_SQL_TYPE_INT;
        strcpy(ptCol->caColType, "int");
        strcpy(ptCol->caPrefix, "i");
    }
    else if (memcmp(pszType, "LONG", 4) == 0)
    {
        ptCol->iType = KR_SQL_TYPE_LONG;
        strcpy(ptCol->caColType, "long");
        strcpy(ptCol->caPrefix, "l");
    }
    else if (memcmp(pszType, "DOUBLE", 4) == 0)
    {
        ptCol->iType = KR_SQL_TYPE_DOUBLE;
        strcpy(ptCol->caColType, "double");
        strcpy(ptCol->caPrefix, "d");
    }
    else if(memcmp(pszType, "CHAR", 4) == 0)
    {
        ptCol->iType = KR_SQL_TYPE_CHAR;
        strcpy(ptCol->caColType, "char");
        strcpy(ptCol->caPrefix, "ca");

        char *p = strchr(pszType, '(') + 1;
        char *p1 = &szLen[0];
        while(*p != '\0')
        {
            *p1++ = *p++;
            if(*p == ')')
            {
                *p1 = '\0';
                break;
            }
        }
        ptCol->iLength = atoi(szLen);

        if(ptCol->iLength <=0)
        {
            fprintf(stderr, "Length=[%s] Error!\n", szLen);
            return -1;
        }
    }
    else
    {
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
        if (memcmp(pszToken, MON_SPLIT, strlen(MON_SPLIT)) == 0)
        {
            gcSplitTableFlag = 'M';
            giSplitColInDex  = iCol;
            printf("***********Split Table By Month!***********\n");
        }
        else if (memcmp(pszToken, DAY_SPLIT, strlen(DAY_SPLIT)) == 0)
        {
            gcSplitTableFlag = 'D';
            giSplitColInDex  = iCol;
            printf("***********Split Table By Day!***********\n");
        }

        /* copy and regulate field name */
        strcpy(tCol.caFieldName, pszToken);
        RegulateName(tCol.caFieldName);

        /* Get Type */
        pszToken = strtok(NULL, "#");
        if (!pszToken)
        {
            fprintf(stderr, "Need #Type# Define.\n");
            return -1;
        }
        if (ParseToken(&tCol, pszToken) != 0)
        {
            fprintf(stderr, "ParseToken [%s] Error.\n", pszToken);
            return -1;
        }

        /* Get The Next */
        pszToken = strtok(NULL, ":");
        if (!pszToken)
        {
              fprintf(stderr, "变量分隔符不匹配.\n");
              return -1;
        }

        memcpy(&gtColumns[iCol], &tCol, sizeof(T_ColDesc));
        iCol++;
    }

    giInputColumns = iCol;

    return 0;
}


static int SqldaInit(struct sqlda **ppSqlda, int size)
{
    int rc = 0;

    *ppSqlda = (struct sqlda *)malloc(SQLDASIZE(size));
    if (*ppSqlda == NULL)
    {
         return 1;
    }

    memcpy((*ppSqlda)->sqldaid, "SQLDA   ", 8);
    (*ppSqlda)->sqldabc = (long) SQLDASIZE(size);
    (*ppSqlda)->sqln = size;
    (*ppSqlda)->sqld = 0;

    return 0;
}


static int RetrieveOutputParams(void)
{
    int i,j,iNumCols;
    struct sqlda *pSqlda;
    T_ColDesc tColDesc={0};
    
    if (ConnectDB() != 0)
    {
        fprintf(stderr, "ConnectDB Error! \n\n");
        return -1;
    }
    
    
/*
EXEC SQL PREPARE stmt2 FROM :hszSqlStmt;
*/

{
#line 492 "gensql.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 492 "gensql.sqc"
  sqlastls(0,hszSqlStmt,0L);
#line 492 "gensql.sqc"
  sqlacall((unsigned short)27,1,0,0,0L);
#line 492 "gensql.sqc"
  sqlastop(0L);
}

#line 492 "gensql.sqc"

    if (SqldaInit(&pSqlda, 1) != 0)
    {
        return -1;
    }
    
    
/*
EXEC SQL DESCRIBE OUTPUT stmt2 INTO :*pSqlda;
*/

{
#line 498 "gensql.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 498 "gensql.sqc"
  sqlausda(3,&*pSqlda,0L);
#line 498 "gensql.sqc"
  sqlacall((unsigned short)22,1,0,3,0L);
#line 498 "gensql.sqc"
  sqlastop(0L);
}

#line 498 "gensql.sqc"

    if ((sqlca.sqlcode != 0) &&
        (sqlca.sqlcode != SQL_RC_W236) &&
        (sqlca.sqlcode != SQL_RC_W237) &&
        (sqlca.sqlcode != SQL_RC_W238) &&
        (sqlca.sqlcode != SQL_RC_W239))
    {
        fprintf(stderr, "describe table error (%s)(%d)\n", hszSqlStmt, (int)sqlca.sqlcode);
        return -1;
    }
    iNumCols = pSqlda->sqld;
    if (iNumCols <= 0)
    {
        fprintf(stderr, "no result return!\n");
        return 0;
    }

    if ((sqlca.sqlcode == 0) || (sqlca.sqlcode == SQL_RC_W236))
    {
        if (SqldaInit(&pSqlda, iNumCols+1) != 0)
        {
            fprintf(stderr, "SqldaInit error!\n");
            return -1;
        }
    }
    else
    {
        if (SqldaInit(&pSqlda, (iNumCols+1)*2) != 0)
        {
            fprintf(stderr, "SqldaInit error!\n");
            return -1;
        }
    }

    giOutputColumns = iNumCols;
    
    
/*
EXEC SQL DESCRIBE OUTPUT stmt2 INTO :*pSqlda;
*/

{
#line 534 "gensql.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 534 "gensql.sqc"
  sqlausda(3,&*pSqlda,0L);
#line 534 "gensql.sqc"
  sqlacall((unsigned short)22,1,0,3,0L);
#line 534 "gensql.sqc"
  sqlastop(0L);
}

#line 534 "gensql.sqc"
        
    for (i=0; i<iNumCols; i++)
    {        
        memset(&tColDesc,0x00,sizeof(tColDesc));
        
        switch (pSqlda->sqlvar[i].sqltype)
        {
            case SQL_TYP_CHAR:
            case SQL_TYP_NCHAR:
            case SQL_TYP_VARCHAR:
            case SQL_TYP_NVARCHAR:
            case SQL_TYP_DATE:
            case SQL_TYP_NDATE:
            case SQL_TYP_TIME:
            case SQL_TYP_NTIME:
            case SQL_TYP_STAMP:
            case SQL_TYP_NSTAMP:
                tColDesc.iDir = 1;
                tColDesc.iType = KR_SQL_TYPE_CHAR;
                strcpy(tColDesc.caColType, "char");
                tColDesc.iLength = pSqlda->sqlvar[i].sqllen;
                strcpy(tColDesc.caPrefix, "ca");    
                strcat(tColDesc.caPrefix, "Out");
                break;
            case SQL_TYP_DECIMAL:
            case SQL_TYP_NDECIMAL:
                tColDesc.iDir = 1;
                tColDesc.iLength = (int)(((char *)&pSqlda->sqlvar[i].sqllen)[0]);
                tColDesc.iScale = (int)(((char *)&pSqlda->sqlvar[i].sqllen)[1]);
                if ((tColDesc.iScale > 0) || (tColDesc.iLength > 9))
                {
                    tColDesc.iType = KR_SQL_TYPE_DOUBLE;
                    strcpy(tColDesc.caColType, "double");
                    strcpy(tColDesc.caPrefix, "d");
                    strcat(tColDesc.caPrefix, "Out");
                }
                else
                {
                    tColDesc.iType = KR_SQL_TYPE_LONG;
                    strcpy(tColDesc.caColType, "long");
                    strcpy(tColDesc.caPrefix, "l");
                    strcat(tColDesc.caPrefix, "Out");
                }
                break;
            case SQL_TYP_BIGINT:
            case SQL_TYP_NBIGINT:
            case SQL_TYP_INTEGER:
            case SQL_TYP_NINTEGER:
            case SQL_TYP_SMALL:
            case SQL_TYP_NSMALL:   
                tColDesc.iType = KR_SQL_TYPE_LONG;     
                strcpy(tColDesc.caColType, "long");
                strcpy(tColDesc.caPrefix, "l");
                strcat(tColDesc.caPrefix, "Out");
                break;
            default:
                printf("unsupported type(%d)\n", pSqlda->sqlvar[i].sqltype);
                return -1;
        }
        
        strcpy(tColDesc.caFieldName, pSqlda->sqlvar[i].sqlname.data);
        RegulateName(tColDesc.caFieldName);
        memcpy(&gtColumns[giInputColumns+i], &tColDesc, sizeof(T_ColDesc));
    }
    
    free(pSqlda);
    
    DisconnectDB();
    
    return 0;
}



int GenCurHeader(FILE *fp)
{
    int i;
    GenComment(fp);
    fprintf(fp, "#ifndef __%s_H__\n", gszRegulatedName);
    fprintf(fp, "#define __%s_H__\n", gszRegulatedName);

    fprintf(fp, "typedef struct \n");
    fprintf(fp, "{\n");
    GenVarDefinition(fp,0);
    fprintf(fp, "}T_%s;\n\n", gszRegulatedName);
    fprintf(fp, "extern int dbs%s(int iFuncCode, T_%s *pt%s);\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "extern void dbs%sInit(T_%s *pt%s);\n",gszRegulatedName , gszRegulatedName, gszRegulatedName);

    fprintf(fp, "#endif\n\n");
    return 0;
}

int GenUpdHeader(FILE *fp)
{
    int i;
    GenComment(fp);
    fprintf(fp, "#ifndef __%s_H__\n", gszRegulatedName);
    fprintf(fp, "#define __%s_H__\n", gszRegulatedName);

    fprintf(fp, "typedef struct \n");
    fprintf(fp, "{\n");
    GenVarDefinition(fp,0);
    fprintf(fp, "}T_%s;\n\n", gszRegulatedName);
    fprintf(fp, "extern int dbs%s(T_%s *pt%s);\n\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);

    fprintf(fp, "#endif\n\n");
    return 0;
}

int GenSeqHeader(FILE *fp)
{
    int i;
    GenComment(fp);
    fprintf(fp, "#ifndef __%s_H__\n", gszRegulatedName);
    fprintf(fp, "#define __%s_H__\n", gszRegulatedName);

    fprintf(fp, "typedef struct \n");
    fprintf(fp, "{\n");
    GenVarDefinition(fp,0);
    fprintf(fp, "}T_%s;\n\n", gszRegulatedName);
    fprintf(fp, "extern int dbs%s(int iFuncCode, T_%s *pt%s);\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "extern void dbs%sInit(T_%s *pt%s);\n",gszRegulatedName , gszRegulatedName, gszRegulatedName);

    fprintf(fp, "#endif\n\n");
    return 0;
}

int GenSelHeader(FILE *fp)
{
    int i;
    GenComment(fp);
    fprintf(fp, "#ifndef __%s_H__\n", gszRegulatedName);
    fprintf(fp, "#define __%s_H__\n", gszRegulatedName);

    fprintf(fp, "typedef struct \n");
    fprintf(fp, "{\n");
    GenVarDefinition(fp,0);
    fprintf(fp, "}T_%s;\n\n", gszRegulatedName);
    fprintf(fp, "extern int dbs%s(int iFuncCode, T_%s *pt%s);\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "extern void dbs%sInit(T_%s *pt%s);\n",gszRegulatedName , gszRegulatedName, gszRegulatedName);

    fprintf(fp, "#endif\n\n");
    return 0;
}

int GenCurImplementation(FILE *fp)
{
    char szSqlStatement[5000+1];
    strcpy(szSqlStatement, hszSqlStmt);

    if (gcSplitTableFlag != 'N')
    {
        char *p = strstr(szSqlStatement, "01 ");
        if (p != NULL)
        {
            *p++ = '%';
            *p++ = '2';
            *p++ = 's';
        }
        else
        {
            fprintf(stderr, "Split Table Name Need With \"01 \" Terminated!\n");
            return -1;
        }
    }

    GenComment(fp);

    /* 通讯区定义 */
    fprintf(fp, "#include <string.h> \n");
    fprintf(fp, "#include \"dbs/dbs_basopr.h\" \n");
    fprintf(fp, "#include \"dbs/%s.h\"\n\n", gszHeaderName);

    fprintf(fp, "EXEC SQL INCLUDE sqlca;\n");
    fprintf(fp, "EXEC SQL WHENEVER SQLERROR CONTINUE; \n");
    fprintf(fp, "EXEC SQL WHENEVER SQLWARNING CONTINUE; \n");
    fprintf(fp, "EXEC SQL WHENEVER NOT FOUND CONTINUE;\n\n");


    fprintf(fp, "EXEC SQL BEGIN DECLARE SECTION;\n");
    fprintf(fp, "struct dbsTemp%s_t\n", gszRegulatedName);
    fprintf(fp, "{\n");
    GenVarDefinition(fp,1);
    fprintf(fp, "}htTemp%s;\n\n", gszRegulatedName);
    fprintf(fp, "char hca%sSql[2048+1]={0};\n", gszRegulatedName);
    fprintf(fp, "EXEC SQL END DECLARE SECTION;\n\n\n");

    /* 游标初始化函数*/
    fprintf(fp, "void dbs%sInit(T_%s *pt%s)\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "{\n");
    fprintf(fp, "    memset(pt%s, 0x00, sizeof(T_%s));\n", gszRegulatedName, gszRegulatedName);
    fprintf(fp, "}\n\n\n");

    /*游标操作函数*/
    fprintf(fp, "int dbs%s(int iFuncCode, T_%s *pt%s)\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "{\n");
    if (gcSplitTableFlag != 'N')
    {
        fprintf(fp, "    char caSplitVal[2+1] = {0};\n");
    }
    fprintf(fp, "    switch (iFuncCode)\n");
    fprintf(fp, "    {\n");
    /* open cursor 函数 */
    fprintf(fp, "      case KR_DBCUROPEN:\n");
    fprintf(fp, "          memcpy(&htTemp%s, pt%s, sizeof(htTemp%s));\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    if (gcSplitTableFlag == 'M')
    {
        fprintf(fp, "          memcpy(caSplitVal, &htTemp%s.%s%s[4], 2);\n", gszRegulatedName, gtColumns[giSplitColInDex].caPrefix, gtColumns[giSplitColInDex].caFieldName);
        fprintf(fp, "          snprintf(hca%sSql, sizeof(hca%sSql), \"%s\", caSplitVal);\n", gszRegulatedName, gszRegulatedName, szSqlStatement);
    }
    else if (gcSplitTableFlag == 'D')
    {
        fprintf(fp, "          memcpy(caSplitVal, &htTemp%s.%s%s[6], 2);\n", gszRegulatedName, gtColumns[giSplitColInDex].caPrefix, gtColumns[giSplitColInDex].caFieldName);
        fprintf(fp, "          snprintf(hca%sSql, sizeof(hca%sSql), \"%s\", caSplitVal);\n", gszRegulatedName, gszRegulatedName, szSqlStatement);
    }
    else
    {
        fprintf(fp, "          strcpy(hca%sSql, \"%s\");\n", gszRegulatedName, szSqlStatement);
    }
    fprintf(fp, "          EXEC SQL PREPARE Stmt%s FROM :hca%sSql;\n", gszRegulatedName, gszRegulatedName);
    fprintf(fp, "          if (sqlca.sqlcode != KR_DBOK)\n");
    fprintf(fp, "          {\n");
    fprintf(fp, "              return sqlca.sqlcode;\n");
    fprintf(fp, "          }\n");
    fprintf(fp, "          EXEC SQL DECLARE %s_cur CURSOR FOR Stmt%s;\n", gszRegulatedName, gszRegulatedName);
    fprintf(fp, "          if (sqlca.sqlcode != KR_DBOK)\n");
    fprintf(fp, "          {\n");
    fprintf(fp, "              return sqlca.sqlcode;\n");
    fprintf(fp, "          }\n");
    fprintf(fp, "          EXEC SQL OPEN %s_cur ", gszRegulatedName);
    int i;
    if (giInputColumns > 0)
    {
          fprintf(fp, "USING\n");
          for (i=0; i<giInputColumns; i++)
          {
                if (i>0)
                      fprintf(fp, ", \n");
                fprintf(fp, "                    :htTemp%s.%s%s", gszRegulatedName, gtColumns[i].caPrefix, gtColumns[i].caFieldName);
          }
    }
    fprintf(fp, ";\n");
    fprintf(fp, "          if (sqlca.sqlcode != KR_DBOK)\n");
    fprintf(fp, "          {\n");
    fprintf(fp, "            return sqlca.sqlcode;\n");
    fprintf(fp, "          }\n");
    fprintf(fp, "          return sqlca.sqlcode;\n\n");


    /* fetch 函数 */
    fprintf(fp, "      case KR_DBCURFETCH:\n");
    fprintf(fp, "          memcpy(&htTemp%s, pt%s, sizeof(htTemp%s));\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "          EXEC SQL FETCH %s_cur INTO\n", gszRegulatedName);
    for (i=giInputColumns; i<giInputColumns + giOutputColumns; i++)
    {
          if (i>giInputColumns)
                fprintf(fp, ", \n");
          fprintf(fp, "                     :htTemp%s.%s%s", gszRegulatedName, gtColumns[i].caPrefix, gtColumns[i].caFieldName);
          fprintf(fp, " :htTemp%s.ind%s%s", gszRegulatedName, gtColumns[i].caPrefix, gtColumns[i].caFieldName);
    }
    fprintf(fp, ";\n");
    fprintf(fp, "          if ((sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND))\n");
    fprintf(fp, "          {\n");
    fprintf(fp, "              return sqlca.sqlcode;\n");
    fprintf(fp, "          }\n");
    fprintf(fp, "          if (sqlca.sqlcode == KR_DBOK)\n");
    fprintf(fp, "              memcpy( pt%s, &htTemp%s, sizeof(T_%s));\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "          return sqlca.sqlcode;\n\n");

    /* close cursor 函数 */
    fprintf(fp, "      case KR_DBCURCLOSE:\n");
    fprintf(fp, "          EXEC SQL CLOSE %s_cur;\n", gszRegulatedName);
    fprintf(fp, "          if (sqlca.sqlcode != KR_DBOK)\n");
    fprintf(fp, "          {\n");
    fprintf(fp, "              return sqlca.sqlcode;\n");
    fprintf(fp, "          }\n");
    fprintf(fp, "          return sqlca.sqlcode;\n\n");

    /*default*/
    fprintf(fp, "      default:\n");
    fprintf(fp, "          return -1;\n");

    fprintf(fp, "  }\n");
    fprintf(fp, "}\n");

    return 0;
}

int GenUpdImplementation(FILE *fp)
{
    char szSqlStatement[5000+1];
    strcpy(szSqlStatement, hszSqlStmt);
    if (gcSplitTableFlag != 'N')
    {
        char *p = strstr(szSqlStatement, "01 ");
        if (p != NULL)
        {
            *p++ = '%';
            *p++ = '2';
            *p++ = 's';
        }
        else
        {
            fprintf(stderr, "Split Table Name Need With \"01 \" Terminated!\n");
            return -1;
        }
    }

    GenComment(fp);

    /* 通讯区定义 */
    fprintf(fp, "#include <string.h> \n");
    fprintf(fp, "#include \"dbs/dbs_basopr.h\" \n");
    fprintf(fp, "#include \"dbs/%s.h\"\n\n", gszHeaderName);

    fprintf(fp, "EXEC SQL INCLUDE sqlca;\n");
    fprintf(fp, "EXEC SQL WHENEVER SQLERROR CONTINUE; \n");
    fprintf(fp, "EXEC SQL WHENEVER SQLWARNING CONTINUE; \n");
    fprintf(fp, "EXEC SQL WHENEVER NOT FOUND CONTINUE;\n\n");


    fprintf(fp, "EXEC SQL BEGIN DECLARE SECTION;\n");
    fprintf(fp, "struct dbsTemp%s_t\n", gszRegulatedName);
    fprintf(fp, "{\n");
    GenVarDefinition(fp,1);
    fprintf(fp, "}htTemp%s;\n\n", gszRegulatedName);
    fprintf(fp, "char hca%sSql[2048+1]={0};\n", gszRegulatedName);
    fprintf(fp, "EXEC SQL END DECLARE SECTION;\n\n\n");

    /* 更新函数操作 */
    fprintf(fp, "int dbs%s(T_%s *pt%s)\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "{\n");
    if (gcSplitTableFlag != 'N' )
    {
        fprintf(fp, "    char caSplitVal[2+1] = {0};\n");
    }
    fprintf(fp, "    memcpy(&htTemp%s, pt%s, sizeof(htTemp%s));\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    /* Add For Split Table*/
    if (gcSplitTableFlag == 'M')
    {
        fprintf(fp, "    memcpy(caSplitVal, &htTemp%s.%s%s[4], 2);\n", gszRegulatedName, gtColumns[giSplitColInDex].caPrefix, gtColumns[giSplitColInDex].caFieldName);
        fprintf(fp, "    snprintf(hca%sSql, sizeof(hca%sSql), \"%s\", caSplitVal);\n", gszRegulatedName, gszRegulatedName, szSqlStatement);
    }
    else if (gcSplitTableFlag == 'D')
    {
        fprintf(fp, "    memcpy(caSplitVal, &htTemp%s.%s%s[6], 2);\n", gszRegulatedName, gtColumns[giSplitColInDex].caPrefix, gtColumns[giSplitColInDex].caFieldName);
        fprintf(fp, "    snprintf(hca%sSql, sizeof(hca%sSql), \"%s\", caSplitVal);\n", gszRegulatedName, gszRegulatedName, szSqlStatement);
    }
    else
    {
        fprintf(fp, "    strcpy(hca%sSql, \"%s\");\n", gszRegulatedName, szSqlStatement);
    }
    fprintf(fp, "    EXEC SQL PREPARE Stmt%s FROM :hca%sSql;\n", gszRegulatedName, gszRegulatedName);
    fprintf(fp, "    if (sqlca.sqlcode != KR_DBOK)\n");
    fprintf(fp, "    {\n");
    fprintf(fp, "        return sqlca.sqlcode;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "    EXEC SQL EXECUTE Stmt%s ", gszRegulatedName);
    if (giInputColumns > 0)
    {
          fprintf(fp, "USING\n");
          int i;
          for (i=0; i<giInputColumns; i++)
          {
                if (i>0)
                      fprintf(fp, ", \n");
                fprintf(fp, "                    :htTemp%s.%s%s", gszRegulatedName, gtColumns[i].caPrefix, gtColumns[i].caFieldName);
          }
    }
    fprintf(fp, ";\n");
    fprintf(fp, "    if ( (sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND) )\n");
    fprintf(fp, "    {\n");
    fprintf(fp, "        return sqlca.sqlcode;\n");
    fprintf(fp, "    }\n\n");
    fprintf(fp, "    return sqlca.sqlcode;\n\n");
    fprintf(fp, "}\n\n\n");
    return 0;
}

int GenSeqImplementation(FILE *fp)
{
    int i;

    GenComment(fp);

    /* 通讯区定义 */
    fprintf(fp, "#include <string.h> \n");
    fprintf(fp, "#include \"dbs/dbs_basopr.h\" \n");
    fprintf(fp, "#include \"dbs/%s.h\"\n\n", gszHeaderName);

    fprintf(fp, "EXEC SQL INCLUDE sqlca;\n");
    fprintf(fp, "EXEC SQL WHENEVER SQLERROR CONTINUE; \n");
    fprintf(fp, "EXEC SQL WHENEVER SQLWARNING CONTINUE; \n");
    fprintf(fp, "EXEC SQL WHENEVER NOT FOUND CONTINUE;\n\n");


    fprintf(fp, "EXEC SQL BEGIN DECLARE SECTION;\n");
    fprintf(fp, "struct dbsTemp%s_t\n", gszRegulatedName);
    fprintf(fp, "{\n");
    GenVarDefinition(fp,1);
    fprintf(fp, "}htTemp%s;\n\n", gszRegulatedName);
    fprintf(fp, "EXEC SQL END DECLARE SECTION;\n\n\n");

    /* 初始化函数 */
    fprintf(fp, "void dbs%sInit(T_%s *pt%s)\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "{\n");
    fprintf(fp, "    memset(pt%s, 0x00, sizeof(*pt%s));\n", gszRegulatedName, gszRegulatedName);
    fprintf(fp, "}\n\n\n");

    /* 操作函数 */
    fprintf(fp, "int dbs%s(int iFuncCode, T_%s *pt%s)\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "{\n");
    fprintf(fp, "    switch (iFuncCode)\n");
    fprintf(fp, "    {\n");
    /* select函数 */
    fprintf(fp, "      case KR_DBSELECT:\n");
    fprintf(fp, "          EXEC SQL %s INTO \n", hszSqlStmt);
    for (i=giInputColumns; i<giInputColumns + giOutputColumns; i++)
    {
          if (i>giInputColumns)
                fprintf(fp, ", \n");
          fprintf(fp, "                     :htTemp%s.%s%s", gszRegulatedName, gtColumns[i].caPrefix, gtColumns[i].caFieldName);
          fprintf(fp, " :htTemp%s.ind%s%s", gszRegulatedName, gtColumns[i].caPrefix, gtColumns[i].caFieldName);
    }
    fprintf(fp, ";\n");
    fprintf(fp, "          if (sqlca.sqlcode != KR_DBOK)\n");
    fprintf(fp, "          {\n");
    fprintf(fp, "              return sqlca.sqlcode;\n");
    fprintf(fp, "          }\n");
    fprintf(fp, "          memcpy( pt%s, &htTemp%s, sizeof(htTemp%s));\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "          return sqlca.sqlcode;\n\n");
    /*default*/
    fprintf(fp, "      default:\n");
    fprintf(fp, "          return -1;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "}\n");

    return 0;
}

int GenSelImplementation(FILE *fp)
{
    int i;

    char hcaSqlBuff1[2000]={0};
    char hcaSqlBuff2[2000]={0};
    char hcaTempBuff[200]={0};
    char *p, *p1, *p2;
    p=strstr(hszSqlStmt, "FROM");
    for(i=0, p1=hszSqlStmt; p1!=p; i++, p1++)
    {
        hcaSqlBuff1[i]=*p1;
    }

    if (giInputColumns > 0)
    {
        //p2=strtok(p, ":");
        p2=strtok(p, "?");
        strcpy(hcaSqlBuff2, p2);
        for (i=0; i<giInputColumns; i++)
        {
            memset(hcaTempBuff, 0x00, sizeof(hcaTempBuff));
            sprintf(hcaTempBuff," :htTemp%s.%s%s", gszRegulatedName, gtColumns[i].caPrefix, gtColumns[i].caFieldName);
            strcat(hcaSqlBuff2, hcaTempBuff);
            //p2=strtok(NULL, " ");
            //p2=strtok(NULL, ":");
            p2=strtok(NULL, "?");
            if (p2) strcat(hcaSqlBuff2, p2);
        }
    }
    else
    {
        strcpy(hcaSqlBuff2,p);
    }

    GenComment(fp);

    /* 通讯区定义 */
    fprintf(fp, "#include <string.h> \n");
    fprintf(fp, "#include \"dbs/dbs_basopr.h\" \n");
    fprintf(fp, "#include \"dbs/%s.h\"\n\n", gszHeaderName);

    fprintf(fp, "EXEC SQL INCLUDE sqlca;\n");
    fprintf(fp, "EXEC SQL WHENEVER SQLERROR CONTINUE; \n");
    fprintf(fp, "EXEC SQL WHENEVER SQLWARNING CONTINUE; \n");
    fprintf(fp, "EXEC SQL WHENEVER NOT FOUND CONTINUE;\n\n");


    fprintf(fp, "EXEC SQL BEGIN DECLARE SECTION;\n");
    fprintf(fp, "struct dbsTemp%s_t\n", gszRegulatedName);
    fprintf(fp, "{\n");
    GenVarDefinition(fp,1);
    fprintf(fp, "}htTemp%s;\n\n", gszRegulatedName);
    fprintf(fp, "EXEC SQL END DECLARE SECTION;\n\n\n");

    /* 初始化函数 */
    fprintf(fp, "void dbs%sInit(T_%s *pt%s)\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "{\n");
    fprintf(fp, "    memset(pt%s, 0x00, sizeof(*pt%s));\n", gszRegulatedName, gszRegulatedName);
    fprintf(fp, "}\n\n\n");

    /* 操作函数 */
    fprintf(fp, "int dbs%s(int iFuncCode, T_%s *pt%s)\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "{\n");
    fprintf(fp, "    switch (iFuncCode)\n");
    fprintf(fp, "    {\n");
    /* select函数 */
    fprintf(fp, "      case KR_DBSELECT:\n");
    fprintf(fp, "          memcpy(&htTemp%s, pt%s, sizeof(htTemp%s));\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "          EXEC SQL %s INTO \n", hcaSqlBuff1);
    for (i=giInputColumns; i<giInputColumns + giOutputColumns; i++)
    {
          if (i>giInputColumns)
                fprintf(fp, ", \n");
          fprintf(fp, "                     :htTemp%s.%s%s", gszRegulatedName, gtColumns[i].caPrefix, gtColumns[i].caFieldName);
          fprintf(fp, " :htTemp%s.ind%s%s", gszRegulatedName, gtColumns[i].caPrefix, gtColumns[i].caFieldName);
    }
    fprintf(fp, "\n                   %s;\n", hcaSqlBuff2);
    fprintf(fp, "          if ((sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND))\n");
    fprintf(fp, "          {\n");
    fprintf(fp, "              return sqlca.sqlcode;\n");
    fprintf(fp, "          }\n");
    fprintf(fp, "          memcpy( pt%s, &htTemp%s, sizeof(htTemp%s));\n", gszRegulatedName, gszRegulatedName, gszRegulatedName);
    fprintf(fp, "          return sqlca.sqlcode;\n\n");
    /*default*/
    fprintf(fp, "      default:\n");
    fprintf(fp, "          return -1;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "}\n");

    return 0;
}

int GenVarDefinition(FILE *fp,int iArea)
{
    int i, c;
    c = giInputColumns + giOutputColumns;
    /* 先In数据，再Out数据 */
    for (i=0; i<c; i++)
    {
	    char *p=iArea==1?"sqlint64":gtColumns[i].caColType;
        switch(gtColumns[i].iType)
        {
            case KR_SQL_TYPE_CHAR:
                fprintf(fp, "    %-12s%s%s[%d+1];", gtColumns[i].caColType, gtColumns[i].caPrefix, gtColumns[i].caFieldName, gtColumns[i].iLength);
                break;
            case KR_SQL_TYPE_LONG:
                fprintf(fp, "    %-12s%s%s;", p, gtColumns[i].caPrefix, gtColumns[i].caFieldName);
                break;
            default:
                fprintf(fp, "    %-12s%s%s;", gtColumns[i].caColType, gtColumns[i].caPrefix, gtColumns[i].caFieldName);
                break;
        }
        fprintf(fp, "    %-12sind%s%s;", "short", gtColumns[i].caPrefix, gtColumns[i].caFieldName);
        fprintf(fp,"\n");
    }
    return 0;
}

int GenComment(FILE *fp)
{

    return 0;
}
