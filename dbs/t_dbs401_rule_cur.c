static char sqla_program_id[292] = 
{
 172,0,65,69,65,78,65,73,100,65,114,107,82,82,75,99,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,84,73,71,69,82,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,84,95,68,66,83,52,48,49,0,0,0,0,0,0,0,0,
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


#line 1 "t_dbs401_rule_cur.sqc"
#include <string.h> 
#include "dbs/dbs_basopr.h" 
#include "dbs/rule_cur.h"


/*
EXEC SQL INCLUDE sqlca;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 5 "t_dbs401_rule_cur.sqc"


/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 6 "t_dbs401_rule_cur.sqc"
 

/*
EXEC SQL WHENEVER SQLWARNING CONTINUE;
*/

#line 7 "t_dbs401_rule_cur.sqc"
 

/*
EXEC SQL WHENEVER NOT FOUND CONTINUE;
*/

#line 8 "t_dbs401_rule_cur.sqc"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 10 "t_dbs401_rule_cur.sqc"

struct dbsTempRuleCur_t
{
    sqlint64    lOutRuleId;    short       indlOutRuleId;
    char        caOutRuleName[30+1];    short       indcaOutRuleName;
    char        caOutRuleDesc[100+1];    short       indcaOutRuleDesc;
    char        caOutRuleString[500+1];    short       indcaOutRuleString;
    char        caOutRuleType[1+1];    short       indcaOutRuleType;
    sqlint64    lOutRuleWeight;    short       indlOutRuleWeight;
    double      dOutRiskAdvcThresh;    short       inddOutRiskAdvcThresh;
    double      dOutRiskNotifThresh;    short       inddOutRiskNotifThresh;
    double      dOutRiskAlertThresh;    short       inddOutRiskAlertThresh;
    double      dOutRiskWarnThresh;    short       inddOutRiskWarnThresh;
    char        caOutThreshIsConfig[4+1];    short       indcaOutThreshIsConfig;
}htTempRuleCur;

char hcaRuleCurSql[2048+1]={0};

/*
EXEC SQL END DECLARE SECTION;
*/

#line 27 "t_dbs401_rule_cur.sqc"



void dbsRuleCurInit(T_RuleCur *ptRuleCur)
{
    memset(ptRuleCur, 0x00, sizeof(T_RuleCur));
}


int dbsRuleCur(int iFuncCode, T_RuleCur *ptRuleCur)
{
    switch (iFuncCode)
    {
      case KR_DBCUROPEN:
          memcpy(&htTempRuleCur, ptRuleCur, sizeof(htTempRuleCur));
          strcpy(hcaRuleCurSql, "SELECT  A.RULE_ID,  A.RULE_NAME,  A.RULE_DESC,  A.RULE_STRING,  A.RULE_TYPE,  A.RULE_WEIGHT,  A.RISK_ADVC_THRESH,  A.RISK_NOTIF_THRESH,  A.RISK_ALERT_THRESH,  A.RISK_WARN_THRESH,  A.THRESH_IS_CONFIG  FROM  KR_TBL_RULE A  WHERE  A.RULE_STATUS = '1'  ORDER BY   A.RULE_ID  ");
          
/*
EXEC SQL PREPARE StmtRuleCur FROM :hcaRuleCurSql;
*/

{
#line 43 "t_dbs401_rule_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 43 "t_dbs401_rule_cur.sqc"
  sqlastls(0,hcaRuleCurSql,0L);
#line 43 "t_dbs401_rule_cur.sqc"
  sqlacall((unsigned short)27,1,0,0,0L);
#line 43 "t_dbs401_rule_cur.sqc"
  sqlastop(0L);
}

#line 43 "t_dbs401_rule_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL DECLARE RuleCur_cur CURSOR FOR StmtRuleCur;
*/

#line 48 "t_dbs401_rule_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL OPEN RuleCur_cur ;
*/

{
#line 53 "t_dbs401_rule_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 53 "t_dbs401_rule_cur.sqc"
  sqlacall((unsigned short)26,1,0,0,0L);
#line 53 "t_dbs401_rule_cur.sqc"
  sqlastop(0L);
}

#line 53 "t_dbs401_rule_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
            return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      case KR_DBCURFETCH:
          memcpy(&htTempRuleCur, ptRuleCur, sizeof(htTempRuleCur));
          
/*
EXEC SQL FETCH RuleCur_cur INTO
                     :htTempRuleCur.lOutRuleId :htTempRuleCur.indlOutRuleId, 
                     :htTempRuleCur.caOutRuleName :htTempRuleCur.indcaOutRuleName, 
                     :htTempRuleCur.caOutRuleDesc :htTempRuleCur.indcaOutRuleDesc, 
                     :htTempRuleCur.caOutRuleString :htTempRuleCur.indcaOutRuleString, 
                     :htTempRuleCur.caOutRuleType :htTempRuleCur.indcaOutRuleType, 
                     :htTempRuleCur.lOutRuleWeight :htTempRuleCur.indlOutRuleWeight, 
                     :htTempRuleCur.dOutRiskAdvcThresh :htTempRuleCur.inddOutRiskAdvcThresh, 
                     :htTempRuleCur.dOutRiskNotifThresh :htTempRuleCur.inddOutRiskNotifThresh, 
                     :htTempRuleCur.dOutRiskAlertThresh :htTempRuleCur.inddOutRiskAlertThresh, 
                     :htTempRuleCur.dOutRiskWarnThresh :htTempRuleCur.inddOutRiskWarnThresh, 
                     :htTempRuleCur.caOutThreshIsConfig :htTempRuleCur.indcaOutThreshIsConfig;
*/

{
#line 73 "t_dbs401_rule_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 73 "t_dbs401_rule_cur.sqc"
  sqlaaloc(3,11,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[11];
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[0].sqltype = 493; sql_setdlist[0].sqllen = 8;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempRuleCur.lOutRuleId;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[0].sqlind = &htTempRuleCur.indlOutRuleId;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[1].sqltype = 461; sql_setdlist[1].sqllen = 31;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[1].sqldata = (void*)htTempRuleCur.caOutRuleName;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[1].sqlind = &htTempRuleCur.indcaOutRuleName;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[2].sqltype = 461; sql_setdlist[2].sqllen = 101;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[2].sqldata = (void*)htTempRuleCur.caOutRuleDesc;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[2].sqlind = &htTempRuleCur.indcaOutRuleDesc;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[3].sqltype = 461; sql_setdlist[3].sqllen = 501;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[3].sqldata = (void*)htTempRuleCur.caOutRuleString;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[3].sqlind = &htTempRuleCur.indcaOutRuleString;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[4].sqltype = 461; sql_setdlist[4].sqllen = 2;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[4].sqldata = (void*)htTempRuleCur.caOutRuleType;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[4].sqlind = &htTempRuleCur.indcaOutRuleType;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[5].sqltype = 493; sql_setdlist[5].sqllen = 8;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[5].sqldata = (void*)&htTempRuleCur.lOutRuleWeight;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[5].sqlind = &htTempRuleCur.indlOutRuleWeight;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[6].sqltype = 481; sql_setdlist[6].sqllen = 8;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[6].sqldata = (void*)&htTempRuleCur.dOutRiskAdvcThresh;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[6].sqlind = &htTempRuleCur.inddOutRiskAdvcThresh;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[7].sqltype = 481; sql_setdlist[7].sqllen = 8;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[7].sqldata = (void*)&htTempRuleCur.dOutRiskNotifThresh;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[7].sqlind = &htTempRuleCur.inddOutRiskNotifThresh;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[8].sqltype = 481; sql_setdlist[8].sqllen = 8;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[8].sqldata = (void*)&htTempRuleCur.dOutRiskAlertThresh;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[8].sqlind = &htTempRuleCur.inddOutRiskAlertThresh;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[9].sqltype = 481; sql_setdlist[9].sqllen = 8;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[9].sqldata = (void*)&htTempRuleCur.dOutRiskWarnThresh;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[9].sqlind = &htTempRuleCur.inddOutRiskWarnThresh;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[10].sqltype = 461; sql_setdlist[10].sqllen = 5;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[10].sqldata = (void*)htTempRuleCur.caOutThreshIsConfig;
#line 73 "t_dbs401_rule_cur.sqc"
      sql_setdlist[10].sqlind = &htTempRuleCur.indcaOutThreshIsConfig;
#line 73 "t_dbs401_rule_cur.sqc"
      sqlasetdata(3,0,11,sql_setdlist,0L,0L);
    }
#line 73 "t_dbs401_rule_cur.sqc"
  sqlacall((unsigned short)25,1,0,3,0L);
#line 73 "t_dbs401_rule_cur.sqc"
  sqlastop(0L);
}

#line 73 "t_dbs401_rule_cur.sqc"

          if ((sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND))
          {
              return sqlca.sqlcode;
          }
          if (sqlca.sqlcode == KR_DBOK)
              memcpy( ptRuleCur, &htTempRuleCur, sizeof(T_RuleCur));
          return sqlca.sqlcode;

      case KR_DBCURCLOSE:
          
/*
EXEC SQL CLOSE RuleCur_cur;
*/

{
#line 83 "t_dbs401_rule_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 83 "t_dbs401_rule_cur.sqc"
  sqlacall((unsigned short)20,1,0,0,0L);
#line 83 "t_dbs401_rule_cur.sqc"
  sqlastop(0L);
}

#line 83 "t_dbs401_rule_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      default:
          return -1;
  }
}
