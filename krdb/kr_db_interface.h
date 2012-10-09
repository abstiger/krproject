#ifndef __KR_DB_INTERFACE_H__
#define __KR_DB_INTERFACE_H__


typedef struct _kr_tradflow_t{
    char     caOutCustNo[20+1];
    char     caOutTransDate[8+1];
    char     caOutTransTime[6+1];
    char     caOutFlowNo[15+1];
    char     caOutTransType[2+1];
    double   dOutTransAmt;
    char     caOutTransLoc[100+1];
} T_KRTradFlow_1;


extern void *GetFldValAssignFunc(int iDataSrcId);

#endif /* __KR_DB_INTERFACE_H__ */
