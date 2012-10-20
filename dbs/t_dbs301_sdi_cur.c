static char sqla_program_id[292] = 
{
 172,0,65,69,65,78,65,73,66,66,56,104,82,82,75,99,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,84,73,71,69,82,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,84,95,68,66,83,51,48,49,0,0,0,0,0,0,0,0,
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


#line 1 "t_dbs301_sdi_cur.sqc"
#include <string.h> 
#include "dbs/dbs_basopr.h" 
#include "dbs/sdi_cur.h"


/*
EXEC SQL INCLUDE sqlca;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 5 "t_dbs301_sdi_cur.sqc"


/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 6 "t_dbs301_sdi_cur.sqc"
 

/*
EXEC SQL WHENEVER SQLWARNING CONTINUE;
*/

#line 7 "t_dbs301_sdi_cur.sqc"
 

/*
EXEC SQL WHENEVER NOT FOUND CONTINUE;
*/

#line 8 "t_dbs301_sdi_cur.sqc"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 10 "t_dbs301_sdi_cur.sqc"

struct dbsTempSdiCur_t
{
    sqlint64    lOutSdiId;    short       indlOutSdiId;
    char        caOutSdiName[30+1];    short       indcaOutSdiName;
    char        caOutSdiDesc[100+1];    short       indcaOutSdiDesc;
    char        caOutSdiType[1+1];    short       indcaOutSdiType;
    char        caOutSdiValueType[1+1];    short       indcaOutSdiValueType;
    char        caOutSdiAggrFunc[50+1];    short       indcaOutSdiAggrFunc;
    char        caOutSdiFreeFunc[50+1];    short       indcaOutSdiFreeFunc;
    sqlint64    lOutStatisticsDatasrc;    short       indlOutStatisticsDatasrc;
    sqlint64    lOutStatisticsIndex;    short       indlOutStatisticsIndex;
    sqlint64    lOutStatisticsField;    short       indlOutStatisticsField;
    sqlint64    lOutStatisticsLocation;    short       indlOutStatisticsLocation;
    char        caOutLocationProperty[1+1];    short       indcaOutLocationProperty;
    char        caOutSdiFilterString[500+1];    short       indcaOutSdiFilterString;
}htTempSdiCur;

char hcaSdiCurSql[2048+1]={0};

/*
EXEC SQL END DECLARE SECTION;
*/

#line 29 "t_dbs301_sdi_cur.sqc"



void dbsSdiCurInit(T_SdiCur *ptSdiCur)
{
    memset(ptSdiCur, 0x00, sizeof(T_SdiCur));
}


int dbsSdiCur(int iFuncCode, T_SdiCur *ptSdiCur)
{
    switch (iFuncCode)
    {
      case KR_DBCUROPEN:
          memcpy(&htTempSdiCur, ptSdiCur, sizeof(htTempSdiCur));
          strcpy(hcaSdiCurSql, "SELECT  A.SDI_ID,  A.SDI_NAME,  A.SDI_DESC,  A.SDI_TYPE,  A.SDI_VALUE_TYPE,  A.SDI_AGGR_FUNC,  A.SDI_FREE_FUNC,  A.STATISTICS_DATASRC,  A.STATISTICS_INDEX,  A.STATISTICS_FIELD,  A.STATISTICS_LOCATION,  A.LOCATION_PROPERTY,  A.SDI_FILTER_STRING  FROM   KR_TBL_SDI_DEF A  WHERE  A.SDI_STATUS='0'  ORDER BY  A.SDI_ID  ");
          
/*
EXEC SQL PREPARE StmtSdiCur FROM :hcaSdiCurSql;
*/

{
#line 45 "t_dbs301_sdi_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 45 "t_dbs301_sdi_cur.sqc"
  sqlastls(0,hcaSdiCurSql,0L);
#line 45 "t_dbs301_sdi_cur.sqc"
  sqlacall((unsigned short)27,1,0,0,0L);
#line 45 "t_dbs301_sdi_cur.sqc"
  sqlastop(0L);
}

#line 45 "t_dbs301_sdi_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL DECLARE SdiCur_cur CURSOR FOR StmtSdiCur;
*/

#line 50 "t_dbs301_sdi_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL OPEN SdiCur_cur ;
*/

{
#line 55 "t_dbs301_sdi_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 55 "t_dbs301_sdi_cur.sqc"
  sqlacall((unsigned short)26,1,0,0,0L);
#line 55 "t_dbs301_sdi_cur.sqc"
  sqlastop(0L);
}

#line 55 "t_dbs301_sdi_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
            return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      case KR_DBCURFETCH:
          memcpy(&htTempSdiCur, ptSdiCur, sizeof(htTempSdiCur));
          
/*
EXEC SQL FETCH SdiCur_cur INTO
                     :htTempSdiCur.lOutSdiId :htTempSdiCur.indlOutSdiId, 
                     :htTempSdiCur.caOutSdiName :htTempSdiCur.indcaOutSdiName, 
                     :htTempSdiCur.caOutSdiDesc :htTempSdiCur.indcaOutSdiDesc, 
                     :htTempSdiCur.caOutSdiType :htTempSdiCur.indcaOutSdiType, 
                     :htTempSdiCur.caOutSdiValueType :htTempSdiCur.indcaOutSdiValueType, 
                     :htTempSdiCur.caOutSdiAggrFunc :htTempSdiCur.indcaOutSdiAggrFunc, 
                     :htTempSdiCur.caOutSdiFreeFunc :htTempSdiCur.indcaOutSdiFreeFunc, 
                     :htTempSdiCur.lOutStatisticsDatasrc :htTempSdiCur.indlOutStatisticsDatasrc, 
                     :htTempSdiCur.lOutStatisticsIndex :htTempSdiCur.indlOutStatisticsIndex, 
                     :htTempSdiCur.lOutStatisticsField :htTempSdiCur.indlOutStatisticsField, 
                     :htTempSdiCur.lOutStatisticsLocation :htTempSdiCur.indlOutStatisticsLocation, 
                     :htTempSdiCur.caOutLocationProperty :htTempSdiCur.indcaOutLocationProperty, 
                     :htTempSdiCur.caOutSdiFilterString :htTempSdiCur.indcaOutSdiFilterString;
*/

{
#line 77 "t_dbs301_sdi_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 77 "t_dbs301_sdi_cur.sqc"
  sqlaaloc(3,13,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[13];
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[0].sqltype = 493; sql_setdlist[0].sqllen = 8;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempSdiCur.lOutSdiId;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[0].sqlind = &htTempSdiCur.indlOutSdiId;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[1].sqltype = 461; sql_setdlist[1].sqllen = 31;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[1].sqldata = (void*)htTempSdiCur.caOutSdiName;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[1].sqlind = &htTempSdiCur.indcaOutSdiName;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[2].sqltype = 461; sql_setdlist[2].sqllen = 101;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[2].sqldata = (void*)htTempSdiCur.caOutSdiDesc;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[2].sqlind = &htTempSdiCur.indcaOutSdiDesc;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[3].sqltype = 461; sql_setdlist[3].sqllen = 2;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[3].sqldata = (void*)htTempSdiCur.caOutSdiType;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[3].sqlind = &htTempSdiCur.indcaOutSdiType;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[4].sqltype = 461; sql_setdlist[4].sqllen = 2;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[4].sqldata = (void*)htTempSdiCur.caOutSdiValueType;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[4].sqlind = &htTempSdiCur.indcaOutSdiValueType;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[5].sqltype = 461; sql_setdlist[5].sqllen = 51;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[5].sqldata = (void*)htTempSdiCur.caOutSdiAggrFunc;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[5].sqlind = &htTempSdiCur.indcaOutSdiAggrFunc;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[6].sqltype = 461; sql_setdlist[6].sqllen = 51;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[6].sqldata = (void*)htTempSdiCur.caOutSdiFreeFunc;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[6].sqlind = &htTempSdiCur.indcaOutSdiFreeFunc;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[7].sqltype = 493; sql_setdlist[7].sqllen = 8;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[7].sqldata = (void*)&htTempSdiCur.lOutStatisticsDatasrc;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[7].sqlind = &htTempSdiCur.indlOutStatisticsDatasrc;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[8].sqltype = 493; sql_setdlist[8].sqllen = 8;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[8].sqldata = (void*)&htTempSdiCur.lOutStatisticsIndex;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[8].sqlind = &htTempSdiCur.indlOutStatisticsIndex;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[9].sqltype = 493; sql_setdlist[9].sqllen = 8;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[9].sqldata = (void*)&htTempSdiCur.lOutStatisticsField;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[9].sqlind = &htTempSdiCur.indlOutStatisticsField;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[10].sqltype = 493; sql_setdlist[10].sqllen = 8;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[10].sqldata = (void*)&htTempSdiCur.lOutStatisticsLocation;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[10].sqlind = &htTempSdiCur.indlOutStatisticsLocation;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[11].sqltype = 461; sql_setdlist[11].sqllen = 2;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[11].sqldata = (void*)htTempSdiCur.caOutLocationProperty;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[11].sqlind = &htTempSdiCur.indcaOutLocationProperty;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[12].sqltype = 461; sql_setdlist[12].sqllen = 501;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[12].sqldata = (void*)htTempSdiCur.caOutSdiFilterString;
#line 77 "t_dbs301_sdi_cur.sqc"
      sql_setdlist[12].sqlind = &htTempSdiCur.indcaOutSdiFilterString;
#line 77 "t_dbs301_sdi_cur.sqc"
      sqlasetdata(3,0,13,sql_setdlist,0L,0L);
    }
#line 77 "t_dbs301_sdi_cur.sqc"
  sqlacall((unsigned short)25,1,0,3,0L);
#line 77 "t_dbs301_sdi_cur.sqc"
  sqlastop(0L);
}

#line 77 "t_dbs301_sdi_cur.sqc"

          if ((sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND))
          {
              return sqlca.sqlcode;
          }
          if (sqlca.sqlcode == KR_DBOK)
              memcpy( ptSdiCur, &htTempSdiCur, sizeof(T_SdiCur));
          return sqlca.sqlcode;

      case KR_DBCURCLOSE:
          
/*
EXEC SQL CLOSE SdiCur_cur;
*/

{
#line 87 "t_dbs301_sdi_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 87 "t_dbs301_sdi_cur.sqc"
  sqlacall((unsigned short)20,1,0,0,0L);
#line 87 "t_dbs301_sdi_cur.sqc"
  sqlastop(0L);
}

#line 87 "t_dbs301_sdi_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      default:
          return -1;
  }
}
