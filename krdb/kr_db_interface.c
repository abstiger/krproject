#include "krutils/kr_utils.h"
#include "kr_db_interface.h"
#include "kr_db_kernel.h"
#include <time.h>

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
        default:
            return NULL;
            break;
    }
}

void AssignFldValFunc_1(void *p1, void *p2)
{
    int j = 0;
    T_KRRecord  *ptRecord = p1;
    T_KRTable   *ptTable = (T_KRTable *)ptRecord->ptTable;
    T_KRTradFlow_1 *ptReqDef = p2;
    void *pFldVal = NULL;
    char caTransDateTime[14+1] = {0};
    
    for (j=0; j<ptTable->iFieldCnt; j++)
    {
        pFldVal = kr_get_field_value(ptRecord, j);
        switch(j)
        {
            case 0:
                *(long *)pFldVal = (long )time(NULL);
                break;
            case 1:
                memset(caTransDateTime, 0x00, sizeof(caTransDateTime));
                snprintf(caTransDateTime, 15, "%8s%6s", ptReqDef->caOutTransDate, ptReqDef->caOutTransTime);
                *(long *)pFldVal = kr_time_to_ttime(caTransDateTime);
                break;
            case 2:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutCustNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 3:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutTransDate, ptTable->ptFieldDef[j].length-1);
                break;    
            case 4:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutTransTime, ptTable->ptFieldDef[j].length-1);
                break;
            case 5:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutFlowNo, ptTable->ptFieldDef[j].length-1);
                break;
            case 6:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutTransType, ptTable->ptFieldDef[j].length-1);
                break;
            case 7:
                *(double *)pFldVal = ptReqDef->dOutTransAmt;
                break;
            case 8:
                memset(pFldVal, 0x00, ptTable->ptFieldDef[j].length);
                memcpy(pFldVal, ptReqDef->caOutTransLoc, ptTable->ptFieldDef[j].length-1);
                break;
            default:
                break;
        }
    }
}