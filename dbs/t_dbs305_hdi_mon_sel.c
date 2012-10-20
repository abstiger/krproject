static char sqla_program_id[292] = 
{
 172,0,65,69,65,78,65,73,51,65,111,107,82,82,75,99,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,84,73,71,69,82,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,84,95,68,66,83,51,48,53,0,0,0,0,0,0,0,0,
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


#line 1 "t_dbs305_hdi_mon_sel.sqc"
#include <string.h> 
#include "dbs/dbs_basopr.h" 
#include "dbs/hdi_mon_sel.h"


/*
EXEC SQL INCLUDE sqlca;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 5 "t_dbs305_hdi_mon_sel.sqc"


/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 6 "t_dbs305_hdi_mon_sel.sqc"
 

/*
EXEC SQL WHENEVER SQLWARNING CONTINUE;
*/

#line 7 "t_dbs305_hdi_mon_sel.sqc"
 

/*
EXEC SQL WHENEVER NOT FOUND CONTINUE;
*/

#line 8 "t_dbs305_hdi_mon_sel.sqc"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 10 "t_dbs305_hdi_mon_sel.sqc"

struct dbsTempHdiMonSel_t
{
    char        caInDataObject[30+1];    short       indcaInDataObject;
    sqlint64    lInDataId;    short       indlInDataId;
    char        caInDataMonthBegin[6+1];    short       indcaInDataMonthBegin;
    char        caInDataMonthEnd[6+1];    short       indcaInDataMonthEnd;
    double      dOutSumDataValue;    short       inddOutSumDataValue;
    sqlint64    lOutCountDataValue;    short       indlOutCountDataValue;
    double      dOutMinDataValue;    short       inddOutMinDataValue;
    double      dOutMaxDataValue;    short       inddOutMaxDataValue;
}htTempHdiMonSel;


/*
EXEC SQL END DECLARE SECTION;
*/

#line 23 "t_dbs305_hdi_mon_sel.sqc"



void dbsHdiMonSelInit(T_HdiMonSel *ptHdiMonSel)
{
    memset(ptHdiMonSel, 0x00, sizeof(*ptHdiMonSel));
}


int dbsHdiMonSel(int iFuncCode, T_HdiMonSel *ptHdiMonSel)
{
    switch (iFuncCode)
    {
      case KR_DBSELECT:
          memcpy(&htTempHdiMonSel, ptHdiMonSel, sizeof(htTempHdiMonSel));
          
/*
EXEC SQL SELECT  SUM(A.DATA_VALUE) AS SUM_DATA_VALUE,  COUNT(A.DATA_VALUE) AS COUNT_DATA_VALUE,  MIN(A.DATA_VALUE) AS MIN_DATA_VALUE,  MAX(A.DATA_VALUE) AS MAX_DATA_VALUE   INTO 
                     :htTempHdiMonSel.dOutSumDataValue :htTempHdiMonSel.inddOutSumDataValue, 
                     :htTempHdiMonSel.lOutCountDataValue :htTempHdiMonSel.indlOutCountDataValue, 
                     :htTempHdiMonSel.dOutMinDataValue :htTempHdiMonSel.inddOutMinDataValue, 
                     :htTempHdiMonSel.dOutMaxDataValue :htTempHdiMonSel.inddOutMaxDataValue
                   FROM  KR_TBL_DYNDATA_MON A  WHERE  A.DATA_OBJECT =   :htTempHdiMonSel.caInDataObject    AND A.DATA_ID =   :htTempHdiMonSel.lInDataId    AND A.DATA_MONTH >=   :htTempHdiMonSel.caInDataMonthBegin    AND A.DATA_MONTH <=   :htTempHdiMonSel.caInDataMonthEnd    ;
*/

{
#line 43 "t_dbs305_hdi_mon_sel.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 43 "t_dbs305_hdi_mon_sel.sqc"
  sqlaaloc(2,4,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[4];
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 31;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[0].sqldata = (void*)htTempHdiMonSel.caInDataObject;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[1].sqltype = 492; sql_setdlist[1].sqllen = 8;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[1].sqldata = (void*)&htTempHdiMonSel.lInDataId;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[2].sqltype = 460; sql_setdlist[2].sqllen = 7;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[2].sqldata = (void*)htTempHdiMonSel.caInDataMonthBegin;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[3].sqltype = 460; sql_setdlist[3].sqllen = 7;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[3].sqldata = (void*)htTempHdiMonSel.caInDataMonthEnd;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[3].sqlind = 0L;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sqlasetdata(2,0,4,sql_setdlist,0L,0L);
    }
#line 43 "t_dbs305_hdi_mon_sel.sqc"
  sqlaaloc(3,4,2,0L);
    {
      struct sqla_setdata_list sql_setdlist[4];
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[0].sqltype = 481; sql_setdlist[0].sqllen = 8;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempHdiMonSel.dOutSumDataValue;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[0].sqlind = &htTempHdiMonSel.inddOutSumDataValue;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[1].sqltype = 493; sql_setdlist[1].sqllen = 8;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[1].sqldata = (void*)&htTempHdiMonSel.lOutCountDataValue;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[1].sqlind = &htTempHdiMonSel.indlOutCountDataValue;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[2].sqltype = 481; sql_setdlist[2].sqllen = 8;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[2].sqldata = (void*)&htTempHdiMonSel.dOutMinDataValue;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[2].sqlind = &htTempHdiMonSel.inddOutMinDataValue;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[3].sqltype = 481; sql_setdlist[3].sqllen = 8;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[3].sqldata = (void*)&htTempHdiMonSel.dOutMaxDataValue;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sql_setdlist[3].sqlind = &htTempHdiMonSel.inddOutMaxDataValue;
#line 43 "t_dbs305_hdi_mon_sel.sqc"
      sqlasetdata(3,0,4,sql_setdlist,0L,0L);
    }
#line 43 "t_dbs305_hdi_mon_sel.sqc"
  sqlacall((unsigned short)24,1,2,3,0L);
#line 43 "t_dbs305_hdi_mon_sel.sqc"
  sqlastop(0L);
}

#line 43 "t_dbs305_hdi_mon_sel.sqc"

          if ((sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND))
          {
              return sqlca.sqlcode;
          }
          memcpy( ptHdiMonSel, &htTempHdiMonSel, sizeof(htTempHdiMonSel));
          return sqlca.sqlcode;

      default:
          return -1;
    }
}
