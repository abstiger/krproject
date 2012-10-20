static char sqla_program_id[292] = 
{
 172,0,65,69,65,78,65,73,87,65,51,104,82,82,75,99,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,84,73,71,69,82,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,84,95,68,66,83,49,48,52,0,0,0,0,0,0,0,0,
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


#line 1 "t_dbs104_datasrc_field_cnt_sel.sqc"
#include <string.h> 
#include "dbs/dbs_basopr.h" 
#include "dbs/datasrc_field_cnt_sel.h"


/*
EXEC SQL INCLUDE sqlca;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 5 "t_dbs104_datasrc_field_cnt_sel.sqc"


/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 6 "t_dbs104_datasrc_field_cnt_sel.sqc"
 

/*
EXEC SQL WHENEVER SQLWARNING CONTINUE;
*/

#line 7 "t_dbs104_datasrc_field_cnt_sel.sqc"
 

/*
EXEC SQL WHENEVER NOT FOUND CONTINUE;
*/

#line 8 "t_dbs104_datasrc_field_cnt_sel.sqc"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 10 "t_dbs104_datasrc_field_cnt_sel.sqc"

struct dbsTempDatasrcFieldCntSel_t
{
    sqlint64    lInDatasrcId;    short       indlInDatasrcId;
    sqlint64    lOutFieldCnt;    short       indlOutFieldCnt;
}htTempDatasrcFieldCntSel;


/*
EXEC SQL END DECLARE SECTION;
*/

#line 17 "t_dbs104_datasrc_field_cnt_sel.sqc"



void dbsDatasrcFieldCntSelInit(T_DatasrcFieldCntSel *ptDatasrcFieldCntSel)
{
    memset(ptDatasrcFieldCntSel, 0x00, sizeof(*ptDatasrcFieldCntSel));
}


int dbsDatasrcFieldCntSel(int iFuncCode, T_DatasrcFieldCntSel *ptDatasrcFieldCntSel)
{
    switch (iFuncCode)
    {
      case KR_DBSELECT:
          memcpy(&htTempDatasrcFieldCntSel, ptDatasrcFieldCntSel, sizeof(htTempDatasrcFieldCntSel));
          
/*
EXEC SQL SELECT  COUNT(*) AS FIELD_CNT   INTO 
                     :htTempDatasrcFieldCntSel.lOutFieldCnt :htTempDatasrcFieldCntSel.indlOutFieldCnt
                   FROM   KR_TBL_DATASRC_FIELD_DEF  WHERE  DATASRC_ID=  :htTempDatasrcFieldCntSel.lInDatasrcId   ;
*/

{
#line 34 "t_dbs104_datasrc_field_cnt_sel.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 34 "t_dbs104_datasrc_field_cnt_sel.sqc"
  sqlaaloc(2,1,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 34 "t_dbs104_datasrc_field_cnt_sel.sqc"
      sql_setdlist[0].sqltype = 492; sql_setdlist[0].sqllen = 8;
#line 34 "t_dbs104_datasrc_field_cnt_sel.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempDatasrcFieldCntSel.lInDatasrcId;
#line 34 "t_dbs104_datasrc_field_cnt_sel.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 34 "t_dbs104_datasrc_field_cnt_sel.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 34 "t_dbs104_datasrc_field_cnt_sel.sqc"
  sqlaaloc(3,1,2,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 34 "t_dbs104_datasrc_field_cnt_sel.sqc"
      sql_setdlist[0].sqltype = 493; sql_setdlist[0].sqllen = 8;
#line 34 "t_dbs104_datasrc_field_cnt_sel.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempDatasrcFieldCntSel.lOutFieldCnt;
#line 34 "t_dbs104_datasrc_field_cnt_sel.sqc"
      sql_setdlist[0].sqlind = &htTempDatasrcFieldCntSel.indlOutFieldCnt;
#line 34 "t_dbs104_datasrc_field_cnt_sel.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 34 "t_dbs104_datasrc_field_cnt_sel.sqc"
  sqlacall((unsigned short)24,1,2,3,0L);
#line 34 "t_dbs104_datasrc_field_cnt_sel.sqc"
  sqlastop(0L);
}

#line 34 "t_dbs104_datasrc_field_cnt_sel.sqc"

          if ((sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND))
          {
              return sqlca.sqlcode;
          }
          memcpy( ptDatasrcFieldCntSel, &htTempDatasrcFieldCntSel, sizeof(htTempDatasrcFieldCntSel));
          return sqlca.sqlcode;

      default:
          return -1;
    }
}
