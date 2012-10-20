static char sqla_program_id[292] = 
{
 172,0,65,69,65,78,65,73,103,66,65,105,82,82,75,99,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,84,73,71,69,82,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,84,95,68,66,83,51,48,50,0,0,0,0,0,0,0,0,
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


#line 1 "t_dbs302_ddi_cur.sqc"
#include <string.h> 
#include "dbs/dbs_basopr.h" 
#include "dbs/ddi_cur.h"


/*
EXEC SQL INCLUDE sqlca;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 5 "t_dbs302_ddi_cur.sqc"


/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 6 "t_dbs302_ddi_cur.sqc"
 

/*
EXEC SQL WHENEVER SQLWARNING CONTINUE;
*/

#line 7 "t_dbs302_ddi_cur.sqc"
 

/*
EXEC SQL WHENEVER NOT FOUND CONTINUE;
*/

#line 8 "t_dbs302_ddi_cur.sqc"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 10 "t_dbs302_ddi_cur.sqc"

struct dbsTempDdiCur_t
{
    sqlint64    lOutDdiId;    short       indlOutDdiId;
    char        caOutDdiName[30+1];    short       indcaOutDdiName;
    char        caOutDdiDesc[100+1];    short       indcaOutDdiDesc;
    char        caOutDdiType[1+1];    short       indcaOutDdiType;
    char        caOutDdiValueType[1+1];    short       indcaOutDdiValueType;
    char        caOutDdiAggrFunc[50+1];    short       indcaOutDdiAggrFunc;
    char        caOutDdiFreeFunc[50+1];    short       indcaOutDdiFreeFunc;
    sqlint64    lOutStatisticsDatasrc;    short       indlOutStatisticsDatasrc;
    sqlint64    lOutStatisticsIndex;    short       indlOutStatisticsIndex;
    sqlint64    lOutStatisticsField;    short       indlOutStatisticsField;
    char        caOutStatisticsType[1+1];    short       indcaOutStatisticsType;
    sqlint64    lOutStatisticsValue;    short       indlOutStatisticsValue;
    sqlint64    lOutStatisticsCount;    short       indlOutStatisticsCount;
    char        caOutStatisticsMethod[1+1];    short       indcaOutStatisticsMethod;
    char        caOutDdiFilterString[500+1];    short       indcaOutDdiFilterString;
}htTempDdiCur;

char hcaDdiCurSql[2048+1]={0};

/*
EXEC SQL END DECLARE SECTION;
*/

#line 31 "t_dbs302_ddi_cur.sqc"



void dbsDdiCurInit(T_DdiCur *ptDdiCur)
{
    memset(ptDdiCur, 0x00, sizeof(T_DdiCur));
}


int dbsDdiCur(int iFuncCode, T_DdiCur *ptDdiCur)
{
    switch (iFuncCode)
    {
      case KR_DBCUROPEN:
          memcpy(&htTempDdiCur, ptDdiCur, sizeof(htTempDdiCur));
          strcpy(hcaDdiCurSql, "SELECT  A.DDI_ID,  A.DDI_NAME,  A.DDI_DESC,  A.DDI_TYPE,  A.DDI_VALUE_TYPE,  A.DDI_AGGR_FUNC,  A.DDI_FREE_FUNC,  A.STATISTICS_DATASRC,  A.STATISTICS_INDEX,  A.STATISTICS_FIELD,  A.STATISTICS_TYPE,  A.STATISTICS_VALUE,  A.STATISTICS_COUNT,  A.STATISTICS_METHOD,  A.DDI_FILTER_STRING  FROM  KR_TBL_DDI_DEF A  WHERE  A.DDI_STATUS = '0'  ORDER BY   A.DDI_ID  ");
          
/*
EXEC SQL PREPARE StmtDdiCur FROM :hcaDdiCurSql;
*/

{
#line 47 "t_dbs302_ddi_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 47 "t_dbs302_ddi_cur.sqc"
  sqlastls(0,hcaDdiCurSql,0L);
#line 47 "t_dbs302_ddi_cur.sqc"
  sqlacall((unsigned short)27,1,0,0,0L);
#line 47 "t_dbs302_ddi_cur.sqc"
  sqlastop(0L);
}

#line 47 "t_dbs302_ddi_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL DECLARE DdiCur_cur CURSOR FOR StmtDdiCur;
*/

#line 52 "t_dbs302_ddi_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL OPEN DdiCur_cur ;
*/

{
#line 57 "t_dbs302_ddi_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 57 "t_dbs302_ddi_cur.sqc"
  sqlacall((unsigned short)26,1,0,0,0L);
#line 57 "t_dbs302_ddi_cur.sqc"
  sqlastop(0L);
}

#line 57 "t_dbs302_ddi_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
            return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      case KR_DBCURFETCH:
          memcpy(&htTempDdiCur, ptDdiCur, sizeof(htTempDdiCur));
          
/*
EXEC SQL FETCH DdiCur_cur INTO
                     :htTempDdiCur.lOutDdiId :htTempDdiCur.indlOutDdiId, 
                     :htTempDdiCur.caOutDdiName :htTempDdiCur.indcaOutDdiName, 
                     :htTempDdiCur.caOutDdiDesc :htTempDdiCur.indcaOutDdiDesc, 
                     :htTempDdiCur.caOutDdiType :htTempDdiCur.indcaOutDdiType, 
                     :htTempDdiCur.caOutDdiValueType :htTempDdiCur.indcaOutDdiValueType, 
                     :htTempDdiCur.caOutDdiAggrFunc :htTempDdiCur.indcaOutDdiAggrFunc, 
                     :htTempDdiCur.caOutDdiFreeFunc :htTempDdiCur.indcaOutDdiFreeFunc, 
                     :htTempDdiCur.lOutStatisticsDatasrc :htTempDdiCur.indlOutStatisticsDatasrc, 
                     :htTempDdiCur.lOutStatisticsIndex :htTempDdiCur.indlOutStatisticsIndex, 
                     :htTempDdiCur.lOutStatisticsField :htTempDdiCur.indlOutStatisticsField, 
                     :htTempDdiCur.caOutStatisticsType :htTempDdiCur.indcaOutStatisticsType, 
                     :htTempDdiCur.lOutStatisticsValue :htTempDdiCur.indlOutStatisticsValue, 
                     :htTempDdiCur.lOutStatisticsCount :htTempDdiCur.indlOutStatisticsCount, 
                     :htTempDdiCur.caOutStatisticsMethod :htTempDdiCur.indcaOutStatisticsMethod, 
                     :htTempDdiCur.caOutDdiFilterString :htTempDdiCur.indcaOutDdiFilterString;
*/

{
#line 81 "t_dbs302_ddi_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 81 "t_dbs302_ddi_cur.sqc"
  sqlaaloc(3,15,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[15];
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[0].sqltype = 493; sql_setdlist[0].sqllen = 8;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempDdiCur.lOutDdiId;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[0].sqlind = &htTempDdiCur.indlOutDdiId;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[1].sqltype = 461; sql_setdlist[1].sqllen = 31;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[1].sqldata = (void*)htTempDdiCur.caOutDdiName;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[1].sqlind = &htTempDdiCur.indcaOutDdiName;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[2].sqltype = 461; sql_setdlist[2].sqllen = 101;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[2].sqldata = (void*)htTempDdiCur.caOutDdiDesc;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[2].sqlind = &htTempDdiCur.indcaOutDdiDesc;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[3].sqltype = 461; sql_setdlist[3].sqllen = 2;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[3].sqldata = (void*)htTempDdiCur.caOutDdiType;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[3].sqlind = &htTempDdiCur.indcaOutDdiType;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[4].sqltype = 461; sql_setdlist[4].sqllen = 2;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[4].sqldata = (void*)htTempDdiCur.caOutDdiValueType;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[4].sqlind = &htTempDdiCur.indcaOutDdiValueType;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[5].sqltype = 461; sql_setdlist[5].sqllen = 51;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[5].sqldata = (void*)htTempDdiCur.caOutDdiAggrFunc;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[5].sqlind = &htTempDdiCur.indcaOutDdiAggrFunc;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[6].sqltype = 461; sql_setdlist[6].sqllen = 51;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[6].sqldata = (void*)htTempDdiCur.caOutDdiFreeFunc;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[6].sqlind = &htTempDdiCur.indcaOutDdiFreeFunc;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[7].sqltype = 493; sql_setdlist[7].sqllen = 8;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[7].sqldata = (void*)&htTempDdiCur.lOutStatisticsDatasrc;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[7].sqlind = &htTempDdiCur.indlOutStatisticsDatasrc;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[8].sqltype = 493; sql_setdlist[8].sqllen = 8;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[8].sqldata = (void*)&htTempDdiCur.lOutStatisticsIndex;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[8].sqlind = &htTempDdiCur.indlOutStatisticsIndex;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[9].sqltype = 493; sql_setdlist[9].sqllen = 8;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[9].sqldata = (void*)&htTempDdiCur.lOutStatisticsField;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[9].sqlind = &htTempDdiCur.indlOutStatisticsField;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[10].sqltype = 461; sql_setdlist[10].sqllen = 2;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[10].sqldata = (void*)htTempDdiCur.caOutStatisticsType;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[10].sqlind = &htTempDdiCur.indcaOutStatisticsType;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[11].sqltype = 493; sql_setdlist[11].sqllen = 8;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[11].sqldata = (void*)&htTempDdiCur.lOutStatisticsValue;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[11].sqlind = &htTempDdiCur.indlOutStatisticsValue;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[12].sqltype = 493; sql_setdlist[12].sqllen = 8;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[12].sqldata = (void*)&htTempDdiCur.lOutStatisticsCount;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[12].sqlind = &htTempDdiCur.indlOutStatisticsCount;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[13].sqltype = 461; sql_setdlist[13].sqllen = 2;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[13].sqldata = (void*)htTempDdiCur.caOutStatisticsMethod;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[13].sqlind = &htTempDdiCur.indcaOutStatisticsMethod;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[14].sqltype = 461; sql_setdlist[14].sqllen = 501;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[14].sqldata = (void*)htTempDdiCur.caOutDdiFilterString;
#line 81 "t_dbs302_ddi_cur.sqc"
      sql_setdlist[14].sqlind = &htTempDdiCur.indcaOutDdiFilterString;
#line 81 "t_dbs302_ddi_cur.sqc"
      sqlasetdata(3,0,15,sql_setdlist,0L,0L);
    }
#line 81 "t_dbs302_ddi_cur.sqc"
  sqlacall((unsigned short)25,1,0,3,0L);
#line 81 "t_dbs302_ddi_cur.sqc"
  sqlastop(0L);
}

#line 81 "t_dbs302_ddi_cur.sqc"

          if ((sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND))
          {
              return sqlca.sqlcode;
          }
          if (sqlca.sqlcode == KR_DBOK)
              memcpy( ptDdiCur, &htTempDdiCur, sizeof(T_DdiCur));
          return sqlca.sqlcode;

      case KR_DBCURCLOSE:
          
/*
EXEC SQL CLOSE DdiCur_cur;
*/

{
#line 91 "t_dbs302_ddi_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 91 "t_dbs302_ddi_cur.sqc"
  sqlacall((unsigned short)20,1,0,0,0L);
#line 91 "t_dbs302_ddi_cur.sqc"
  sqlastop(0L);
}

#line 91 "t_dbs302_ddi_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      default:
          return -1;
  }
}
