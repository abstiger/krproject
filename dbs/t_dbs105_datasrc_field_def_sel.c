static char sqla_program_id[292] = 
{
 172,0,65,69,65,78,65,73,65,66,52,104,82,82,75,99,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,84,73,71,69,82,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,84,95,68,66,83,49,48,53,0,0,0,0,0,0,0,0,
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


#line 1 "t_dbs105_datasrc_field_def_sel.sqc"
#include <string.h> 
#include "dbs/dbs_basopr.h" 
#include "dbs/datasrc_field_def_sel.h"


/*
EXEC SQL INCLUDE sqlca;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 5 "t_dbs105_datasrc_field_def_sel.sqc"


/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 6 "t_dbs105_datasrc_field_def_sel.sqc"
 

/*
EXEC SQL WHENEVER SQLWARNING CONTINUE;
*/

#line 7 "t_dbs105_datasrc_field_def_sel.sqc"
 

/*
EXEC SQL WHENEVER NOT FOUND CONTINUE;
*/

#line 8 "t_dbs105_datasrc_field_def_sel.sqc"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 10 "t_dbs105_datasrc_field_def_sel.sqc"

struct dbsTempDatasrcFieldDefSel_t
{
    sqlint64    lInDatasrcId ;    short       indlInDatasrcId ;
    sqlint64    lInFieldId ;    short       indlInFieldId ;
    char        caOutFieldName[30+1];    short       indcaOutFieldName;
    char        caOutFieldDesc[100+1];    short       indcaOutFieldDesc;
    char        caOutFieldType[1+1];    short       indcaOutFieldType;
    double      dOutFieldLength;    short       inddOutFieldLength;
}htTempDatasrcFieldDefSel;


/*
EXEC SQL END DECLARE SECTION;
*/

#line 21 "t_dbs105_datasrc_field_def_sel.sqc"



void dbsDatasrcFieldDefSelInit(T_DatasrcFieldDefSel *ptDatasrcFieldDefSel)
{
    memset(ptDatasrcFieldDefSel, 0x00, sizeof(*ptDatasrcFieldDefSel));
}


int dbsDatasrcFieldDefSel(int iFuncCode, T_DatasrcFieldDefSel *ptDatasrcFieldDefSel)
{
    switch (iFuncCode)
    {
      case KR_DBSELECT:
          memcpy(&htTempDatasrcFieldDefSel, ptDatasrcFieldDefSel, sizeof(htTempDatasrcFieldDefSel));
          
/*
EXEC SQL SELECT  A.FIELD_NAME,  A.FIELD_DESC,  A.FIELD_TYPE,  A.FIELD_LENGTH   INTO 
                     :htTempDatasrcFieldDefSel.caOutFieldName :htTempDatasrcFieldDefSel.indcaOutFieldName, 
                     :htTempDatasrcFieldDefSel.caOutFieldDesc :htTempDatasrcFieldDefSel.indcaOutFieldDesc, 
                     :htTempDatasrcFieldDefSel.caOutFieldType :htTempDatasrcFieldDefSel.indcaOutFieldType, 
                     :htTempDatasrcFieldDefSel.dOutFieldLength :htTempDatasrcFieldDefSel.inddOutFieldLength
                   FROM   KR_TBL_DATASRC_FIELD_DEF A  WHERE  A.DATASRC_ID =   :htTempDatasrcFieldDefSel.lInDatasrcId   AND   A.FIELD_ID =   :htTempDatasrcFieldDefSel.lInFieldId    ;
*/

{
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
  sqlaaloc(2,2,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[2];
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[0].sqltype = 492; sql_setdlist[0].sqllen = 8;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempDatasrcFieldDefSel.lInDatasrcId;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[1].sqltype = 492; sql_setdlist[1].sqllen = 8;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[1].sqldata = (void*)&htTempDatasrcFieldDefSel.lInFieldId;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sqlasetdata(2,0,2,sql_setdlist,0L,0L);
    }
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
  sqlaaloc(3,4,2,0L);
    {
      struct sqla_setdata_list sql_setdlist[4];
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[0].sqltype = 461; sql_setdlist[0].sqllen = 31;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[0].sqldata = (void*)htTempDatasrcFieldDefSel.caOutFieldName;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[0].sqlind = &htTempDatasrcFieldDefSel.indcaOutFieldName;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[1].sqltype = 461; sql_setdlist[1].sqllen = 101;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[1].sqldata = (void*)htTempDatasrcFieldDefSel.caOutFieldDesc;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[1].sqlind = &htTempDatasrcFieldDefSel.indcaOutFieldDesc;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[2].sqltype = 461; sql_setdlist[2].sqllen = 2;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[2].sqldata = (void*)htTempDatasrcFieldDefSel.caOutFieldType;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[2].sqlind = &htTempDatasrcFieldDefSel.indcaOutFieldType;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[3].sqltype = 481; sql_setdlist[3].sqllen = 8;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[3].sqldata = (void*)&htTempDatasrcFieldDefSel.dOutFieldLength;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sql_setdlist[3].sqlind = &htTempDatasrcFieldDefSel.inddOutFieldLength;
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
      sqlasetdata(3,0,4,sql_setdlist,0L,0L);
    }
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
  sqlacall((unsigned short)24,1,2,3,0L);
#line 41 "t_dbs105_datasrc_field_def_sel.sqc"
  sqlastop(0L);
}

#line 41 "t_dbs105_datasrc_field_def_sel.sqc"

          if ((sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND))
          {
              return sqlca.sqlcode;
          }
          memcpy( ptDatasrcFieldDefSel, &htTempDatasrcFieldDefSel, sizeof(htTempDatasrcFieldDefSel));
          return sqlca.sqlcode;

      default:
          return -1;
    }
}
