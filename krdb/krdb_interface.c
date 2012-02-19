#include "bas/bas_global.h"
#include "krdb/krdb.h"
#include "krdb/krdb_interface.h"
#include "shm/ip.h"
#include "shm/shm_sharemem.h"
#include "shm/blk_ip.h"

#include <stdlib.h>

static int BsearchCmp(const void *a, const void *b);
static int BLK_BsearchCmp(const void *a, const void *b);
static void SetAreaInfo(T_KRRecord  *ptRecord, double dOutLongIp, int iArea_no, int iArea_pro, int iArea_city, int iFlg__no);
static void SetBLKIPFlg(T_KRRecord  *ptRecord, double dOutLongIp, int iFlg_no);
static int CalCustAge(char *caChanlCustNo);

void AssignFldValFunc_1(void *p1, void *p2);
void AssignFldValFunc_2(void *p1, void *p2);
void AssignFldValFunc_3(void *p1, void *p2);
void AssignFldValFunc_4(void *p1, void *p2);


void *GetFldValAssignFunc(int iDataSrcId)
{
    switch(iDataSrcId)
    {
        case 1:
            return (void *)AssignFldValFunc_1;
            break;
        case 2:
            return (void *)AssignFldValFunc_2;
            break;    
        case 3:
            return (void *)AssignFldValFunc_3;
            break;
        case 4:
            return (void *)AssignFldValFunc_4;
            break; 
        default:
            return NULL;
            break;
    }
}



static int BsearchCmp(const void *a, const void *b)
{
    if (*(double *)a < ((IPLIB *)b)->dLongStart)
    {
        return -1;
    }
    if (*(double *)a > ((IPLIB *)b)->dLongEnd)
    {
        return 1;
    }
    return 0;
}

static int BLK_BsearchCmp(const void *a, const void *b)
{
    if (*(double *)a < ((T_BLKIP *)b)->dLongStart)
    {
        return -1;
    }
    if (*(double *)a > ((T_BLKIP *)b)->dLongEnd)
    {
        return 1;
    }
    return 0;
}



static void SetAreaInfo(T_KRRecord  *ptRecord, double dOutLongIp, int iArea_no, int iArea_pro, int iArea_city, int iFlg_no)
{
    T_IPDef   *ptIPDef = &gtpShmBuf->taIPItem[gnSecId];
    T_IpVal   *ptIPVal = &ptIPDef->stIpLibDef;
    IPLIB     *ptResult = NULL;
    void *pFldVal = NULL;

    ptResult = (IPLIB *) bsearch(&dOutLongIp, ptIPVal->stIpLib, ptIPVal->lIpCnt, sizeof(IPLIB), BsearchCmp);
    if (ptResult != NULL)
    {
         pFldVal = kr_get_field_value(ptRecord, iArea_no);
         memcpy(pFldVal, ptResult->cAreaNo, 6);

         pFldVal = kr_get_field_value(ptRecord, iArea_pro);
         memcpy(pFldVal, ptResult->cAreaNo, 2);

         pFldVal = kr_get_field_value(ptRecord, iArea_city);
         memcpy(pFldVal, ptResult->cAreaNo, 4);

         pFldVal = kr_get_field_value(ptRecord, iFlg_no);
         memcpy(pFldVal, ptResult->cFlg, 1);
    }
}


static void SetBLKIPFlg(T_KRRecord  *ptRecord, double dOutLongIp, int iFlg_no)
{
    T_BLKIPDef   *ptBLKIPDef = &gtpShmBuf->taBLKIPItem[gnSecId];
    T_BLKIpVal   *ptBLKIPVal = &ptBLKIPDef->stBLKIPDef;
    IPLIB     *ptResult = NULL;
    void *pFldVal = NULL;

    ptResult = (IPLIB *) bsearch(&dOutLongIp, ptBLKIPVal->stBLKIP, ptBLKIPVal->lIpCnt, sizeof(T_BLKIP), BLK_BsearchCmp);
    if (ptResult != NULL)
    {
         pFldVal = kr_get_field_value(ptRecord, iFlg_no);
         memcpy(pFldVal, "1", 1);
    }
    else
    {
        pFldVal = kr_get_field_value(ptRecord, iFlg_no);
        memcpy(pFldVal, "0", 1);
    }
}

static int CalCustAge(char *caChanlCustNo)
{

    struct timeval    tv;
    struct tm        *ptm;
    char caIdNo[17+1];
    char caDate[4+1];
    long iYear=0;
    memset(caIdNo,0,sizeof(caIdNo));
    memset(caDate,0,sizeof(caDate));
    basRTrim(caChanlCustNo);

    gettimeofday(&tv, NULL);
    ptm = localtime(&(tv.tv_sec));

    iYear = ptm->tm_year+1900;

    if(strlen(caChanlCustNo)==15 && basIsNumeric(caChanlCustNo))
    {
        return (iYear-basAtol(caChanlCustNo+6,2)-1900);
    }
    else if(strlen(caChanlCustNo)==18)
    {
        memcpy(caIdNo,caChanlCustNo,17);
        if(basIsNumeric(caIdNo))
            return (iYear-basAtol(caIdNo+6,4));
        else
            return 0;
    }
    else
        return 0;
}


void AssignFldValFunc_1(void *p1, void *p2)
{
    int j = 0;
    int iResult = 0;
    int iArea_no = 0;
    int iCity_no = 0;
    int iPro_no = 0;
    int iFlag_no = 0;
    T_KRRecord  *ptRecord = p1;
    T_KRTable   *ptTable = (T_KRTable *)ptRecord->ptTable;
    T_ErmTradDef_1 *ptReqDef = p2;
    void *pFldVal = NULL;
    
    for (j=0; j<ptTable->iFieldCnt; j++)
    {
        pFldVal = kr_get_field_value(ptRecord, j);
        switch(j)
        {
            case 0:
                *(int *)pFldVal = 1;
                break;
            case 1:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCustNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 2:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutFlatSysDate, ptTable->ptFieldDef[j].length-1);
                break;    
            case 3:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutOrderbytime, 6);
                break;
            case 4:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutFlowNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 5:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutChanlNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 6:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                iFlag_no = j;
                break;
            case 7:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutOldCustGrd, ptTable->ptFieldDef[j].length-1);
                break;
            case 8:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                basRTrim(ptReqDef->caOutChanlTradNo);
                memcpy(pFldVal, ptReqDef->caOutChanlTradNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 9:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutChanlCustNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 10:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCustTyp, ptTable->ptFieldDef[j].length-1);
                break;
            case 11:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCustGrd, ptTable->ptFieldDef[j].length-1);
                break;
            case 12:
                *(int *)pFldVal = ptReqDef->lOutBatchNo;
                break;
            case 13:
                *(double *)pFldVal = ptReqDef->dOutAmt1;
                break;
            case 14:
                *(double *)pFldVal = ptReqDef->dOutAmt3;
                break;
            case 15:
                *(double *)pFldVal = ptReqDef->dOutSvc;
                break;
            case 16:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                basRTrim(ptReqDef->caOutAcctNo);
                memcpy(pFldVal, ptReqDef->caOutAcctNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 17:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutAcctType, ptTable->ptFieldDef[j].length-1);
                break;
            case 18:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutAcctName, ptTable->ptFieldDef[j].length-1);
                break;
            case 19:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutIdAreaNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 20:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignAreaNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 21:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                iArea_no = 21;
                break;
            case 22:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCertTyp, ptTable->ptFieldDef[j].length-1);
                break;
            case 23:
               memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignDate, ptTable->ptFieldDef[j].length-1);
                break;
            case 24:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignType, ptTable->ptFieldDef[j].length-1);
                break;
            case 25:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutTradSts, ptTable->ptFieldDef[j].length-1);
                break;
            case 26:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignBran, ptTable->ptFieldDef[j].length-1);
                break;
            case 27:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutAcctSignTyp, ptTable->ptFieldDef[j].length-1);
                break;
            case 28:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutAssAcctType, ptTable->ptFieldDef[j].length-1);
                break;
            case 29:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutAssAcctName, ptTable->ptFieldDef[j].length-1);
                break;
            case 30:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                basRTrim(ptReqDef->caOutReturnCode);
                memcpy(pFldVal, ptReqDef->caOutReturnCode, ptTable->ptFieldDef[j].length-1);
                break;
            case 31:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                basRTrim(ptReqDef->caOutMercNo);
                memcpy(pFldVal, ptReqDef->caOutMercNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 32:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutFlg1, ptTable->ptFieldDef[j].length-1);
                break;
            case 33:
               memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutAcctBran, ptTable->ptFieldDef[j].length-1);
                break;
            case 34:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutAssAcctBran, ptTable->ptFieldDef[j].length-1);
                break;
            case 35:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutOpenAreaNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 36:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutAssAreaNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 37:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutChanlFlowNo, 1);
                break;
            case 38:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);

                if(strcmp(ptReqDef->caOutChanlNo, "01") == 0 && ptReqDef->dOutLongIp != (double)0)
                {
                    SetBLKIPFlg(ptRecord, ptReqDef->dOutLongIp, j);
                }
                break;
            case 39:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutIdAreaNo, 2);
                break;
            case 40:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignAreaNo, 2);
                break;
            case 41:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                iPro_no = j;
                break;
            case 42:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutIdAreaNo, 4);
                break;
            case 43:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignAreaNo, 4);
                break;
            case 44:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                iCity_no = j;
                break;
            case 45:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                basRTrim(ptReqDef->caOutAssAcctNo);
                memcpy(pFldVal, ptReqDef->caOutAssAcctNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 46:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutChanlFlowNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 47:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                basRTrim(ptReqDef->caOutBackFlowNo);
                memcpy(pFldVal, ptReqDef->caOutBackFlowNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 48:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                if(strcmp(ptReqDef->caOutChanlNo, "01") || strcmp(ptReqDef->caOutChanlNo, "02") )
                {
                    *(int *)pFldVal = CalCustAge(ptReqDef->caOutChanlCustNo);
                }
                break;
            case 49:
                *(int *)pFldVal = strlen(basRTrim(ptReqDef->caOutAssAcctName));
                break;
            default:
                break;
        }
    }
    if(strcmp(ptReqDef->caOutChanlNo, "01") == 0 && ptReqDef->dOutLongIp != (double)0)
    {
        SetAreaInfo(ptRecord, ptReqDef->dOutLongIp, iArea_no, iPro_no, iCity_no, iFlag_no);
    }
}

void AssignFldValFunc_2(void *p1, void *p2)
{
    int j = 0;
    T_KRRecord  *ptRecord = p1;
    T_KRTable   *ptTable = (T_KRTable *)ptRecord->ptTable;
    T_ErmQueryTradDef_2 *ptReqDef = p2;
    void *pFldVal = NULL;
    for (j=0; j<ptTable->iFieldCnt; j++)
    {
        pFldVal = kr_get_field_value(ptRecord, j);
        switch(j)
        {
            case 0:
                *(int *)pFldVal = 2;
                break;
            case 1:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCustNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 2:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutFlatSysDate, ptTable->ptFieldDef[j].length-1);
                break;    
            case 3:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutOrderbytime, 6);
                break;
            case 4:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutFlowNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 5:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutChanlNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 6:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignSts, ptTable->ptFieldDef[j].length-1);
                break;
            case 7:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutTradSts, ptTable->ptFieldDef[j].length-1);
                break;
            case 8:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutChanlCustNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 9:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCustNm, ptTable->ptFieldDef[j].length-1);
                break;
            case 10:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCustTyp, ptTable->ptFieldDef[j].length-1);
                break;
            case 11:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCustGrd, ptTable->ptFieldDef[j].length-1);
                break;
            case 12:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                basRTrim(ptReqDef->caOutAcctNo);
                memcpy(pFldVal, ptReqDef->caOutAcctNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 13:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutAcctTyp, ptTable->ptFieldDef[j].length-1);
                break;
            case 14:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutAcctBran, ptTable->ptFieldDef[j].length-1);
                break;
            case 15:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                basRTrim(ptReqDef->caOutChanlTradNo);  
                memcpy(pFldVal, ptReqDef->caOutChanlTradNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 16:
                *(int *)pFldVal = ptReqDef->lOutBatchNo;
                break;
            case 17:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignBran, ptTable->ptFieldDef[j].length-1);
                break;
            case 18:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                basRTrim(ptReqDef->caOutReturnCode);
                memcpy(pFldVal, ptReqDef->caOutReturnCode, ptTable->ptFieldDef[j].length-1);
                break;
            default:
                break;


        }
    }
}

void AssignFldValFunc_3(void *p1, void *p2)
{
    int j = 0;
    int iResult = 0;
    int iArea_no = 0;
    int iCity_no = 0;
    int iPro_no = 0;
    int iFlag_no = 0;
    T_KRRecord  *ptRecord = p1;
    T_KRTable   *ptTable = (T_KRTable *)ptRecord->ptTable;
    T_ErmLoginTradDef_3 *ptReqDef = p2;
    void *pFldVal = NULL;
    for (j=0; j<ptTable->iFieldCnt; j++)
    {
        pFldVal = kr_get_field_value(ptRecord, j);
        switch(j)
        {
            case 0:
                *(int *)pFldVal = 3;
                break;
            case 1:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCustNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 2:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutFlatSysDate, ptTable->ptFieldDef[j].length-1);
                break;    
            case 3:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutOrderbytime, 6);
                break;
            case 4:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutFlowNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 5:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutChanlNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 6:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                iFlag_no = j;
                break;
            case 7:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutChanlCustNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 8:
                *(int *)pFldVal = ptReqDef->lOutBatchNo;
                break;
            case 9:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCustNm, ptTable->ptFieldDef[j].length-1);
                break;
            case 10:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCustTyp, ptTable->ptFieldDef[j].length-1);
                break;
            case 11:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCustGrd, ptTable->ptFieldDef[j].length-1);
                break;
            case 12:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                basRTrim(ptReqDef->caOutAcctNo);
                memcpy(pFldVal, ptReqDef->caOutAcctNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 13:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutAcctTyp, ptTable->ptFieldDef[j].length-1);
                break;
            case 14:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutAuthType, ptTable->ptFieldDef[j].length-1);
                break;
            case 15:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                basRTrim(ptReqDef->caOutChanlTradNo);  
                memcpy(pFldVal, ptReqDef->caOutChanlTradNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 16:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignSts, ptTable->ptFieldDef[j].length-1);
                break;
            case 17:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutIdAreaNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 18:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignBran, ptTable->ptFieldDef[j].length-1);
                break;
            case 19:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                iArea_no = j;
                break;
            case 20:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCertTyp, ptTable->ptFieldDef[j].length-1);
                break;
            case 21:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutTradSts, ptTable->ptFieldDef[j].length-1);
                break;
            case 22:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignAreaNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 23:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutAcctBran, ptTable->ptFieldDef[j].length-1);
                break;
            case 24:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                basRTrim(ptReqDef->caOutReturnCode);
                memcpy(pFldVal, ptReqDef->caOutReturnCode, ptTable->ptFieldDef[j].length-1);
                break;
            case 25:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutAddInf, ptTable->ptFieldDef[j].length-1);
                break;
            case 26:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutFlg1, ptTable->ptFieldDef[j].length-1);
                break;
            case 27:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutTradBran, ptTable->ptFieldDef[j].length-1);
                break;
            case 28:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);

                if(strcmp(ptReqDef->caOutChanlNo, "01") == 0 && ptReqDef->dOutLongIp != (double)0)
                {
                    SetBLKIPFlg(ptRecord, ptReqDef->dOutLongIp, j);
                }

                break;
            case 29:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutIdAreaNo, 2);
                break;
            case 30:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                iPro_no = j;
                break;
            case 31:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignAreaNo, 2);
                break;
            case 32:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutIdAreaNo, 4);
                break;
            case 33:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                iCity_no = j;
                break;
            case 34:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignAreaNo, 4);
                break;
            case 35:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                if(strcmp(ptReqDef->caOutChanlNo, "01") || strcmp(ptReqDef->caOutChanlNo, "02") )
                {
                    *(int *)pFldVal = CalCustAge(ptReqDef->caOutChanlCustNo);
                }
            default:
                break;
        }
    }
    if(strcmp(ptReqDef->caOutChanlNo, "01") == 0 && ptReqDef->dOutLongIp != (double)0)
    {
        SetAreaInfo(ptRecord, ptReqDef->dOutLongIp, iArea_no, iPro_no, iCity_no, iFlag_no);
    }
}

void AssignFldValFunc_4(void *p1, void *p2)
{
    int j = 0;
    int iArea_no = 0;
    int iCity_no = 0;
    int iPro_no = 0;
    int iFlag_no = 0;
    T_KRRecord  *ptRecord = p1;
    T_KRTable   *ptTable = (T_KRTable *)ptRecord->ptTable;
    T_ErmSynMainDef_4 *ptReqDef = p2;
    void *pFldVal = NULL;
    for (j=0; j<ptTable->iFieldCnt; j++)
    {
        pFldVal = kr_get_field_value(ptRecord, j);
        switch(j)
        {
            case 0:
                *(int *)pFldVal = 4;
                break;
            case 1:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCustNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 2:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignDate, ptTable->ptFieldDef[j].length-1);
                break;    
            case 3:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutOrderbytime, ptTable->ptFieldDef[j].length-1);
                break;
            case 4:
                *(int *)pFldVal = ptReqDef->lOutBatchNo;
                break;
            case 5:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutFlowNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 6:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutChanlNo, 2);
                break;
            case 7:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutChanlCustNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 8:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                iFlag_no = j;
                break;
            case 9:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                iArea_no = j;
                break;
            case 10:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCertTyp, ptTable->ptFieldDef[j].length-1);
                break;
            case 11:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutMblAreaNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 12:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutIdAreaNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 13:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignBran, ptTable->ptFieldDef[j].length-1);
                break;
            case 14:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutTermChanl, ptTable->ptFieldDef[j].length-1);
                break;
            case 15:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignAreaNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 16:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutTradSts, ptTable->ptFieldDef[j].length-1);
                break;
            case 17:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutPacctFscrdFlg, ptTable->ptFieldDef[j].length-1);
                break;
            case 18:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutPchanlFuncCode, ptTable->ptFieldDef[j].length-1);
                break;
            case 19:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutPacctAcctTyp, ptTable->ptFieldDef[j].length-1);
                break;
            case 20:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutChgUkeyFlg, ptTable->ptFieldDef[j].length-1);
                break;
            case 21:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutChgTpwdFlg, ptTable->ptFieldDef[j].length-1);
                break;
            case 22:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutChgQpwdFlg, ptTable->ptFieldDef[j].length-1);
                break;
            case 23:
               memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutChgNmblFlg, ptTable->ptFieldDef[j].length-1);
                break;
            case 24:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutPchanlSignSts, ptTable->ptFieldDef[j].length-1);
                break;
            case 25:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutPacctAcctNm, ptTable->ptFieldDef[j].length-1);
                break;
            case 26:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutPchanlAuthType, ptTable->ptFieldDef[j].length-1);
                break;
            case 27:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutPacctFuncCode, ptTable->ptFieldDef[j].length-1);
                break;
            case 28:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutPacctAcctNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 29:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutPacctCcbAcctFlg, ptTable->ptFieldDef[j].length-1);
                break; 
            case 30:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                basRTrim(ptReqDef->caOutChanlTradNo);
                memcpy(pFldVal, ptReqDef->caOutChanlTradNo, ptTable->ptFieldDef[j].length-1);
                break; 
            case 31:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutTradAreaNo, ptTable->ptFieldDef[j].length-1);
                break; 
            case 32:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutTradBran, ptTable->ptFieldDef[j].length-1);
                break; 
            case 33:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                iPro_no = j;
                break;
            case 34:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutMblAreaNo, 2);
                break;
            case 35:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutIdAreaNo, 2);
                break;
            case 36:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignAreaNo, 2);
                break;
            case 37:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                iCity_no = j;
                break;
            case 38:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutMblAreaNo, 4);
                break;
            case 39:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutIdAreaNo, 4);
                break;
            case 40:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutSignAreaNo, 4);
                break;
            case 41:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutPchanlDn, ptTable->ptFieldDef[j].length-1);
                break;
            case 42:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutTradAreaNo, 2);
                break;
            case 43:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutTradAreaNo, 4);
                break;
            default:
                break;
        }
    }
    if(strcmp(ptReqDef->caOutTermChanl, "01") == 0 && ptReqDef->dOutLongIp != (double)0)
    {
        SetAreaInfo(ptRecord, ptReqDef->dOutLongIp, iArea_no, iPro_no, iCity_no, iFlag_no);
    }
}
