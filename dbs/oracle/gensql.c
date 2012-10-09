
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[10];
};
static const struct sqlcxp sqlfpn =
{
    9,
    "gensql.pc"
};


static unsigned int sqlctx = 36971;


static struct sqlexd {
   unsigned int   sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
      const short *cud;
   unsigned char  *sqlest;
      const char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
            void  **sqphsv;
   unsigned int   *sqphsl;
            int   *sqphss;
            void  **sqpind;
            int   *sqpins;
   unsigned int   *sqparm;
   unsigned int   **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
              int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
            void  *sqhstv[4];
   unsigned int   sqhstl[4];
            int   sqhsts[4];
            void  *sqindv[4];
            int   sqinds[4];
   unsigned int   sqharm[4];
   unsigned int   *sqharc[4];
   unsigned short  sqadto[4];
   unsigned short  sqtdso[4];
} sqlstm = {12,4};

/* SQLLIB Prototypes */
extern void sqlcxt (void **, unsigned int *,
                    struct sqlexd *, const struct sqlcxp *);
extern void sqlcx2t(void **, unsigned int *,
                    struct sqlexd *, const struct sqlcxp *);
extern void sqlbuft(void **, char *);
extern void sqlgs2t(void **, char *);
extern void sqlorat(void **, unsigned int *, void *);

/* Forms Interface */
static const int IAPSUCC = 0;
static const int IAPFAIL = 1403;
static const int IAPFTL  = 535;
extern void sqliem(char *, int *);

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{12,4130,852,0,0,
5,0,0,1,0,0,283,323,0,0,4,4,0,1,0,1,9,0,0,1,9,0,0,1,10,0,0,1,10,0,0,
36,0,0,2,0,0,273,487,0,0,1,1,0,1,0,1,5,0,0,
55,0,0,2,0,0,276,494,0,0,1,1,0,1,0,3,32,0,0,
74,0,0,3,0,0,286,604,0,0,0,0,0,1,0,
};


#line 1 "gensql.pc"
/*****************************************************************************/
/* PROGRAM NAME: gensql.pc                                                   */
/* DESCRIPTIONS: gensql tool for oracle                                      */
/* AUTHOR      : Tiger                                                       */
/* CREATE DATE : 2010-11-15                                                  */
/*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>


/* EXEC SQL INCLUDE SQLDA;
 */ 
#line 1 "/home/oracle/oracle/product/10.2.0/db_1/precomp/public/SQLDA.H"
/*
 * $Header: sqlda.h 08-may-2002.12:13:42 apopat Exp $ sqlda.h 
 */

/***************************************************************
*      The SQLDA descriptor definition                         *
*--------------------------------------------------------------*
*      VAX/3B Version                                          *
*                                                              *
* Copyright (c) 1987, 2002, Oracle Corporation.  All rights reserved.  *
***************************************************************/


/* NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************
*/

/*  MODIFIED
    apopat     05/08/02  - [2362423] MVS PE to make lines shorter than 79
    apopat     07/31/99 -  [707588] TAB to blanks for OCCS
    lvbcheng   10/27/98 -  change long to int for sqlda
    lvbcheng   08/15/97 -  Move sqlda protos to sqlcpr.h
    lvbcheng   06/25/97 -  Move sqlda protos to this file
    jbasu      01/29/95 -  correct typo
    jbasu      01/27/95 -  correct comment - ub2->sb2
    jbasu      12/12/94 - Bug 217878: note this is an SOSD file
    Morse      12/01/87 - undef L and S for v6 include files
    Richey     07/13/87 - change int defs to long 
    Clare      09/13/84 - Port: Ch types to match SQLLIB structs
    Clare      10/02/86 - Add ifndef SQLDA
*/

#ifndef SQLDA_
#define SQLDA_ 1
 
#ifdef T
# undef T
#endif
#ifdef F
# undef F
#endif

#ifdef S
# undef S
#endif
#ifdef L
# undef L
#endif
 
struct SQLDA {
  /* ub4    */ int        N; /* Descriptor size in number of entries        */
  /* text** */ char     **V; /* Ptr to Arr of addresses of main variables   */
  /* ub4*   */ int       *L; /* Ptr to Arr of lengths of buffers            */
  /* sb2*   */ short     *T; /* Ptr to Arr of types of buffers              */
  /* sb2**  */ short    **I; /* Ptr to Arr of addresses of indicator vars   */
  /* sb4    */ int        F; /* Number of variables found by DESCRIBE       */
  /* text** */ char     **S; /* Ptr to Arr of variable name pointers        */
  /* ub2*   */ short     *M; /* Ptr to Arr of max lengths of var. names     */
  /* ub2*   */ short     *C; /* Ptr to Arr of current lengths of var. names */
  /* text** */ char     **X; /* Ptr to Arr of ind. var. name pointers       */
  /* ub2*   */ short     *Y; /* Ptr to Arr of max lengths of ind. var. names*/
  /* ub2*   */ short     *Z; /* Ptr to Arr of cur lengths of ind. var. names*/
  };
 
typedef struct SQLDA SQLDA;
 
#endif

/* ----------------- */
/* defines for sqlda */
/* ----------------- */

#define SQLSQLDAAlloc(arg1, arg2, arg3, arg4) sqlaldt(arg1, arg2, arg3, arg4) 

#define SQLSQLDAFree(arg1, arg2) sqlclut(arg1, arg2) 



/* EXEC SQL INCLUDE SQLCA;
 */ 
#line 1 "/home/oracle/oracle/product/10.2.0/db_1/precomp/public/SQLCA.H"
/*
 * $Header: sqlca.h 24-apr-2003.12:50:58 mkandarp Exp $ sqlca.h 
 */

/* Copyright (c) 1985, 2003, Oracle Corporation.  All rights reserved.  */
 
/*
NAME
  SQLCA : SQL Communications Area.
FUNCTION
  Contains no code. Oracle fills in the SQLCA with status info
  during the execution of a SQL stmt.
NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************

  If the symbol SQLCA_STORAGE_CLASS is defined, then the SQLCA
  will be defined to have this storage class. For example:
 
    #define SQLCA_STORAGE_CLASS extern
 
  will define the SQLCA as an extern.
 
  If the symbol SQLCA_INIT is defined, then the SQLCA will be
  statically initialized. Although this is not necessary in order
  to use the SQLCA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the SQLCA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then SQLCA_INIT should be left undefined --
  all others can define SQLCA_INIT if they wish.

  If the symbol SQLCA_NONE is defined, then the SQLCA variable will
  not be defined at all.  The symbol SQLCA_NONE should not be defined
  in source modules that have embedded SQL.  However, source modules
  that have no embedded SQL, but need to manipulate a sqlca struct
  passed in as a parameter, can set the SQLCA_NONE symbol to avoid
  creation of an extraneous sqlca variable.
 
MODIFIED
    lvbcheng   07/31/98 -  long to int
    jbasu      12/12/94 -  Bug 217878: note this is an SOSD file
    losborne   08/11/92 -  No sqlca var if SQLCA_NONE macro set 
  Clare      12/06/84 - Ch SQLCA to not be an extern.
  Clare      10/21/85 - Add initialization.
  Bradbury   01/05/86 - Only initialize when SQLCA_INIT set
  Clare      06/12/86 - Add SQLCA_STORAGE_CLASS option.
*/
 
#ifndef SQLCA
#define SQLCA 1
 
struct   sqlca
         {
         /* ub1 */ char    sqlcaid[8];
         /* b4  */ int     sqlabc;
         /* b4  */ int     sqlcode;
         struct
           {
           /* ub2 */ unsigned short sqlerrml;
           /* ub1 */ char           sqlerrmc[70];
           } sqlerrm;
         /* ub1 */ char    sqlerrp[8];
         /* b4  */ int     sqlerrd[6];
         /* ub1 */ char    sqlwarn[8];
         /* ub1 */ char    sqlext[8];
         };

#ifndef SQLCA_NONE 
#ifdef   SQLCA_STORAGE_CLASS
SQLCA_STORAGE_CLASS struct sqlca sqlca
#else
         struct sqlca sqlca
#endif
 
#ifdef  SQLCA_INIT
         = {
         {'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '},
         sizeof(struct sqlca),
         0,
         { 0, {0}},
         {'N', 'O', 'T', ' ', 'S', 'E', 'T', ' '},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0}
         }
#endif
         ;
#endif
 
#endif
 
/* end SQLCA */

#line 17 "gensql.pc"

/* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 
#line 18 "gensql.pc"

/* EXEC SQL WHENEVER SQLWARNING CONTINUE; */ 
#line 19 "gensql.pc"

/* EXEC SQL WHENEVER NOT FOUND CONTINUE; */ 
#line 20 "gensql.pc"


/* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 22 "gensql.pc"

    char    hszSqlStmt[2048];
    /* EXEC SQL VAR hszSqlStmt IS STRING(2048); */ 
#line 24 "gensql.pc"

/* EXEC SQL END DECLARE SECTION; */ 
#line 25 "gensql.pc"


/* Maximum number of select-list items or bind variables. */
#define MAX_ITEMS         200

/* Maximum lengths of the _names_ of the
   select-list items or indicator variables. */
#define MAX_VNAME_LEN     50
#define MAX_INAME_LEN     50

#ifndef NULL
#define NULL  0
#endif

#define SQL_TYP_INT     1
#define SQL_TYP_LONG    2
#define SQL_TYP_DOUBLE  3
#define SQL_TYP_CHAR    4

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


extern struct SQLDA *sqlald( int, size_t, size_t );
static void RegulateName(char *name);
static int ParseParameter(int argc, char *argv[]);
static int GetDynStatement(void);
static int ParseToken(T_ColDesc *ptCol, char *pszToken);
static int RetrieveInputParams(void);
static int ConnectDB(void);
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

/* 把标识符转换成大小写相间的格式 */
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

/*解析命令行参数*/
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
    sprintf(gszSrcFileName, "%s.pc", szArgName);


    printf("gszModuleName=[%s], gszHeaderName=[%s]\n", gszModuleName, gszHeaderName);
    printf("gszHeadDefine=[%s], gszRegulatedName=[%s]\n", gszHeadDefine, gszRegulatedName);
    printf("gszCfgFileName =[%s]\n", gszCfgFileName);
    printf("gszHeadFileName=[%s]\n", gszHeadFileName);
    printf("gszSrcFileName =[%s]\n", gszSrcFileName);

    return 0;
}

/* 从配置文件中获取SQL语句 */
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


/* 连接数据库 */
static int ConnectDB(void)
{
    /* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 311 "gensql.pc"

        /* VARCHAR  username[128]; */ 
struct { unsigned short len; unsigned char arr[128]; } username;
#line 312 "gensql.pc"

        /* VARCHAR  password[32]; */ 
struct { unsigned short len; unsigned char arr[32]; } password;
#line 313 "gensql.pc"

    /* EXEC SQL END DECLARE SECTION; */ 
#line 314 "gensql.pc"


    strcpy(username.arr, getenv("DBUSER"));
    username.len = (unsigned short)strlen((char *) username.arr);

    strcpy(password.arr, getenv("DBPASS"));
    password.len = (unsigned short)strlen((char *) password.arr);


    /* EXEC SQL CONNECT :username IDENTIFIED BY :password; */ 
#line 323 "gensql.pc"

{
#line 323 "gensql.pc"
    struct sqlexd sqlstm;
#line 323 "gensql.pc"
    sqlstm.sqlvsn = 12;
#line 323 "gensql.pc"
    sqlstm.arrsiz = 4;
#line 323 "gensql.pc"
    sqlstm.sqladtp = &sqladt;
#line 323 "gensql.pc"
    sqlstm.sqltdsp = &sqltds;
#line 323 "gensql.pc"
    sqlstm.iters = (unsigned int  )10;
#line 323 "gensql.pc"
    sqlstm.offset = (unsigned int  )5;
#line 323 "gensql.pc"
    sqlstm.cud = sqlcud0;
#line 323 "gensql.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 323 "gensql.pc"
    sqlstm.sqlety = (unsigned short)256;
#line 323 "gensql.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 323 "gensql.pc"
    sqlstm.sqhstv[0] = (         void  *)&username;
#line 323 "gensql.pc"
    sqlstm.sqhstl[0] = (unsigned int  )130;
#line 323 "gensql.pc"
    sqlstm.sqhsts[0] = (         int  )130;
#line 323 "gensql.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 323 "gensql.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 323 "gensql.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 323 "gensql.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 323 "gensql.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 323 "gensql.pc"
    sqlstm.sqhstv[1] = (         void  *)&password;
#line 323 "gensql.pc"
    sqlstm.sqhstl[1] = (unsigned int  )34;
#line 323 "gensql.pc"
    sqlstm.sqhsts[1] = (         int  )34;
#line 323 "gensql.pc"
    sqlstm.sqindv[1] = (         void  *)0;
#line 323 "gensql.pc"
    sqlstm.sqinds[1] = (         int  )0;
#line 323 "gensql.pc"
    sqlstm.sqharm[1] = (unsigned int  )0;
#line 323 "gensql.pc"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 323 "gensql.pc"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 323 "gensql.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 323 "gensql.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 323 "gensql.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 323 "gensql.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 323 "gensql.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 323 "gensql.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 323 "gensql.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 323 "gensql.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 323 "gensql.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 323 "gensql.pc"
    sqlstm.sqlcmax = (unsigned int )100;
#line 323 "gensql.pc"
    sqlstm.sqlcmin = (unsigned int )2;
#line 323 "gensql.pc"
    sqlstm.sqlcincr = (unsigned int )1;
#line 323 "gensql.pc"
    sqlstm.sqlctimeout = (unsigned int )0;
#line 323 "gensql.pc"
    sqlstm.sqlcnowait = (unsigned int )0;
#line 323 "gensql.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 323 "gensql.pc"
}

#line 323 "gensql.pc"

    if (sqlca.sqlcode != 0)
    {
        fprintf(stderr, "Cannot connect to ORACLE as user %s\n", username.arr);
        return -1;
    }

    return 0;
}

static int ParseToken(T_ColDesc *ptCol, char *pszType)
{
    char szLen[10]={0};

    ptCol->iDir = 0;
    if (memcmp(pszType, "INT", 3) == 0)
    {
        ptCol->iType = SQL_TYP_INT;
        strcpy(ptCol->caColType, "int");
        strcpy(ptCol->caPrefix, "i");
    }
    else if (memcmp(pszType, "LONG", 4) == 0)
    {
        ptCol->iType = SQL_TYP_LONG;
        strcpy(ptCol->caColType, "long");
        strcpy(ptCol->caPrefix, "l");
    }
    else if (memcmp(pszType, "DOUBLE", 4) == 0)
    {
        ptCol->iType = SQL_TYP_DOUBLE;
        strcpy(ptCol->caColType, "double");
        strcpy(ptCol->caPrefix, "d");
    }
    else if(memcmp(pszType, "CHAR", 4) == 0)
    {
        ptCol->iType = SQL_TYP_CHAR;
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


/* 获取sql中输入参数信息 */
static int RetrieveInputParams(void)
{
    int iCol = 0;
    char *pszToken = NULL;
    T_ColDesc tCol={0};
    gcSplitTableFlag = 'N';
    pszToken = strtok(gszInputSql, ":");
    while (pszToken)
    {
        strcat(hszSqlStmt, pszToken);
        pszToken = strtok(NULL, "#");
        if (!pszToken)
            break;

        /* Get Column Name */
        strcat(hszSqlStmt, ":");
        strcat(hszSqlStmt, pszToken);

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


static int RetrieveOutputParams()
{
    int i = 0 , null_ok, precision, scale;
    SQLDA *select_dp;
    T_ColDesc tCol={0};

    /* 连接数据库 */
    if (ConnectDB() != 0)
    {
        fprintf(stderr, "ConnectDB Error! \n\n");
        return -1;
    }

    if ((select_dp = sqlald (MAX_ITEMS, MAX_VNAME_LEN, MAX_INAME_LEN)) == (SQLDA *) 0)
    {
        fprintf(stderr, "Cannot allocate memory for select descriptor.");
        return -1;
    }
    select_dp->N = MAX_ITEMS;

    /* Allocate the pointers to the indicator variables, and the
       actual data. */
    for (i = 0; i < select_dp->N; i++) {
        select_dp->I[i] = (short *) malloc(sizeof (short));
        select_dp->V[i] = (char *) malloc(1);
    }

    /* EXEC SQL PREPARE S FROM :hszSqlStmt; */ 
#line 487 "gensql.pc"

{
#line 487 "gensql.pc"
    struct sqlexd sqlstm;
#line 487 "gensql.pc"
    sqlstm.sqlvsn = 12;
#line 487 "gensql.pc"
    sqlstm.arrsiz = 4;
#line 487 "gensql.pc"
    sqlstm.sqladtp = &sqladt;
#line 487 "gensql.pc"
    sqlstm.sqltdsp = &sqltds;
#line 487 "gensql.pc"
    sqlstm.stmt = "";
#line 487 "gensql.pc"
    sqlstm.iters = (unsigned int  )1;
#line 487 "gensql.pc"
    sqlstm.offset = (unsigned int  )36;
#line 487 "gensql.pc"
    sqlstm.cud = sqlcud0;
#line 487 "gensql.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 487 "gensql.pc"
    sqlstm.sqlety = (unsigned short)256;
#line 487 "gensql.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 487 "gensql.pc"
    sqlstm.sqhstv[0] = (         void  *)hszSqlStmt;
#line 487 "gensql.pc"
    sqlstm.sqhstl[0] = (unsigned int  )2048;
#line 487 "gensql.pc"
    sqlstm.sqhsts[0] = (         int  )0;
#line 487 "gensql.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 487 "gensql.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 487 "gensql.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 487 "gensql.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 487 "gensql.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 487 "gensql.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 487 "gensql.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 487 "gensql.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 487 "gensql.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 487 "gensql.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 487 "gensql.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 487 "gensql.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 487 "gensql.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 487 "gensql.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 487 "gensql.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 487 "gensql.pc"
}

#line 487 "gensql.pc"

    if ((sqlca.sqlcode != 0))
    {
        fprintf(stderr, "prepare sql (%s) error (%s)\n", hszSqlStmt, sqlca.sqlerrm.sqlerrmc);
        return -1;
    }

    /* EXEC SQL DESCRIBE SELECT LIST FOR S INTO select_dp; */ 
#line 494 "gensql.pc"

{
#line 494 "gensql.pc"
    struct sqlexd sqlstm;
#line 494 "gensql.pc"
    sqlstm.sqlvsn = 12;
#line 494 "gensql.pc"
    sqlstm.arrsiz = 4;
#line 494 "gensql.pc"
    sqlstm.sqladtp = &sqladt;
#line 494 "gensql.pc"
    sqlstm.sqltdsp = &sqltds;
#line 494 "gensql.pc"
    sqlstm.iters = (unsigned int  )1;
#line 494 "gensql.pc"
    sqlstm.offset = (unsigned int  )55;
#line 494 "gensql.pc"
    sqlstm.cud = sqlcud0;
#line 494 "gensql.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 494 "gensql.pc"
    sqlstm.sqlety = (unsigned short)256;
#line 494 "gensql.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 494 "gensql.pc"
    sqlstm.sqhstv[0] = (         void  *)select_dp;
#line 494 "gensql.pc"
    sqlstm.sqhstl[0] = (unsigned int  )0;
#line 494 "gensql.pc"
    sqlstm.sqhsts[0] = (         int  )0;
#line 494 "gensql.pc"
    sqlstm.sqindv[0] = (         void  *)0;
#line 494 "gensql.pc"
    sqlstm.sqinds[0] = (         int  )0;
#line 494 "gensql.pc"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 494 "gensql.pc"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 494 "gensql.pc"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 494 "gensql.pc"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 494 "gensql.pc"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 494 "gensql.pc"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 494 "gensql.pc"
    sqlstm.sqpind = sqlstm.sqindv;
#line 494 "gensql.pc"
    sqlstm.sqpins = sqlstm.sqinds;
#line 494 "gensql.pc"
    sqlstm.sqparm = sqlstm.sqharm;
#line 494 "gensql.pc"
    sqlstm.sqparc = sqlstm.sqharc;
#line 494 "gensql.pc"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 494 "gensql.pc"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 494 "gensql.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 494 "gensql.pc"
}

#line 494 "gensql.pc"

    if ((sqlca.sqlcode != 0))
    {
        fprintf(stderr, "Describe SELECT LIST  error (%s)\n", sqlca.sqlerrm.sqlerrmc);
        return -1;
    }

    /* If F is negative, there were more select-list
       items than originally allocated by sqlald(). */
    if (select_dp->F < 0)
    {
        fprintf(stderr, "\nToo many select-list items (%d), maximum is %d\n",
                -(select_dp->F), MAX_ITEMS);
        return -1;
    }

    /* Set the maximum number of array elements in the
       descriptor to the number found. */
    select_dp->N = select_dp->F;

    giOutputColumns = select_dp->F;
    for (i = 0; i < select_dp->F; i++)
    {
        memset(&tCol, 0x00, sizeof(T_ColDesc));
        /* Turn off high-order bit of datatype (in this example,
           it does not matter if the column is NOT NULL). */
        sqlnul ((unsigned short *)&(select_dp->T[i]), (unsigned short *)&(select_dp->T[i]), &null_ok);

        switch (select_dp->T[i])
        {
            case  1 :
            case  96 :
            case  187 :
                     /* CHAR datatype: no change in length
                              needed, except possibly for TO_CHAR
                              conversions (not handled here). */
                tCol.iDir = 1;
                tCol.iType = SQL_TYP_CHAR;
                strcpy(tCol.caColType, "char");
                strncpy(tCol.caFieldName, select_dp->S[i], select_dp->C[i]);
                strcpy(tCol.caPrefix, "ca");
                strcat(tCol.caPrefix, "Out");
                tCol.iLength = select_dp->L[i];
                break;
            case  2 : /* NUMBER datatype: use sqlprc() to
                         extract precision and scale. */
                sqlprc ((unsigned long *)&(select_dp->L[i]), &precision, &scale);
                      /* Allow for maximum size of NUMBER. */
                if (precision == 0) precision = 40;
                      /* Also allow for decimal point and
                         possible sign. */
                /* convert NUMBER datatype to FLOAT if scale > 0,
                   INT otherwise. */
                tCol.iDir = 1;
                strncpy(tCol.caFieldName, select_dp->S[i], select_dp->C[i]);
                tCol.iLength = precision;
                tCol.iScale = scale;
                if (scale > 0 || precision == 40)
                {
                    tCol.iType = SQL_TYP_DOUBLE;
                    strcpy(tCol.caColType, "double");
                    strcpy(tCol.caPrefix, "d");
                    strcat(tCol.caPrefix, "Out");
                }
                else
                {
                    tCol.iType = SQL_TYP_LONG;
                    strcpy(tCol.caColType, "long");
                    strcpy(tCol.caPrefix, "l");
                    strcat(tCol.caPrefix, "Out");
                }
                break;

            case  8 : /* LONG datatype */
                tCol.iDir = 1;
                strncpy(tCol.caFieldName, select_dp->S[i], select_dp->C[i]);
                tCol.iLength = select_dp->L[i];
                tCol.iType = SQL_TYP_LONG;
                strcpy(tCol.caColType, "long");
                strcpy(tCol.caPrefix, "l");
                strcat(tCol.caPrefix, "Out");
                break;
            default:
                printf("unkown type =[%d], name=[%s]\n", select_dp->T[i], select_dp->S[i]);
                tCol.iDir = 1;
                strncpy(tCol.caFieldName, select_dp->S[i], select_dp->C[i]);
                tCol.iLength = select_dp->L[i];
                tCol.iType = SQL_TYP_INT;
                strcpy(tCol.caColType, "int");
                strcpy(tCol.caPrefix, "i");
                strcat(tCol.caPrefix, "Out");
                break;
        }

        RegulateName(tCol.caFieldName);
        memcpy(&gtColumns[giInputColumns+i], &tCol, sizeof(T_ColDesc));
    }

    /* When done, free the memory allocated for
       pointers in the bind and select descriptors. */
    for (i = 0; i < MAX_ITEMS; i++)
    {
        if (select_dp->V[i] != (char *) 0)
            free(select_dp->V[i]);
        free(select_dp->I[i]); /* MAX_ITEMS were allocated. */
    }

    /* Free space used by the descriptors themselves. */
    sqlclu(select_dp);

    /* EXEC SQL COMMIT WORK RELEASE; */ 
#line 604 "gensql.pc"

{
#line 604 "gensql.pc"
    struct sqlexd sqlstm;
#line 604 "gensql.pc"
    sqlstm.sqlvsn = 12;
#line 604 "gensql.pc"
    sqlstm.arrsiz = 4;
#line 604 "gensql.pc"
    sqlstm.sqladtp = &sqladt;
#line 604 "gensql.pc"
    sqlstm.sqltdsp = &sqltds;
#line 604 "gensql.pc"
    sqlstm.iters = (unsigned int  )1;
#line 604 "gensql.pc"
    sqlstm.offset = (unsigned int  )74;
#line 604 "gensql.pc"
    sqlstm.cud = sqlcud0;
#line 604 "gensql.pc"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 604 "gensql.pc"
    sqlstm.sqlety = (unsigned short)256;
#line 604 "gensql.pc"
    sqlstm.occurs = (unsigned int  )0;
#line 604 "gensql.pc"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 604 "gensql.pc"
}

#line 604 "gensql.pc"


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
        p2=strtok(p, ":");
        strcpy(hcaSqlBuff2, p2);
        for (i=0; i<giInputColumns; i++)
        {
            memset(hcaTempBuff, 0x00, sizeof(hcaTempBuff));
            sprintf(hcaTempBuff," :htTemp%s.%s%s", gszRegulatedName, gtColumns[i].caPrefix, gtColumns[i].caFieldName);
            strcat(hcaSqlBuff2, hcaTempBuff);
            p2=strtok(NULL, " ");
            p2=strtok(NULL, ":");
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
        switch(gtColumns[i].iType)
        {
            case SQL_TYP_CHAR:
                fprintf(fp, "    %-12s%s%s[%d+1];", gtColumns[i].caColType, gtColumns[i].caPrefix, gtColumns[i].caFieldName, gtColumns[i].iLength);
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
