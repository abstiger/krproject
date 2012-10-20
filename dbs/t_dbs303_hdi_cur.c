static char sqla_program_id[292] = 
{
 172,0,65,69,65,78,65,73,82,66,108,107,82,82,75,99,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,84,73,71,69,82,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,84,95,68,66,83,51,48,51,0,0,0,0,0,0,0,0,
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


#line 1 "t_dbs303_hdi_cur.sqc"
#include <string.h> 
#include "dbs/dbs_basopr.h" 
#include "dbs/hdi_cur.h"


/*
EXEC SQL INCLUDE sqlca;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 5 "t_dbs303_hdi_cur.sqc"


/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 6 "t_dbs303_hdi_cur.sqc"
 

/*
EXEC SQL WHENEVER SQLWARNING CONTINUE;
*/

#line 7 "t_dbs303_hdi_cur.sqc"
 

/*
EXEC SQL WHENEVER NOT FOUND CONTINUE;
*/

#line 8 "t_dbs303_hdi_cur.sqc"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 10 "t_dbs303_hdi_cur.sqc"

struct dbsTempHdiCur_t
{
    sqlint64    lOutHdiId;    short       indlOutHdiId;
    char        caOutHdiName[30+1];    short       indcaOutHdiName;
    char        caOutHdiDesc[100+1];    short       indcaOutHdiDesc;
    char        caOutHdiType[1+1];    short       indcaOutHdiType;
    char        caOutHdiValueType[1+1];    short       indcaOutHdiValueType;
    char        caOutHdiAggrFunc[50+1];    short       indcaOutHdiAggrFunc;
    char        caOutHdiFreeFunc[50+1];    short       indcaOutHdiFreeFunc;
    sqlint64    lOutStatisticsDatasrc;    short       indlOutStatisticsDatasrc;
    sqlint64    lOutStatisticsIndex;    short       indlOutStatisticsIndex;
    char        caOutStatisticsType[1+1];    short       indcaOutStatisticsType;
    sqlint64    lOutStatisticsValue;    short       indlOutStatisticsValue;
    char        caOutStatisticsMethod[1+1];    short       indcaOutStatisticsMethod;
}htTempHdiCur;

char hcaHdiCurSql[2048+1]={0};

/*
EXEC SQL END DECLARE SECTION;
*/

#line 28 "t_dbs303_hdi_cur.sqc"



void dbsHdiCurInit(T_HdiCur *ptHdiCur)
{
    memset(ptHdiCur, 0x00, sizeof(T_HdiCur));
}


int dbsHdiCur(int iFuncCode, T_HdiCur *ptHdiCur)
{
    switch (iFuncCode)
    {
      case KR_DBCUROPEN:
          memcpy(&htTempHdiCur, ptHdiCur, sizeof(htTempHdiCur));
          strcpy(hcaHdiCurSql, "SELECT  A.HDI_ID,  A.HDI_NAME,  A.HDI_DESC,  A.HDI_TYPE,  A.HDI_VALUE_TYPE,  A.HDI_AGGR_FUNC,  A.HDI_FREE_FUNC,  A.STATISTICS_DATASRC,  A.STATISTICS_INDEX,  A.STATISTICS_TYPE,  A.STATISTICS_VALUE,  A.STATISTICS_METHOD  FROM  KR_TBL_HDI_DEF A  WHERE  A.HDI_STATUS = '0'  ORDER BY   A.HDI_ID  ");
          
/*
EXEC SQL PREPARE StmtHdiCur FROM :hcaHdiCurSql;
*/

{
#line 44 "t_dbs303_hdi_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 44 "t_dbs303_hdi_cur.sqc"
  sqlastls(0,hcaHdiCurSql,0L);
#line 44 "t_dbs303_hdi_cur.sqc"
  sqlacall((unsigned short)27,1,0,0,0L);
#line 44 "t_dbs303_hdi_cur.sqc"
  sqlastop(0L);
}

#line 44 "t_dbs303_hdi_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL DECLARE HdiCur_cur CURSOR FOR StmtHdiCur;
*/

#line 49 "t_dbs303_hdi_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL OPEN HdiCur_cur ;
*/

{
#line 54 "t_dbs303_hdi_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 54 "t_dbs303_hdi_cur.sqc"
  sqlacall((unsigned short)26,1,0,0,0L);
#line 54 "t_dbs303_hdi_cur.sqc"
  sqlastop(0L);
}

#line 54 "t_dbs303_hdi_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
            return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      case KR_DBCURFETCH:
          memcpy(&htTempHdiCur, ptHdiCur, sizeof(htTempHdiCur));
          
/*
EXEC SQL FETCH HdiCur_cur INTO
                     :htTempHdiCur.lOutHdiId :htTempHdiCur.indlOutHdiId, 
                     :htTempHdiCur.caOutHdiName :htTempHdiCur.indcaOutHdiName, 
                     :htTempHdiCur.caOutHdiDesc :htTempHdiCur.indcaOutHdiDesc, 
                     :htTempHdiCur.caOutHdiType :htTempHdiCur.indcaOutHdiType, 
                     :htTempHdiCur.caOutHdiValueType :htTempHdiCur.indcaOutHdiValueType, 
                     :htTempHdiCur.caOutHdiAggrFunc :htTempHdiCur.indcaOutHdiAggrFunc, 
                     :htTempHdiCur.caOutHdiFreeFunc :htTempHdiCur.indcaOutHdiFreeFunc, 
                     :htTempHdiCur.lOutStatisticsDatasrc :htTempHdiCur.indlOutStatisticsDatasrc, 
                     :htTempHdiCur.lOutStatisticsIndex :htTempHdiCur.indlOutStatisticsIndex, 
                     :htTempHdiCur.caOutStatisticsType :htTempHdiCur.indcaOutStatisticsType, 
                     :htTempHdiCur.lOutStatisticsValue :htTempHdiCur.indlOutStatisticsValue, 
                     :htTempHdiCur.caOutStatisticsMethod :htTempHdiCur.indcaOutStatisticsMethod;
*/

{
#line 75 "t_dbs303_hdi_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 75 "t_dbs303_hdi_cur.sqc"
  sqlaaloc(3,12,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[12];
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[0].sqltype = 493; sql_setdlist[0].sqllen = 8;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempHdiCur.lOutHdiId;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[0].sqlind = &htTempHdiCur.indlOutHdiId;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[1].sqltype = 461; sql_setdlist[1].sqllen = 31;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[1].sqldata = (void*)htTempHdiCur.caOutHdiName;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[1].sqlind = &htTempHdiCur.indcaOutHdiName;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[2].sqltype = 461; sql_setdlist[2].sqllen = 101;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[2].sqldata = (void*)htTempHdiCur.caOutHdiDesc;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[2].sqlind = &htTempHdiCur.indcaOutHdiDesc;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[3].sqltype = 461; sql_setdlist[3].sqllen = 2;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[3].sqldata = (void*)htTempHdiCur.caOutHdiType;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[3].sqlind = &htTempHdiCur.indcaOutHdiType;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[4].sqltype = 461; sql_setdlist[4].sqllen = 2;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[4].sqldata = (void*)htTempHdiCur.caOutHdiValueType;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[4].sqlind = &htTempHdiCur.indcaOutHdiValueType;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[5].sqltype = 461; sql_setdlist[5].sqllen = 51;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[5].sqldata = (void*)htTempHdiCur.caOutHdiAggrFunc;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[5].sqlind = &htTempHdiCur.indcaOutHdiAggrFunc;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[6].sqltype = 461; sql_setdlist[6].sqllen = 51;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[6].sqldata = (void*)htTempHdiCur.caOutHdiFreeFunc;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[6].sqlind = &htTempHdiCur.indcaOutHdiFreeFunc;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[7].sqltype = 493; sql_setdlist[7].sqllen = 8;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[7].sqldata = (void*)&htTempHdiCur.lOutStatisticsDatasrc;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[7].sqlind = &htTempHdiCur.indlOutStatisticsDatasrc;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[8].sqltype = 493; sql_setdlist[8].sqllen = 8;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[8].sqldata = (void*)&htTempHdiCur.lOutStatisticsIndex;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[8].sqlind = &htTempHdiCur.indlOutStatisticsIndex;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[9].sqltype = 461; sql_setdlist[9].sqllen = 2;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[9].sqldata = (void*)htTempHdiCur.caOutStatisticsType;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[9].sqlind = &htTempHdiCur.indcaOutStatisticsType;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[10].sqltype = 493; sql_setdlist[10].sqllen = 8;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[10].sqldata = (void*)&htTempHdiCur.lOutStatisticsValue;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[10].sqlind = &htTempHdiCur.indlOutStatisticsValue;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[11].sqltype = 461; sql_setdlist[11].sqllen = 2;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[11].sqldata = (void*)htTempHdiCur.caOutStatisticsMethod;
#line 75 "t_dbs303_hdi_cur.sqc"
      sql_setdlist[11].sqlind = &htTempHdiCur.indcaOutStatisticsMethod;
#line 75 "t_dbs303_hdi_cur.sqc"
      sqlasetdata(3,0,12,sql_setdlist,0L,0L);
    }
#line 75 "t_dbs303_hdi_cur.sqc"
  sqlacall((unsigned short)25,1,0,3,0L);
#line 75 "t_dbs303_hdi_cur.sqc"
  sqlastop(0L);
}

#line 75 "t_dbs303_hdi_cur.sqc"

          if ((sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND))
          {
              return sqlca.sqlcode;
          }
          if (sqlca.sqlcode == KR_DBOK)
              memcpy( ptHdiCur, &htTempHdiCur, sizeof(T_HdiCur));
          return sqlca.sqlcode;

      case KR_DBCURCLOSE:
          
/*
EXEC SQL CLOSE HdiCur_cur;
*/

{
#line 85 "t_dbs303_hdi_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 85 "t_dbs303_hdi_cur.sqc"
  sqlacall((unsigned short)20,1,0,0,0L);
#line 85 "t_dbs303_hdi_cur.sqc"
  sqlastop(0L);
}

#line 85 "t_dbs303_hdi_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      default:
          return -1;
  }
}
