static char sqla_program_id[292] = 
{
 172,0,65,69,65,78,65,73,55,65,119,104,82,82,75,99,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,84,73,71,69,82,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,68,66,83,95,66,65,83,79,0,0,0,0,0,0,0,0,
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


#line 1 "dbs_basopr.sqc"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbs_basopr.h"


/*
EXEC SQL include sqlca;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 7 "dbs_basopr.sqc"



/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 9 "dbs_basopr.sqc"

   char H_dbName[100];
   char H_dbUser[100];
   char H_dbPass[100];

/*
EXEC SQL END DECLARE SECTION;
*/

#line 13 "dbs_basopr.sqc"


/*得到数据库密码*/
/*****************************************************************************/
/* FUNC:   int GetDbPwd(char * saPwd)                                        */
/* PARAMS: saPwd - 密码(O)                                                   */
/* RETURN:  0：成功                                                          */
/*         -1：失败                                                          */
/* DESC:   得到数据库密码                                                    */
/*****************************************************************************/
int GetDbPwd(char *saPwd)
{
    char    sDBUser[17];
    char    sDBPassword[17];

    char    sPwd[17];
    char    sFileName[128];
    FILE    *fp;
    int     i;
    int     j;

    memset(sDBPassword, 0x00, sizeof(sDBPassword));

    memset(sPwd,0,sizeof(sPwd));
    memset(sFileName,0,sizeof(sFileName));

    /*从环境变量中读取文件名*/
    sprintf(sFileName, "%s", getenv("TL_DBPWD_FILE"));

    /*打开文件*/
    fp = fopen (sFileName, "r");
    if(fp == NULL)
    {
        printf ("open file[%s] error\n", sFileName);
        return -1;
    }
    
    /*从文件中读取16个字节的密码*/
    if( fgets (sPwd, 16, fp) == NULL )
    {
        printf ("read from file[%s] error\n", sFileName);
        fclose(fp);
        return -2;
    }
    fclose(fp);

    /*通过一个规则的方式得到数据库的密码*/
    /*for(i=0;i<strlen(sPwd);i++)
    {
        if (sPwd[i] >= 50 && sPwd[i] <= 75 )
            sDBPassword[i] = sPwd[i] + 47 ;
        else if (sPwd[i] >= 40 && sPwd[i] <= 49)
            sDBPassword[i] = 'a' - sPwd[i];
        else
            sDBPassword[i] = sPwd[i];

    }*/
    for(i=0,j=0; i<strlen(sPwd); i=i+2)
    {
        sDBPassword[j]=((sPwd[i]-'A')<<4)+((sPwd[i+1]-'A')&0xf);
        j++;
    }
    sDBPassword[j]='\0';
        
    sprintf(saPwd,"%s",sDBPassword);

    return 0;
}


/*****************************************************************************/
/* FUNC:   int dbsDbConnect(void)                                            */
/* PARAMS: 无                                                                */
/* RETURN:  0：成功                                                          */
/*         -1：失败                                                          */
/* DESC:   连接数据库                                                        */
/*****************************************************************************/
int dbsDbConnect(void)
{
    memset(H_dbName, 0, sizeof(H_dbName));
    memset(H_dbUser, 0, sizeof(H_dbUser));
    memset(H_dbPass, 0, sizeof(H_dbPass));

    if (getenv("DBNAME") != NULL) {
	    strncpy(H_dbName, (char *)(getenv("DBNAME")), sizeof(H_dbName)-1);
    }
    
    if (getenv("DBUSER") != NULL) {
	    strncpy(H_dbUser, (char *)(getenv("DBUSER")), sizeof(H_dbUser)-1);
    }
    
    if (getenv("DBPASS") != NULL) {
	    strncpy(H_dbPass, (char *)(getenv("DBPASS")), sizeof(H_dbPass)-1);
    }

	/* 设置数据库用户口令 */
	/* GetDbPwd(H_dbPass); */

	//EXEC SQL CONNECT :H_dbUser IDENTIFIED BY :H_dbPass;
	
/*
EXEC SQL connect to :H_dbName;
*/

{
#line 112 "dbs_basopr.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 112 "dbs_basopr.sqc"
  sqlaaloc(2,1,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 112 "dbs_basopr.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 100;
#line 112 "dbs_basopr.sqc"
      sql_setdlist[0].sqldata = (void*)H_dbName;
#line 112 "dbs_basopr.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 112 "dbs_basopr.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 112 "dbs_basopr.sqc"
  sqlacall((unsigned short)29,4,2,0,0L);
#line 112 "dbs_basopr.sqc"
  sqlastop(0L);
}

#line 112 "dbs_basopr.sqc"
 
    if (sqlca.sqlcode != 0) 
    {
        return -1;
    }

    return 0;
}

/*****************************************************************************/
/* FUNC:   int dbsDbDisconnect(void)                                         */
/* PARAMS: 无                                                                */
/* RETURN:  0：成功                                                          */
/*         -1：失败                                                          */
/* DESC:   断开数据库                                                        */
/*****************************************************************************/
int dbsDbDisconnect(void)
{
    
/*
EXEC SQL disconnect current;
*/

{
#line 130 "dbs_basopr.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 130 "dbs_basopr.sqc"
  sqlacall((unsigned short)40,24,0,0,0L);
#line 130 "dbs_basopr.sqc"
  sqlastop(0L);
}

#line 130 "dbs_basopr.sqc"

    if (sqlca.sqlcode != 0)
    {
        return -1;
    }
    return 0;
}

/*****************************************************************************/
/* FUNC:   int dbsDbCommit(void)                                             */
/* PARAMS: 无                                                                */
/* RETURN:  0：成功                                                          */
/*         -1：失败                                                          */
/* DESC:   提交事务失败                                                      */
/*****************************************************************************/
int dbsDbCommit(void)
{
    
/*
EXEC SQL commit;
*/

{
#line 147 "dbs_basopr.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 147 "dbs_basopr.sqc"
  sqlacall((unsigned short)21,0,0,0,0L);
#line 147 "dbs_basopr.sqc"
  sqlastop(0L);
}

#line 147 "dbs_basopr.sqc"

    if (sqlca.sqlcode != 0)
    {
          return -1;
    }
    return 0;
}

/*****************************************************************************/
/* FUNC:   int dbsDbRollback(void)                                           */
/* PARAMS: 无                                                                */
/* RETURN:  0：成功                                                          */
/*         -1：失败                                                          */
/* DESC:   回滚事务失败                                                      */
/*****************************************************************************/
int dbsDbRollback(void)
{
    
/*
EXEC SQL rollback;
*/

{
#line 164 "dbs_basopr.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 164 "dbs_basopr.sqc"
  sqlacall((unsigned short)28,0,0,0,0L);
#line 164 "dbs_basopr.sqc"
  sqlastop(0L);
}

#line 164 "dbs_basopr.sqc"

    if (sqlca.sqlcode != 0)
    {
        return -1;
    }
    return 0;
}
