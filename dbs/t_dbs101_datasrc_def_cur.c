static char sqla_program_id[292] = 
{
 172,0,65,69,65,78,65,73,77,65,121,104,82,82,75,99,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,84,73,71,69,82,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,84,95,68,66,83,49,48,49,0,0,0,0,0,0,0,0,
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


#line 1 "t_dbs101_datasrc_def_cur.sqc"
#include <string.h> 
#include "dbs/dbs_basopr.h" 
#include "dbs/datasrc_def_cur.h"


/*
EXEC SQL INCLUDE sqlca;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 5 "t_dbs101_datasrc_def_cur.sqc"


/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 6 "t_dbs101_datasrc_def_cur.sqc"
 

/*
EXEC SQL WHENEVER SQLWARNING CONTINUE;
*/

#line 7 "t_dbs101_datasrc_def_cur.sqc"
 

/*
EXEC SQL WHENEVER NOT FOUND CONTINUE;
*/

#line 8 "t_dbs101_datasrc_def_cur.sqc"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 10 "t_dbs101_datasrc_def_cur.sqc"

struct dbsTempDatasrcDefCur_t
{
    sqlint64    lOutDatasrcId;    short       indlOutDatasrcId;
    char        caOutDatasrcName[30+1];    short       indcaOutDatasrcName;
    char        caOutDatasrcDesc[100+1];    short       indcaOutDatasrcDesc;
    char        caOutDatasrcMapFunc[50+1];    short       indcaOutDatasrcMapFunc;
    char        caOutDatasrcUsage[1+1];    short       indcaOutDatasrcUsage;
    char        caOutMmapFileName[100+1];    short       indcaOutMmapFileName;
    char        caOutSizeKeepMode[1+1];    short       indcaOutSizeKeepMode;
    sqlint64    lOutSizeKeepValue;    short       indlOutSizeKeepValue;
}htTempDatasrcDefCur;

char hcaDatasrcDefCurSql[2048+1]={0};

/*
EXEC SQL END DECLARE SECTION;
*/

#line 24 "t_dbs101_datasrc_def_cur.sqc"



void dbsDatasrcDefCurInit(T_DatasrcDefCur *ptDatasrcDefCur)
{
    memset(ptDatasrcDefCur, 0x00, sizeof(T_DatasrcDefCur));
}


int dbsDatasrcDefCur(int iFuncCode, T_DatasrcDefCur *ptDatasrcDefCur)
{
    switch (iFuncCode)
    {
      case KR_DBCUROPEN:
          memcpy(&htTempDatasrcDefCur, ptDatasrcDefCur, sizeof(htTempDatasrcDefCur));
          strcpy(hcaDatasrcDefCurSql, "SELECT  A.DATASRC_ID,  A.DATASRC_NAME,  A.DATASRC_DESC,  A.DATASRC_MAP_FUNC,  A.DATASRC_USAGE,  A.MMAP_FILE_NAME,  A.SIZE_KEEP_MODE,  A.SIZE_KEEP_VALUE  FROM  KR_TBL_DATASRC_DEF A  WHERE  A.DATASRC_USAGE = '1'  ");
          
/*
EXEC SQL PREPARE StmtDatasrcDefCur FROM :hcaDatasrcDefCurSql;
*/

{
#line 40 "t_dbs101_datasrc_def_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 40 "t_dbs101_datasrc_def_cur.sqc"
  sqlastls(0,hcaDatasrcDefCurSql,0L);
#line 40 "t_dbs101_datasrc_def_cur.sqc"
  sqlacall((unsigned short)27,1,0,0,0L);
#line 40 "t_dbs101_datasrc_def_cur.sqc"
  sqlastop(0L);
}

#line 40 "t_dbs101_datasrc_def_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL DECLARE DatasrcDefCur_cur CURSOR FOR StmtDatasrcDefCur;
*/

#line 45 "t_dbs101_datasrc_def_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL OPEN DatasrcDefCur_cur ;
*/

{
#line 50 "t_dbs101_datasrc_def_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 50 "t_dbs101_datasrc_def_cur.sqc"
  sqlacall((unsigned short)26,1,0,0,0L);
#line 50 "t_dbs101_datasrc_def_cur.sqc"
  sqlastop(0L);
}

#line 50 "t_dbs101_datasrc_def_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
            return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      case KR_DBCURFETCH:
          memcpy(&htTempDatasrcDefCur, ptDatasrcDefCur, sizeof(htTempDatasrcDefCur));
          
/*
EXEC SQL FETCH DatasrcDefCur_cur INTO
                     :htTempDatasrcDefCur.lOutDatasrcId :htTempDatasrcDefCur.indlOutDatasrcId, 
                     :htTempDatasrcDefCur.caOutDatasrcName :htTempDatasrcDefCur.indcaOutDatasrcName, 
                     :htTempDatasrcDefCur.caOutDatasrcDesc :htTempDatasrcDefCur.indcaOutDatasrcDesc, 
                     :htTempDatasrcDefCur.caOutDatasrcMapFunc :htTempDatasrcDefCur.indcaOutDatasrcMapFunc, 
                     :htTempDatasrcDefCur.caOutDatasrcUsage :htTempDatasrcDefCur.indcaOutDatasrcUsage, 
                     :htTempDatasrcDefCur.caOutMmapFileName :htTempDatasrcDefCur.indcaOutMmapFileName, 
                     :htTempDatasrcDefCur.caOutSizeKeepMode :htTempDatasrcDefCur.indcaOutSizeKeepMode, 
                     :htTempDatasrcDefCur.lOutSizeKeepValue :htTempDatasrcDefCur.indlOutSizeKeepValue;
*/

{
#line 67 "t_dbs101_datasrc_def_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 67 "t_dbs101_datasrc_def_cur.sqc"
  sqlaaloc(3,8,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[8];
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[0].sqltype = 493; sql_setdlist[0].sqllen = 8;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempDatasrcDefCur.lOutDatasrcId;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[0].sqlind = &htTempDatasrcDefCur.indlOutDatasrcId;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[1].sqltype = 461; sql_setdlist[1].sqllen = 31;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[1].sqldata = (void*)htTempDatasrcDefCur.caOutDatasrcName;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[1].sqlind = &htTempDatasrcDefCur.indcaOutDatasrcName;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[2].sqltype = 461; sql_setdlist[2].sqllen = 101;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[2].sqldata = (void*)htTempDatasrcDefCur.caOutDatasrcDesc;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[2].sqlind = &htTempDatasrcDefCur.indcaOutDatasrcDesc;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[3].sqltype = 461; sql_setdlist[3].sqllen = 51;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[3].sqldata = (void*)htTempDatasrcDefCur.caOutDatasrcMapFunc;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[3].sqlind = &htTempDatasrcDefCur.indcaOutDatasrcMapFunc;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[4].sqltype = 461; sql_setdlist[4].sqllen = 2;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[4].sqldata = (void*)htTempDatasrcDefCur.caOutDatasrcUsage;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[4].sqlind = &htTempDatasrcDefCur.indcaOutDatasrcUsage;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[5].sqltype = 461; sql_setdlist[5].sqllen = 101;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[5].sqldata = (void*)htTempDatasrcDefCur.caOutMmapFileName;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[5].sqlind = &htTempDatasrcDefCur.indcaOutMmapFileName;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[6].sqltype = 461; sql_setdlist[6].sqllen = 2;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[6].sqldata = (void*)htTempDatasrcDefCur.caOutSizeKeepMode;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[6].sqlind = &htTempDatasrcDefCur.indcaOutSizeKeepMode;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[7].sqltype = 493; sql_setdlist[7].sqllen = 8;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[7].sqldata = (void*)&htTempDatasrcDefCur.lOutSizeKeepValue;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sql_setdlist[7].sqlind = &htTempDatasrcDefCur.indlOutSizeKeepValue;
#line 67 "t_dbs101_datasrc_def_cur.sqc"
      sqlasetdata(3,0,8,sql_setdlist,0L,0L);
    }
#line 67 "t_dbs101_datasrc_def_cur.sqc"
  sqlacall((unsigned short)25,1,0,3,0L);
#line 67 "t_dbs101_datasrc_def_cur.sqc"
  sqlastop(0L);
}

#line 67 "t_dbs101_datasrc_def_cur.sqc"

          if ((sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND))
          {
              return sqlca.sqlcode;
          }
          if (sqlca.sqlcode == KR_DBOK)
              memcpy( ptDatasrcDefCur, &htTempDatasrcDefCur, sizeof(T_DatasrcDefCur));
          return sqlca.sqlcode;

      case KR_DBCURCLOSE:
          
/*
EXEC SQL CLOSE DatasrcDefCur_cur;
*/

{
#line 77 "t_dbs101_datasrc_def_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 77 "t_dbs101_datasrc_def_cur.sqc"
  sqlacall((unsigned short)20,1,0,0,0L);
#line 77 "t_dbs101_datasrc_def_cur.sqc"
  sqlastop(0L);
}

#line 77 "t_dbs101_datasrc_def_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      default:
          return -1;
  }
}
