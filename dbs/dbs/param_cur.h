/******************************************************************************
 *             This file was generated by rapodbc automatically.              *
 *                     Don't modify it by yourself!                           *
 *   Author: Tiger(https://github.com/AbsoluteTiger/rapodbc)                  *
 *      Time: Mon Oct 11 21:19:11 2021                               *
 ******************************************************************************/

#ifndef __ParamCur_H__
#define __ParamCur_H__

#include "dbs/dbs_basopr.h"

typedef struct 
{
    SQLHSTMT    hstmt;
    char        caInParamVersion[20+1];
    char        caInParamClassName[30+1];
    char        caOutParamClassName[30+1];
    char        caOutParamObjectKey[30+1];
    char        caOutParamObjectString[2048+1];
}T_ParamCur;

extern int dbsParamCur(T_DbsEnv *dbsenv, int iFuncCode, T_ParamCur *ptParamCur);
extern void dbsParamCurInit(T_ParamCur *ptParamCur);
#endif

