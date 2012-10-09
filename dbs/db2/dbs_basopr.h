#ifndef __DBS_BASOPR_H
#define __DBS_BASOPR_H

/* 数据库SQLCODE的宏定义 For DB2 */
#define KR_DBOK              0
#define KR_DBNOTFOUND        100
#define KR_DBNULLVAL         -305
#define KR_DBDUPKEY          -1
/*#define KR_DBIDXVIOLATE      -603*/

/* 数据库操作的功能代码宏定义 */
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

extern int dbsDbConnect(void);
extern int dbsDbDisconnect(void);
extern int dbsDbCommit(void);
extern int dbsDbRollback(void);

#endif


