static char sqla_program_id[292] = 
{
 172,0,65,69,65,78,65,73,49,65,122,104,82,82,75,99,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,84,73,71,69,82,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,84,95,68,66,83,49,48,50,0,0,0,0,0,0,0,0,
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


#line 1 "t_dbs102_datasrc_index_cur.sqc"
#include <string.h> 
#include "dbs/dbs_basopr.h" 
#include "dbs/datasrc_index_cur.h"


/*
EXEC SQL INCLUDE sqlca;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 5 "t_dbs102_datasrc_index_cur.sqc"


/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 6 "t_dbs102_datasrc_index_cur.sqc"
 

/*
EXEC SQL WHENEVER SQLWARNING CONTINUE;
*/

#line 7 "t_dbs102_datasrc_index_cur.sqc"
 

/*
EXEC SQL WHENEVER NOT FOUND CONTINUE;
*/

#line 8 "t_dbs102_datasrc_index_cur.sqc"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 10 "t_dbs102_datasrc_index_cur.sqc"

struct dbsTempDatasrcIndexCur_t
{
    sqlint64    lInDatasrcId;    short       indlInDatasrcId;
    sqlint64    lOutDatasrcId;    short       indlOutDatasrcId;
    sqlint64    lOutIndexId;    short       indlOutIndexId;
    char        caOutIndexName[30+1];    short       indcaOutIndexName;
    char        caOutIndexDesc[100+1];    short       indcaOutIndexDesc;
    char        caOutIndexType[1+1];    short       indcaOutIndexType;
    sqlint64    lOutIndexFieldId;    short       indlOutIndexFieldId;
    sqlint64    lOutSortFieldId;    short       indlOutSortFieldId;
}htTempDatasrcIndexCur;

char hcaDatasrcIndexCurSql[2048+1]={0};

/*
EXEC SQL END DECLARE SECTION;
*/

#line 24 "t_dbs102_datasrc_index_cur.sqc"



void dbsDatasrcIndexCurInit(T_DatasrcIndexCur *ptDatasrcIndexCur)
{
    memset(ptDatasrcIndexCur, 0x00, sizeof(T_DatasrcIndexCur));
}


int dbsDatasrcIndexCur(int iFuncCode, T_DatasrcIndexCur *ptDatasrcIndexCur)
{
    switch (iFuncCode)
    {
      case KR_DBCUROPEN:
          memcpy(&htTempDatasrcIndexCur, ptDatasrcIndexCur, sizeof(htTempDatasrcIndexCur));
          strcpy(hcaDatasrcIndexCurSql, "SELECT  A.DATASRC_ID,  A.INDEX_ID,  A.INDEX_NAME,  A.INDEX_DESC,  A.INDEX_TYPE,  A.INDEX_FIELD_ID,  A.SORT_FIELD_ID  FROM  KR_TBL_DATASRC_INDEX_DEF A  WHERE  DATASRC_ID= ?   ORDER BY  A.DATASRC_ID,  A.INDEX_ID  ");
          
/*
EXEC SQL PREPARE StmtDatasrcIndexCur FROM :hcaDatasrcIndexCurSql;
*/

{
#line 40 "t_dbs102_datasrc_index_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 40 "t_dbs102_datasrc_index_cur.sqc"
  sqlastls(0,hcaDatasrcIndexCurSql,0L);
#line 40 "t_dbs102_datasrc_index_cur.sqc"
  sqlacall((unsigned short)27,1,0,0,0L);
#line 40 "t_dbs102_datasrc_index_cur.sqc"
  sqlastop(0L);
}

#line 40 "t_dbs102_datasrc_index_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL DECLARE DatasrcIndexCur_cur CURSOR FOR StmtDatasrcIndexCur;
*/

#line 45 "t_dbs102_datasrc_index_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          
/*
EXEC SQL OPEN DatasrcIndexCur_cur USING
                    :htTempDatasrcIndexCur.lInDatasrcId;
*/

{
#line 51 "t_dbs102_datasrc_index_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 51 "t_dbs102_datasrc_index_cur.sqc"
  sqlaaloc(2,1,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 51 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[0].sqltype = 492; sql_setdlist[0].sqllen = 8;
#line 51 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempDatasrcIndexCur.lInDatasrcId;
#line 51 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 51 "t_dbs102_datasrc_index_cur.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 51 "t_dbs102_datasrc_index_cur.sqc"
  sqlacall((unsigned short)26,1,2,0,0L);
#line 51 "t_dbs102_datasrc_index_cur.sqc"
  sqlastop(0L);
}

#line 51 "t_dbs102_datasrc_index_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
            return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      case KR_DBCURFETCH:
          memcpy(&htTempDatasrcIndexCur, ptDatasrcIndexCur, sizeof(htTempDatasrcIndexCur));
          
/*
EXEC SQL FETCH DatasrcIndexCur_cur INTO
                     :htTempDatasrcIndexCur.lOutDatasrcId :htTempDatasrcIndexCur.indlOutDatasrcId, 
                     :htTempDatasrcIndexCur.lOutIndexId :htTempDatasrcIndexCur.indlOutIndexId, 
                     :htTempDatasrcIndexCur.caOutIndexName :htTempDatasrcIndexCur.indcaOutIndexName, 
                     :htTempDatasrcIndexCur.caOutIndexDesc :htTempDatasrcIndexCur.indcaOutIndexDesc, 
                     :htTempDatasrcIndexCur.caOutIndexType :htTempDatasrcIndexCur.indcaOutIndexType, 
                     :htTempDatasrcIndexCur.lOutIndexFieldId :htTempDatasrcIndexCur.indlOutIndexFieldId, 
                     :htTempDatasrcIndexCur.lOutSortFieldId :htTempDatasrcIndexCur.indlOutSortFieldId;
*/

{
#line 67 "t_dbs102_datasrc_index_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 67 "t_dbs102_datasrc_index_cur.sqc"
  sqlaaloc(3,7,2,0L);
    {
      struct sqla_setdata_list sql_setdlist[7];
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[0].sqltype = 493; sql_setdlist[0].sqllen = 8;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempDatasrcIndexCur.lOutDatasrcId;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[0].sqlind = &htTempDatasrcIndexCur.indlOutDatasrcId;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[1].sqltype = 493; sql_setdlist[1].sqllen = 8;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[1].sqldata = (void*)&htTempDatasrcIndexCur.lOutIndexId;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[1].sqlind = &htTempDatasrcIndexCur.indlOutIndexId;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[2].sqltype = 461; sql_setdlist[2].sqllen = 31;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[2].sqldata = (void*)htTempDatasrcIndexCur.caOutIndexName;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[2].sqlind = &htTempDatasrcIndexCur.indcaOutIndexName;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[3].sqltype = 461; sql_setdlist[3].sqllen = 101;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[3].sqldata = (void*)htTempDatasrcIndexCur.caOutIndexDesc;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[3].sqlind = &htTempDatasrcIndexCur.indcaOutIndexDesc;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[4].sqltype = 461; sql_setdlist[4].sqllen = 2;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[4].sqldata = (void*)htTempDatasrcIndexCur.caOutIndexType;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[4].sqlind = &htTempDatasrcIndexCur.indcaOutIndexType;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[5].sqltype = 493; sql_setdlist[5].sqllen = 8;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[5].sqldata = (void*)&htTempDatasrcIndexCur.lOutIndexFieldId;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[5].sqlind = &htTempDatasrcIndexCur.indlOutIndexFieldId;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[6].sqltype = 493; sql_setdlist[6].sqllen = 8;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[6].sqldata = (void*)&htTempDatasrcIndexCur.lOutSortFieldId;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sql_setdlist[6].sqlind = &htTempDatasrcIndexCur.indlOutSortFieldId;
#line 67 "t_dbs102_datasrc_index_cur.sqc"
      sqlasetdata(3,0,7,sql_setdlist,0L,0L);
    }
#line 67 "t_dbs102_datasrc_index_cur.sqc"
  sqlacall((unsigned short)25,1,0,3,0L);
#line 67 "t_dbs102_datasrc_index_cur.sqc"
  sqlastop(0L);
}

#line 67 "t_dbs102_datasrc_index_cur.sqc"

          if ((sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND))
          {
              return sqlca.sqlcode;
          }
          if (sqlca.sqlcode == KR_DBOK)
              memcpy( ptDatasrcIndexCur, &htTempDatasrcIndexCur, sizeof(T_DatasrcIndexCur));
          return sqlca.sqlcode;

      case KR_DBCURCLOSE:
          
/*
EXEC SQL CLOSE DatasrcIndexCur_cur;
*/

{
#line 77 "t_dbs102_datasrc_index_cur.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 77 "t_dbs102_datasrc_index_cur.sqc"
  sqlacall((unsigned short)20,1,0,0,0L);
#line 77 "t_dbs102_datasrc_index_cur.sqc"
  sqlastop(0L);
}

#line 77 "t_dbs102_datasrc_index_cur.sqc"

          if (sqlca.sqlcode != KR_DBOK)
          {
              return sqlca.sqlcode;
          }
          return sqlca.sqlcode;

      default:
          return -1;
  }
}
