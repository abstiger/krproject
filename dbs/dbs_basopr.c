#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbs_basopr.h"


T_DbsEnv *dbsConnect(char *dsn, char *user, char *pass)
{
    int        rc;
    T_DbsEnv *dbsenv = calloc(1, sizeof(T_DbsEnv));
    if (dbsenv == NULL) {
        fprintf(stderr, "calloc DbsEnv failed:[%d]!\n", rc);
        goto failure;
    }
    dbsenv->dsn = strdup(dsn);
    dbsenv->user = strdup(user);
    dbsenv->pass = strdup(pass);

    rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &dbsenv->henv);
    if (rc != SQL_SUCCESS) {
        fprintf(stderr, "SQLAllocHandle ENV failed:[%d]!\n", rc);
        goto failure;
    }
    
    rc = SQLSetEnvAttr(dbsenv->henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0);
    if (rc != SQL_SUCCESS) {
        fprintf(stderr, "SQLSetEnvAttr SQL_OV_ODBC3 failed:[%d]!\n", rc);
        goto failure;
    }

    rc = SQLAllocHandle(SQL_HANDLE_DBC, dbsenv->henv, &dbsenv->hdbc);
    if (rc != SQL_SUCCESS) {
        fprintf(stderr, "SQLAllocHandle DBC failed:[%d]!\n", rc);
        goto failure;
    }

    /*
    rc = SQLSetConnectAttr(dbsenv->hdbc, SQL_ATTR_ODBC_CURSORS, (SQLPOINTER) SQL_CUR_USE_ODBC, 0);
    if (rc != SQL_SUCCESS) {
        fprintf(stderr, "SQLSetConnectAttr failed:[%d]!\n", rc);
        goto failure;
    }
    */
    rc = SQLSetConnectAttr(dbsenv->hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER) SQL_AUTOCOMMIT_OFF, 0);
    if (rc != SQL_SUCCESS) {
        fprintf(stderr, "SQLSetConnectAttr SQL_AUTOCOMMIT_OFF failed:[%d]!\n", rc);
        goto failure;
    }

    rc = SQLConnect(dbsenv->hdbc, dbsenv->dsn, SQL_NTS, dbsenv->user, SQL_NTS, dbsenv->pass, SQL_NTS);
    if (rc != SQL_SUCCESS) {
        fprintf(stderr, "SQLConnect failed:[%d]!\n", rc);
        goto failure;
    }

    return dbsenv;

failure:
    dbsDisconnect(dbsenv);
    return NULL;
}


int dbsDisconnect(T_DbsEnv *dbsenv)
{
    if (dbsenv) {
        if (dbsenv->hdbc) {
            SQLDisconnect(dbsenv->hdbc);
            SQLFreeHandle(SQL_HANDLE_DBC, dbsenv->hdbc);
        }
        if (dbsenv->henv) SQLFreeHandle(SQL_HANDLE_ENV, dbsenv->henv);
        free(dbsenv->dsn);
        free(dbsenv->user);
        free(dbsenv->pass);
        free(dbsenv);
    }
    return 0;
}


int dbsCommit(T_DbsEnv *dbsenv)
{
    return SQLEndTran(SQL_HANDLE_DBC, dbsenv->hdbc, SQL_COMMIT);
}


int dbsRollback(T_DbsEnv *dbsenv)
{
    return SQLEndTran(SQL_HANDLE_DBC, dbsenv->hdbc, SQL_ROLLBACK);
}


void dbsGetError(T_DbsEnv *dbsenv, SQLHSTMT hstmt)
{
    SQLSMALLINT   i=0, msglen=0;

    while ((SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i++, 
                    dbsenv->sqlstate, &dbsenv->sqlcode, dbsenv->sqlerrmsg, 
                    sizeof(dbsenv->sqlerrmsg), &msglen)) != SQL_NO_DATA) {
        if (msglen > 0) {
        /*
            fprintf(stderr, "sqlstate[%s], sqlcode[%d], sqlerrmsg[%s]\n",
                    dbsenv->sqlstate, dbsenv->sqlcode, dbsenv->sqlerrmsg);
        */
            break;
        }
    }
}

