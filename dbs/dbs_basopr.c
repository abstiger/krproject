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

	rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &dbsenv->henv);
	if (rc != SQL_SUCCESS) {
		fprintf(stderr, "SQLAllocHandle ENV failed:[%d]!\n", rc);
		goto failure;
	}
	
	rc = SQLSetEnvAttr(dbsenv->henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0);
	if (rc != SQL_SUCCESS) {
		fprintf(stderr, "SQLSetEnvAttr failed:[%d]!\n", rc);
		goto failure;
	}

	rc = SQLAllocHandle(SQL_HANDLE_DBC, dbsenv->henv, &dbsenv->hdbc);
	if (rc != SQL_SUCCESS) {
		fprintf(stderr, "SQLAllocHandle DBC failed:[%d]!\n", rc);
		goto failure;
	}

	rc = SQLSetConnectAttr(dbsenv->hdbc, SQL_ATTR_ODBC_CURSORS, (SQLPOINTER) SQL_CUR_USE_ODBC, 0);
	if (rc != SQL_SUCCESS) {
		fprintf(stderr, "SQLSetConnectAttr failed:[%d]!\n", rc);
		goto failure;
	}

	rc = SQLConnect(dbsenv->hdbc, (SQLCHAR *)dsn, SQL_NTS, (SQLCHAR *)user, SQL_NTS, (SQLCHAR *)pass, SQL_NTS );
	if (rc != SQL_SUCCESS) {
		fprintf(stderr, "SQLConnect failed:[%d]!\n", rc);
		goto failure;
	}
	return dbsenv;

failure:
	if (dbsenv) {
		if (dbsenv->hdbc) {
			SQLDisconnect(dbsenv->hdbc);
		    SQLFreeHandle(SQL_HANDLE_DBC, dbsenv->hdbc);
		}
		if (dbsenv->henv) SQLFreeHandle(SQL_HANDLE_ENV, dbsenv->henv);
		free(dbsenv);
	}
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


