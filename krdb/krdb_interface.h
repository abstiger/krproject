#ifndef __KRDB_INTERFACE_H__
#define __KRDB_INTERFACE_H__

#include "dbs/erm/tbl0_cur.h"
#include "dbs/erm/tbl1_cur.h"
#include "dbs/erm/tbl2_cur.h"
#include "dbs/erm/tbl3_cur.h"
#include "dbs/erm/tbl4_cur.h"
#include "dbs/erm/tbl5_cur.h"
#include "dbs/erm/tbl6_cur.h"
#include "dbs/erm/tbl7_cur.h"
#include "dbs/erm/tbl8_cur.h"
#include "dbs/erm/tbl9_cur.h"

typedef T_Tbl0Cur T_ErmTradDef_1;
typedef T_Tbl1Cur T_ErmQueryTradDef_2;
typedef T_Tbl2Cur T_ErmLoginTradDef_3;
typedef T_Tbl3Cur T_ErmSynMainDef_4;

typedef enum {
    KR_FIELDNO_DATASRC  = 0,
    KR_FIELDNO_CUST_NO  = 1,
    KR_FIELDNO_TXN_DATE = 2,
    KR_FIELDNO_TXN_TIME = 3
} E_FIELDNO;

extern void *GetFldValAssignFunc(int iDataSrcId);

#endif /* __KRDB_INTERFACE_H__ */
