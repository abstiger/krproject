static char sqla_program_id[292] = 
{
 172,0,65,69,65,78,65,73,104,65,55,104,82,82,75,99,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,84,73,71,69,82,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,84,95,68,66,83,50,48,50,0,0,0,0,0,0,0,0,
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


#line 1 "t_dbs202_set_cfg_cur.sqc"
#include <string.h> 
#include "dbs/dbs_basopr.h" 
#include "dbs/set_cfg_cur.h"


/*
EXEC SQL INCLUDE sqlca;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 5 "t_dbs202_set_cfg_cur.sqc"


/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 6 "t_dbs202_set_cfg_cur.sqc"
 

/*
EXEC SQL WHENEVER SQLWARNING CONTINUE;
*/

#line 7 "t_dbs202_set_cfg_cur.sqc"
 

/*
EXEC SQL WHENEVER NOT FOUND CONTINUE;
*/

#line 8 "t_dbs202_set_cfg_cur.sqc"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 10 "t_dbs202_set_cfg_cur.sqc"

struct dbsTempSetCfgCur_t
{
    sqlint64    lInSetId;    short       indlInSetId;
    char        caOutElementValue[200+1];    short       indcaOutElementValue;
    char        caOutElementDesc[200+1];    short       indcaOutElementDesc;
}htTempSetCfgCur;

char hcaSetCfgCurSql[2048+1]={0};

/*
EXEC SQL END DECLARE SECTION;
*/

#line 19 "t_dbs202_set_cfg_cur.sqc"



void dbsSetCfgCurInit(T_SetCfgCur *ptSetCfgCur)
{
    memset(ptSetCfgCur, 0x00, sizeof(T_SetCfgCur));
}


int dbsSetCfgCur(int iFuncCode, T_SetCfgCur *ptSetCfgCur)
{
    switch (iFuncCode)
    {
      case KR_DBCUROPEN:
          memcpy(&htTempSetCfgCur, ptSetCfgCur, sizeof(htTempSetCfgCur));
          strcpy(hcaSetCfgCurSql, "SELECT  A.ELEMENT_VALUE,  A.ELEMENT_DESC  FROM  KR_TBL_SET_CFG A  WHERE  A.SET_ID= ?   ");
          
/*
EXEC SQL PREPARE StmtSetCfgCur FROM :hcaSetCfgCurSql;
*/

{
#line 35 "t_dbs202_set_cfg_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 35 "t_dbs202_set_cfg_cur.sqc"
  sqlastls(0,hcaSetCfgCurSql,0L);
#line 35 "t_dbs202_set_cfg_cur.sqc"
  sqlacall((unsigned short)27,1,0,0,0L);
#line 35 "t_dbs202_set_cfg_cur.sqc"
  sqlastop(0L);
}

#line 35 "t_dbs202_set_cfg_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL DECLARE SetCfgCur_cur CURSOR FOR StmtSetCfgCur;
*/

#line 40 "t_dbs202_set_cfg_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL OPEN SetCfgCur_cur USING
                    :htTempSetCfgCur.lInSetId;
*/

{
#line 46 "t_dbs202_set_cfg_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 46 "t_dbs202_set_cfg_cur.sqc"
  sqlaaloc(2,1,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 46 "t_dbs202_set_cfg_cur.sqc"
      sql_setdlist[0].sqltype = 492; sql_setdlist[0].sqllen = 8;
#line 46 "t_dbs202_set_cfg_cur.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempSetCfgCur.lInSetId;
#line 46 "t_dbs202_set_cfg_cur.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 46 "t_dbs202_set_cfg_cur.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 46 "t_dbs202_set_cfg_cur.sqc"
  sqlacall((unsigned short)26,1,2,0,0L);
#line 46 "t_dbs202_set_cfg_cur.sqc"
  sqlastop(0L);
}

#line 46 "t_dbs202_set_cfg_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
            return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      case KR_DBCURFETCH:
          memcpy(&htTempSetCfgCur, ptSetCfgCur, sizeof(htTempSetCfgCur));
          
/*
EXEC SQL FETCH SetCfgCur_cur INTO
                     :htTempSetCfgCur.caOutElementValue :htTempSetCfgCur.indcaOutElementValue, 
                     :htTempSetCfgCur.caOutElementDesc :htTempSetCfgCur.indcaOutElementDesc;
*/

{
#line 57 "t_dbs202_set_cfg_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 57 "t_dbs202_set_cfg_cur.sqc"
  sqlaaloc(3,2,2,0L);
    {
      struct sqla_setdata_list sql_setdlist[2];
#line 57 "t_dbs202_set_cfg_cur.sqc"
      sql_setdlist[0].sqltype = 461; sql_setdlist[0].sqllen = 201;
#line 57 "t_dbs202_set_cfg_cur.sqc"
      sql_setdlist[0].sqldata = (void*)htTempSetCfgCur.caOutElementValue;
#line 57 "t_dbs202_set_cfg_cur.sqc"
      sql_setdlist[0].sqlind = &htTempSetCfgCur.indcaOutElementValue;
#line 57 "t_dbs202_set_cfg_cur.sqc"
      sql_setdlist[1].sqltype = 461; sql_setdlist[1].sqllen = 201;
#line 57 "t_dbs202_set_cfg_cur.sqc"
      sql_setdlist[1].sqldata = (void*)htTempSetCfgCur.caOutElementDesc;
#line 57 "t_dbs202_set_cfg_cur.sqc"
      sql_setdlist[1].sqlind = &htTempSetCfgCur.indcaOutElementDesc;
#line 57 "t_dbs202_set_cfg_cur.sqc"
      sqlasetdata(3,0,2,sql_setdlist,0L,0L);
    }
#line 57 "t_dbs202_set_cfg_cur.sqc"
  sqlacall((unsigned short)25,1,0,3,0L);
#line 57 "t_dbs202_set_cfg_cur.sqc"
  sqlastop(0L);
}

#line 57 "t_dbs202_set_cfg_cur.sqc"

          if ((sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND))
          {
              return sqlca.sqlcode;
          }
          if (sqlca.sqlcode == KR_DBOK)
              memcpy( ptSetCfgCur, &htTempSetCfgCur, sizeof(T_SetCfgCur));
          return sqlca.sqlcode;

      case KR_DBCURCLOSE:
          
/*
EXEC SQL CLOSE SetCfgCur_cur;
*/

{
#line 67 "t_dbs202_set_cfg_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 67 "t_dbs202_set_cfg_cur.sqc"
  sqlacall((unsigned short)20,1,0,0,0L);
#line 67 "t_dbs202_set_cfg_cur.sqc"
  sqlastop(0L);
}

#line 67 "t_dbs202_set_cfg_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      default:
          return -1;
  }
}
