#include "kr_record.h"
#include "kr_input.h"
#include "kr_input_define.h"

/*record definition*/
struct _kr_record_t
{
    KRFreeFunc         pfFree;
    T_KRInputDefine    *ptInputDefine;
    char               *pRecBuf;
};

extern T_KRInputDefine* kr_input_get_define(T_KRInput *ptInput, int iInputId);

T_KRRecord* kr_record_new(T_KRInput *ptInput, int iInputId)
{
    T_KRInputDefine *ptInputDefine = kr_input_get_define(ptInput, iInputId);
    if (ptInputDefine == NULL) {
        KR_LOG(KR_LOGERROR, "kr_input_get_define [%d] failed!", iInputId);
        return NULL;
    }
    
    T_KRRecord *ptRecord = kr_calloc(sizeof(*ptRecord)+ptInputDefine->iRecordSize);
    if (ptRecord == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptRecord failed!");
        return NULL;
    }
    
    /*create and set new record*/
    ptRecord->pfFree = NULL;
    ptRecord->ptInputDefine = ptInputDefine;
    ptRecord->pRecBuf = (char *)ptRecord+sizeof(*ptRecord);

    return ptRecord;
}


void kr_record_free(T_KRRecord *ptRecord)
{
    if (ptRecord) {
        if (ptRecord->pfFree) {
            ptRecord->pfFree(ptRecord);
        }
        kr_free(ptRecord);
    }
}


T_KRRecord* kr_record_dup(T_KRRecord *ptRecord)
{
    T_KRRecord *ptRecordDup = kr_calloc(sizeof(*ptRecord)+ptRecord->ptInputDefine->iRecordSize);
    if (ptRecord == NULL) {
        KR_LOG(KR_LOGERROR, "kr_calloc ptRecordDup failed!");
        return NULL;
    }
    
    /*create and set new record*/
    memcpy(ptRecordDup, ptRecord, sizeof(*ptRecord)+ptRecord->ptInputDefine->iRecordSize);
    ptRecordDup->pRecBuf = (char *)ptRecordDup+sizeof(*ptRecordDup);
    
    return ptRecordDup;
}


int kr_record_compare(T_KRRecord *ptRec1, T_KRRecord *ptRec2, int iFieldId)
{
    /*compare field type*/
    E_KRType eType1 = kr_record_get_field_type(ptRec1, iFieldId);
    E_KRType eType2 = kr_record_get_field_type(ptRec1, iFieldId);
    if (eType1 != eType2) return 0;
    
    /*get value compare function*/
    KRCompareFunc pfCompare = kr_get_compare_func(eType1);
    if (pfCompare == NULL) return 0;

    /*compare field value*/
    void *pVal1 = kr_record_get_field_value(ptRec1, iFieldId);
    void *pVal2 = kr_record_get_field_value(ptRec2, iFieldId);

    return pfCompare(pVal1, pVal2);
}


char* kr_record_get_field_name(T_KRRecord *ptRecord, int iFieldId)
{ 
    return ptRecord->ptInputDefine->ptFieldDef[iFieldId].name;
}


int kr_record_get_field_length(T_KRRecord *ptRecord, int iFieldId)
{ 
    return ptRecord->ptInputDefine->ptFieldDef[iFieldId].length;
}


E_KRType kr_record_get_field_type(T_KRRecord *ptRecord, int iFieldId)
{ 
    return ptRecord->ptInputDefine->ptFieldDef[iFieldId].type;
}


void* kr_record_get_field_value(T_KRRecord *ptRecord, int iFieldId)
{
    size_t offset = ptRecord->ptInputDefine->ptFieldDef[iFieldId].offset;
    return &ptRecord->pRecBuf[offset];
}


time_t kr_record_get_proctime(T_KRRecord *ptRecord)
{
    time_t tProcTime;
    tProcTime = (time_t )*(long *)kr_record_get_field_value(\
            ptRecord, KR_FIELDID_PROCTIME);
    return tProcTime;
}


time_t kr_record_get_transtime(T_KRRecord *ptRecord)
{
    time_t tTransTime;
    tTransTime = (time_t )*(long *)kr_record_get_field_value(\
            ptRecord, KR_FIELDID_TRANSTIME);
    return tTransTime;
}


int kr_record_get_input_id(T_KRRecord *ptRecord)
{
    return ptRecord->ptInputDefine->iInputId;
}


int kr_record_get_field_cnt(T_KRRecord *ptRecord)
{
    return ptRecord->ptInputDefine->iFieldCnt;
}

