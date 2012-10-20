static char sqla_program_id[292] = 
{
 172,0,65,69,65,78,65,73,77,65,110,107,82,82,75,99,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,84,73,71,69,82,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,84,95,68,66,83,51,48,52,0,0,0,0,0,0,0,0,
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


#line 1 "t_dbs304_hdi_day_sel.sqc"
#include <string.h> 
#include "dbs/dbs_basopr.h" 
#include "dbs/hdi_day_sel.h"


/*
EXEC SQL INCLUDE sqlca;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 5 "t_dbs304_hdi_day_sel.sqc"


/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 6 "t_dbs304_hdi_day_sel.sqc"
 

/*
EXEC SQL WHENEVER SQLWARNING CONTINUE;
*/

#line 7 "t_dbs304_hdi_day_sel.sqc"
 

/*
EXEC SQL WHENEVER NOT FOUND CONTINUE;
*/

#line 8 "t_dbs304_hdi_day_sel.sqc"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 10 "t_dbs304_hdi_day_sel.sqc"

struct dbsTempHdiDaySel_t
{
    char        caInDataObject[30+1];    short       indcaInDataObject;
    sqlint64    lInDataId;    short       indlInDataId;
    char        caInDataDateBegin[8+1];    short       indcaInDataDateBegin;
    char        caInDataDateEnd[8+1];    short       indcaInDataDateEnd;
    double      dOutSumDataValue;    short       inddOutSumDataValue;
    sqlint64    lOutCountDataValue;    short       indlOutCountDataValue;
    double      dOutMinDataValue;    short       inddOutMinDataValue;
    double      dOutMaxDataValue;    short       inddOutMaxDataValue;
}htTempHdiDaySel;


/*
EXEC SQL END DECLARE SECTION;
*/

#line 23 "t_dbs304_hdi_day_sel.sqc"



void dbsHdiDaySelInit(T_HdiDaySel *ptHdiDaySel)
{
    memset(ptHdiDaySel, 0x00, sizeof(*ptHdiDaySel));
}


int dbsHdiDaySel(int iFuncCode, T_HdiDaySel *ptHdiDaySel)
{
    switch (iFuncCode)
    {
      case KR_DBSELECT:
          memcpy(&htTempHdiDaySel, ptHdiDaySel, sizeof(htTempHdiDaySel));
          
/*
EXEC SQL SELECT  SUM(A.DATA_VALUE) AS SUM_DATA_VALUE,  COUNT(A.DATA_VALUE) AS COUNT_DATA_VALUE,  MIN(A.DATA_VALUE) AS MIN_DATA_VALUE,  MAX(A.DATA_VALUE) AS MAX_DATA_VALUE   INTO 
                     :htTempHdiDaySel.dOutSumDataValue :htTempHdiDaySel.inddOutSumDataValue, 
                     :htTempHdiDaySel.lOutCountDataValue :htTempHdiDaySel.indlOutCountDataValue, 
                     :htTempHdiDaySel.dOutMinDataValue :htTempHdiDaySel.inddOutMinDataValue, 
                     :htTempHdiDaySel.dOutMaxDataValue :htTempHdiDaySel.inddOutMaxDataValue
                   FROM  KR_TBL_DYNDATA_DAY A  WHERE  A.DATA_OBJECT =   :htTempHdiDaySel.caInDataObject    AND A.DATA_ID =   :htTempHdiDaySel.lInDataId    AND A.DATA_DATE >=   :htTempHdiDaySel.caInDataDateBegin    AND A.DATA_DATE <=   :htTempHdiDaySel.caInDataDateEnd    ;
*/

{
#line 43 "t_dbs304_hdi_day_sel.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 43 "t_dbs304_hdi_day_sel.sqc"
  sqlaaloc(2,4,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[4];
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 31;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[0].sqldata = (void*)htTempHdiDaySel.caInDataObject;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[1].sqltype = 492; sql_setdlist[1].sqllen = 8;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[1].sqldata = (void*)&htTempHdiDaySel.lInDataId;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[2].sqltype = 460; sql_setdlist[2].sqllen = 9;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[2].sqldata = (void*)htTempHdiDaySel.caInDataDateBegin;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[3].sqltype = 460; sql_setdlist[3].sqllen = 9;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[3].sqldata = (void*)htTempHdiDaySel.caInDataDateEnd;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[3].sqlind = 0L;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sqlasetdata(2,0,4,sql_setdlist,0L,0L);
    }
#line 43 "t_dbs304_hdi_day_sel.sqc"
  sqlaaloc(3,4,2,0L);
    {
      struct sqla_setdata_list sql_setdlist[4];
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[0].sqltype = 481; sql_setdlist[0].sqllen = 8;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempHdiDaySel.dOutSumDataValue;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[0].sqlind = &htTempHdiDaySel.inddOutSumDataValue;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[1].sqltype = 493; sql_setdlist[1].sqllen = 8;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[1].sqldata = (void*)&htTempHdiDaySel.lOutCountDataValue;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[1].sqlind = &htTempHdiDaySel.indlOutCountDataValue;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[2].sqltype = 481; sql_setdlist[2].sqllen = 8;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[2].sqldata = (void*)&htTempHdiDaySel.dOutMinDataValue;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[2].sqlind = &htTempHdiDaySel.inddOutMinDataValue;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[3].sqltype = 481; sql_setdlist[3].sqllen = 8;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[3].sqldata = (void*)&htTempHdiDaySel.dOutMaxDataValue;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sql_setdlist[3].sqlind = &htTempHdiDaySel.inddOutMaxDataValue;
#line 43 "t_dbs304_hdi_day_sel.sqc"
      sqlasetdata(3,0,4,sql_setdlist,0L,0L);
    }
#line 43 "t_dbs304_hdi_day_sel.sqc"
  sqlacall((unsigned short)24,1,2,3,0L);
#line 43 "t_dbs304_hdi_day_sel.sqc"
  sqlastop(0L);
}

#line 43 "t_dbs304_hdi_day_sel.sqc"

          if ((sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND))
          {
              return sqlca.sqlcode;
          }
          memcpy( ptHdiDaySel, &htTempHdiDaySel, sizeof(htTempHdiDaySel));
          return sqlca.sqlcode;

      default:
          return -1;
    }
}
