#ifndef __KR_INPUT_H__
#define __KR_INPUT_H__

#include "krutils/kr_utils.h"
#include "krparam/kr_param.h"
#include "krengine/kr_engine.h"

/*the two public fieldno of ptDB'field definition*/
typedef enum {
    KR_FIELDID_PROCTIME    = 0,   /*timestamp of processing*/
    KR_FIELDID_TRANSTIME   = 1    /*timestamp of transaction*/
}E_KRPublicFieldId;


//struct forward declaration
typedef struct _kr_input_t T_KRInput;
typedef struct _kr_record_t T_KRRecord;


//input functions
extern T_KRInput* kr_input_construct(T_KRParam *ptParam, char *psInputModule);
extern void kr_input_destruct(T_KRInput* ptInput);
extern int kr_input_check(T_KRInput* ptInput, T_KRParam *ptParam);
extern T_KRRecord *kr_input_process(T_KRInput *ptInput, T_KRMessage *ptMessage);

//record functions
extern T_KRRecord* kr_record_new(T_KRInput *ptInput, int iInputId);
extern void kr_record_free(T_KRRecord *ptRecord);
extern T_KRRecord* kr_record_dup(T_KRRecord *ptRecord);
extern int kr_record_compare(T_KRRecord *ptRec1, T_KRRecord *ptRec2, int iFieldId);

extern char* kr_record_get_field_name(T_KRRecord *ptRecord, int iFieldId);
extern int kr_record_get_field_length(T_KRRecord *ptRecord, int iFieldId);
extern E_KRType kr_record_get_field_type(T_KRRecord *ptRecord, int iFieldId);
extern void* kr_record_get_field_value(T_KRRecord *ptRecord, int iFieldId);
extern time_t kr_record_get_proctime(T_KRRecord *ptRecord);
extern time_t kr_record_get_transtime(T_KRRecord *ptRecord);
extern int kr_record_get_input_id(T_KRRecord *ptRecord);
extern int kr_record_get_field_cnt(T_KRRecord *ptRecord);


#endif /* __KR_INPUT_H__ */
