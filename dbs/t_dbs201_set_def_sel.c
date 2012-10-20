static char sqla_program_id[292] = 
{
 172,0,65,69,65,78,65,73,104,66,53,104,82,82,75,99,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,84,73,71,69,82,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,84,95,68,66,83,50,48,49,0,0,0,0,0,0,0,0,
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


#line 1 "t_dbs201_set_def_sel.sqc"
#include <string.h> 
#include "dbs/dbs_basopr.h" 
#include "dbs/set_def_sel.h"


/*
EXEC SQL INCLUDE sqlca;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 5 "t_dbs201_set_def_sel.sqc"


/*
EXEC SQL WHENEVER SQLERROR CONTINUE;
*/

#line 6 "t_dbs201_set_def_sel.sqc"
 

/*
EXEC SQL WHENEVER SQLWARNING CONTINUE;
*/

#line 7 "t_dbs201_set_def_sel.sqc"
 

/*
EXEC SQL WHENEVER NOT FOUND CONTINUE;
*/

#line 8 "t_dbs201_set_def_sel.sqc"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 10 "t_dbs201_set_def_sel.sqc"

struct dbsTempSetDefSel_t
{
    sqlint64    lInSetId;    short       indlInSetId;
    sqlint64    lOutSetId;    short       indlOutSetId;
    char        caOutSetName[30+1];    short       indcaOutSetName;
    char        caOutSetUsage[2+1];    short       indcaOutSetUsage;
    char        caOutSetType[2+1];    short       indcaOutSetType;
    char        caOutElementType[1+1];    short       indcaOutElementType;
    sqlint64    lOutElementLength;    short       indlOutElementLength;
}htTempSetDefSel;


/*
EXEC SQL END DECLARE SECTION;
*/

#line 22 "t_dbs201_set_def_sel.sqc"



void dbsSetDefSelInit(T_SetDefSel *ptSetDefSel)
{
    memset(ptSetDefSel, 0x00, sizeof(*ptSetDefSel));
}


int dbsSetDefSel(int iFuncCode, T_SetDefSel *ptSetDefSel)
{
    switch (iFuncCode)
    {
      case KR_DBSELECT:
          memcpy(&htTempSetDefSel, ptSetDefSel, sizeof(htTempSetDefSel));
          
/*
EXEC SQL SELECT  A.SET_ID,  A.SET_NAME,  A.SET_USAGE,  A.SET_TYPE,  A.ELEMENT_TYPE,  A.ELEMENT_LENGTH   INTO 
                     :htTempSetDefSel.lOutSetId :htTempSetDefSel.indlOutSetId, 
                     :htTempSetDefSel.caOutSetName :htTempSetDefSel.indcaOutSetName, 
                     :htTempSetDefSel.caOutSetUsage :htTempSetDefSel.indcaOutSetUsage, 
                     :htTempSetDefSel.caOutSetType :htTempSetDefSel.indcaOutSetType, 
                     :htTempSetDefSel.caOutElementType :htTempSetDefSel.indcaOutElementType, 
                     :htTempSetDefSel.lOutElementLength :htTempSetDefSel.indlOutElementLength
                   FROM  KR_TBL_SET_DEF A  WHERE  A.SET_ID=  :htTempSetDefSel.lInSetId   ;
*/

{
#line 44 "t_dbs201_set_def_sel.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 44 "t_dbs201_set_def_sel.sqc"
  sqlaaloc(2,1,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[0].sqltype = 492; sql_setdlist[0].sqllen = 8;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempSetDefSel.lInSetId;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 44 "t_dbs201_set_def_sel.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 44 "t_dbs201_set_def_sel.sqc"
  sqlaaloc(3,6,2,0L);
    {
      struct sqla_setdata_list sql_setdlist[6];
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[0].sqltype = 493; sql_setdlist[0].sqllen = 8;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[0].sqldata = (void*)&htTempSetDefSel.lOutSetId;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[0].sqlind = &htTempSetDefSel.indlOutSetId;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[1].sqltype = 461; sql_setdlist[1].sqllen = 31;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[1].sqldata = (void*)htTempSetDefSel.caOutSetName;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[1].sqlind = &htTempSetDefSel.indcaOutSetName;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[2].sqltype = 461; sql_setdlist[2].sqllen = 3;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[2].sqldata = (void*)htTempSetDefSel.caOutSetUsage;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[2].sqlind = &htTempSetDefSel.indcaOutSetUsage;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[3].sqltype = 461; sql_setdlist[3].sqllen = 3;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[3].sqldata = (void*)htTempSetDefSel.caOutSetType;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[3].sqlind = &htTempSetDefSel.indcaOutSetType;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[4].sqltype = 461; sql_setdlist[4].sqllen = 2;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[4].sqldata = (void*)htTempSetDefSel.caOutElementType;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[4].sqlind = &htTempSetDefSel.indcaOutElementType;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[5].sqltype = 493; sql_setdlist[5].sqllen = 8;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[5].sqldata = (void*)&htTempSetDefSel.lOutElementLength;
#line 44 "t_dbs201_set_def_sel.sqc"
      sql_setdlist[5].sqlind = &htTempSetDefSel.indlOutElementLength;
#line 44 "t_dbs201_set_def_sel.sqc"
      sqlasetdata(3,0,6,sql_setdlist,0L,0L);
    }
#line 44 "t_dbs201_set_def_sel.sqc"
  sqlacall((unsigned short)24,1,2,3,0L);
#line 44 "t_dbs201_set_def_sel.sqc"
  sqlastop(0L);
}

#line 44 "t_dbs201_set_def_sel.sqc"

          if ((sqlca.sqlcode != KR_DBOK) && (sqlca.sqlcode != KR_DBNOTFOUND))
          {
              return sqlca.sqlcode;
          }
          memcpy( ptSetDefSel, &htTempSetDefSel, sizeof(htTempSetDefSel));
          return sqlca.sqlcode;

      default:
          return -1;
    }
}
