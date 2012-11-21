#ifndef __DBS_BASOPR_H
#define __DBS_BASOPR_H

/* we use odbc version 3 */
#define ODBCVER 0x0351

/*sqlext.h --> sql.h --> sqltypes.h*/ 
#include "sqlext.h" 

/* db environment, one connection per this */
typedef struct _dbs_env_t 
{ 
    SQLHENV    henv; 
    SQLHDBC    hdbc; 
    SQLHSTMT   hstmt; 
}T_DbsEnv;

/* return code */
#define KR_DBOK              SQL_SUCCESS
#define KR_DBNOTFOUND        SQL_NO_DATA
#define KR_DBNULLVAL         SQL_NULL_DATA

/* db operation function code */
#define KR_DBSELECT          1
#define KR_DBLOCK            2
#define KR_DBINSERT          3
#define KR_DBUPDATE          4
#define KR_DBDELETE          5
#define KR_DBCUROPEN         6
#define KR_DBCURFETCH        7
#define KR_DBCURCLOSE        8
#define KR_DBDELETEALL       9
#define KR_DBEXPORTALL       0
#define KR_DBCURINIT        10

/* dbs base function */
T_DbsEnv *dbsConnect(char *dsn, char *user, char *pass);
int dbsDisconnect(T_DbsEnv *dbsenv);
int dbsCommit(T_DbsEnv *dbsenv);
int dbsRollback(T_DbsEnv *dbsenv);

#endif

