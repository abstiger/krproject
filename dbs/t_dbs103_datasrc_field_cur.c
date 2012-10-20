static char sqla_program_id[292] = 
{
 172,0,65,69,65,78,65,73,87,66,49,104,82,82,75,99,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,84,73,71,69,82,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,84,95,68,66,83,49,48,51,0,0,0,0,0,0,0,0,
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


#line 1 "t_dbs103_datasrc_field_cur.sqc"
#include <string.h> 
#include "dbs/dbs_basopr.h" 
#include "dbs/datasrc_field_cur.h"


/*
EXEC SQL INCLUDE sqlca;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 5 "t_dbs103_datasrc_field_cur.sqc"


/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 6 "t_dbs103_datasrc_field_cur.sqc"
 

/*
EXEC SQL WHENEVER SQLWARNING CONTINUE;
*/

#line 7 "t_dbs103_datasrc_field_cur.sqc"
 

/*
EXEC SQL WHENEVER NOT FOUND CONTINUE;
*/

#line 8 "t_dbs103_datasrc_field_cur.sqc"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 10 "t_dbs103_datasrc_field_cur.sqc"

struct dbsTempDatasrcFieldCur_t
{
    sqlint64    lInDatasrcId;    short       indlInDatasrcId;
    sqlint64    lOutDatasrcId;    short       indlOutDatasrcId;
    sqlint64    lOutFieldId;    short       indlOutFieldId;
    char        caOutFieldName[30+1];    short       indcaOutFieldName;
    char        caOutFieldDesc[100+1];    short       indcaOutFieldDesc;
    char        caOutFieldType[1+1];    short       indcaOutFieldType;
    double      dOutFieldLength;    short       inddOutFieldLength;
}htTempDatasrcFieldCur;

char hcaDatasrcFieldCurSql[2048+1]={0};

/*
EXEC SQL END DECLARE SECTION;
*/

#line 23 "t_dbs103_datasrc_field_cur.sqc"



void dbsDatasrcFieldCurInit(T_DatasrcFieldCur *ptDatasrcFieldCur)
{
    memset(ptDatasrcFieldCur, 0x00, sizeof(T_DatasrcFieldCur));
}


int dbsDatasrcFieldCur(int iFuncCode, T_DatasrcFieldCur *ptDatasrcFieldCur)
{
    switch (iFuncCode)
    {
      case KR_DBCUROPEN:
          memcpy(&htTempDatasrcFieldCur, ptDatasrcFieldCur, sizeof(htTempDatasrcFieldCur));
          strcpy(hcaDatasrcFieldCurSql, "SELECT  A.DATASRC_ID,  A.FIELD_ID,  A.FIELD_NAME,  A.FIELD_DESC,  A.FIELD_TYPE,  A.FIELD_LENGTH  FROM  KR_TBL_DATASRC_FIELD_DEF A  WHERE  DATASRC_ID= ?   ORDER BY  A.DATASRC_ID,  A.FIELD_ID  ");
          
/*
EXEC SQL PREPARE StmtDatasrcFieldCur FROM :hcaDatasrcFieldCurSql;
*/

{
#line 39 "t_dbs103_datasrc_field_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 39 "t_dbs103_datasrc_field_cur.sqc"
  sqlastls(0,hcaDatasrcFieldCurSql,0L);
#line 39 "t_dbs103_datasrc_field_cur.sqc"
  sqlacall((unsigned short)27,1,0,0,0L);
#line 39 "t_dbs103_datasrc_field_cur.sqc"
  sqlastop(0L);
}

#line 39 "t_dbs103_datasrc_field_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL DECLARE DatasrcFieldCur_cur CURSOR FOR StmtDatasrcFieldCur;
*/

#line 44 "t_dbs103_datasrc_field_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL OPEN DatasrcFieldCur_cur USING
                    :htTempDatasrcFieldCur.lInDatasrcId;
*/

{
#line 50 "t_dbs103_datasrc_field_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 50 "t_dbs103_datasrc_field_cur.sqc"
  sqlaaloc(2,1,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 50 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[0].sqltype = 492; sql_setdlist[0].sqllen = 8;
#line 50 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempDatasrcFieldCur.lInDatasrcId;
#line 50 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 50 "t_dbs103_datasrc_field_cur.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 50 "t_dbs103_datasrc_field_cur.sqc"
  sqlacall((unsigned short)26,1,2,0,0L);
#line 50 "t_dbs103_datasrc_field_cur.sqc"
  sqlastop(0L);
}

#line 50 "t_dbs103_datasrc_field_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
            return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      case KR_DBCURFETCH:
          memcpy(&htTempDatasrcFieldCur, ptDatasrcFieldCur, sizeof(htTempDatasrcFieldCur));
          
/*
EXEC SQL FETCH DatasrcFieldCur_cur INTO
                     :htTempDatasrcFieldCur.lOutDatasrcId :htTempDatasrcFieldCur.indlOutDatasrcId, 
                     :htTempDatasrcFieldCur.lOutFieldId :htTempDatasrcFieldCur.indlOutFieldId, 
                     :htTempDatasrcFieldCur.caOutFieldName :htTempDatasrcFieldCur.indcaOutFieldName, 
                     :htTempDatasrcFieldCur.caOutFieldDesc :htTempDatasrcFieldCur.indcaOutFieldDesc, 
                     :htTempDatasrcFieldCur.caOutFieldType :htTempDatasrcFieldCur.indcaOutFieldType, 
                     :htTempDatasrcFieldCur.dOutFieldLength :htTempDatasrcFieldCur.inddOutFieldLength;
*/

{
#line 65 "t_dbs103_datasrc_field_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 65 "t_dbs103_datasrc_field_cur.sqc"
  sqlaaloc(3,6,2,0L);
    {
      struct sqla_setdata_list sql_setdlist[6];
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[0].sqltype = 493; sql_setdlist[0].sqllen = 8;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempDatasrcFieldCur.lOutDatasrcId;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[0].sqlind = &htTempDatasrcFieldCur.indlOutDatasrcId;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[1].sqltype = 493; sql_setdlist[1].sqllen = 8;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[1].sqldata = (void*)&htTempDatasrcFieldCur.lOutFieldId;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[1].sqlind = &htTempDatasrcFieldCur.indlOutFieldId;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[2].sqltype = 461; sql_setdlist[2].sqllen = 31;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[2].sqldata = (void*)htTempDatasrcFieldCur.caOutFieldName;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[2].sqlind = &htTempDatasrcFieldCur.indcaOutFieldName;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[3].sqltype = 461; sql_setdlist[3].sqllen = 101;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[3].sqldata = (void*)htTempDatasrcFieldCur.caOutFieldDesc;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[3].sqlind = &htTempDatasrcFieldCur.indcaOutFieldDesc;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[4].sqltype = 461; sql_setdlist[4].sqllen = 2;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[4].sqldata = (void*)htTempDatasrcFieldCur.caOutFieldType;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[4].sqlind = &htTempDatasrcFieldCur.indcaOutFieldType;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[5].sqltype = 481; sql_setdlist[5].sqllen = 8;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[5].sqldata = (void*)&htTempDatasrcFieldCur.dOutFieldLength;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sql_setdlist[5].sqlind = &htTempDatasrcFieldCur.inddOutFieldLength;
#line 65 "t_dbs103_datasrc_field_cur.sqc"
      sqlasetdata(3,0,6,sql_setdlist,0L,0L);
    }
#line 65 "t_dbs103_datasrc_field_cur.sqc"
  sqlacall((unsigned short)25,1,0,3,0L);
#line 65 "t_dbs103_datasrc_field_cur.sqc"
  sqlastop(0L);
}

#line 65 "t_dbs103_datasrc_field_cur.sqc"

          if ((sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND))
          {
              return sqlca.sqlcode;
          }
          if (sqlca.sqlcode == KR_DBOK)
              memcpy( ptDatasrcFieldCur, &htTempDatasrcFieldCur, sizeof(T_DatasrcFieldCur));
          return sqlca.sqlcode;

      case KR_DBCURCLOSE:
          
/*
EXEC SQL CLOSE DatasrcFieldCur_cur;
*/

{
#line 75 "t_dbs103_datasrc_field_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 75 "t_dbs103_datasrc_field_cur.sqc"
  sqlacall((unsigned short)20,1,0,0,0L);
#line 75 "t_dbs103_datasrc_field_cur.sqc"
  sqlastop(0L);
}

#line 75 "t_dbs103_datasrc_field_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      default:
          return -1;
  }
}
