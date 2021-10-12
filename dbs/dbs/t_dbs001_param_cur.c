/******************************************************************************
 *             This file was generated by rapodbc automatically.              *
 *                     Don't modify it by yourself!                           *
 *   Author: Tiger(https://github.com/AbsoluteTiger/rapodbc)                  *
 *      Time: Mon Oct 11 21:19:11 2021                               *
 ******************************************************************************/

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include "param_cur.h"

void dbsParamCurInit(T_ParamCur *ptParamCur)
{
    memset(ptParamCur, 0x00, sizeof(T_ParamCur));
}


int dbsParamCur(T_DbsEnv *dbsenv, int iFuncCode, T_ParamCur *ptParamCur)
{
    SQLRETURN rc = SQL_SUCCESS;
    SQLLEN iLen = 0, iNTS = SQL_NTS;
    switch (iFuncCode)
    {
      case KR_DBCUROPEN:
      {
          char hcaParamCurSql[2048+1]={0};
          strcpy(hcaParamCurSql, "SELECT  A.PARAM_CLASS_NAME,  A.PARAM_OBJECT_KEY,  A.PARAM_OBJECT_STRING  FROM  KR_PARAM_OBJECT A  WHERE  A.PARAM_VERSION =  ?  AND  A.PARAM_CLASS_NAME =  ?   ");
          rc = SQLAllocHandle(SQL_HANDLE_STMT, dbsenv->hdbc, &ptParamCur->hstmt);
          if (rc != SQL_SUCCESS) {
              dbsGetError(dbsenv, ptParamCur->hstmt);
              return rc;
          }
          rc = SQLPrepare(ptParamCur->hstmt, (SQLCHAR *)hcaParamCurSql, SQL_NTS);
          if (rc != SQL_SUCCESS) {
              dbsGetError(dbsenv, ptParamCur->hstmt);
              return rc;
          }
          rc = SQLBindParameter(ptParamCur->hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_C_CHAR, 20, 0, ptParamCur->caInParamVersion, 21, &iNTS);
          if (rc != SQL_SUCCESS) {
              dbsGetError(dbsenv, ptParamCur->hstmt);
              return rc;
          }
          rc = SQLBindParameter(ptParamCur->hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_C_CHAR, 30, 0, ptParamCur->caInParamClassName, 31, &iNTS);
          if (rc != SQL_SUCCESS) {
              dbsGetError(dbsenv, ptParamCur->hstmt);
              return rc;
          }
          rc = SQLExecute(ptParamCur->hstmt);
          if (rc != SQL_SUCCESS) {
              dbsGetError(dbsenv, ptParamCur->hstmt);
              return rc;
          }
          rc = SQLBindCol(ptParamCur->hstmt, 1, SQL_C_CHAR, ptParamCur->caOutParamClassName, 31, &iNTS);
          if (rc != SQL_SUCCESS) {
              dbsGetError(dbsenv, ptParamCur->hstmt);
              return rc;
          }
          rc = SQLBindCol(ptParamCur->hstmt, 2, SQL_C_CHAR, ptParamCur->caOutParamObjectKey, 31, &iNTS);
          if (rc != SQL_SUCCESS) {
              dbsGetError(dbsenv, ptParamCur->hstmt);
              return rc;
          }
          rc = SQLBindCol(ptParamCur->hstmt, 3, SQL_C_CHAR, ptParamCur->caOutParamObjectString, 2049, &iNTS);
          if (rc != SQL_SUCCESS) {
              dbsGetError(dbsenv, ptParamCur->hstmt);
              return rc;
          }
          return rc;
      }
      case KR_DBCURFETCH:
          rc = SQLFetch(ptParamCur->hstmt);
          if (rc != SQL_SUCCESS && rc != SQL_NO_DATA) {
              dbsGetError(dbsenv, ptParamCur->hstmt);
              return rc;
          }
          return rc;

      case KR_DBCURCLOSE:
          rc = SQLFreeHandle(SQL_HANDLE_STMT, ptParamCur->hstmt);
          return rc;

      default:
          return -1;
  }
}
