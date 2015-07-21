#ifndef __KR_OUTPUT_DEFINE_H__
#define __KR_OUTPUT_DEFINE_H__

#include "krutils/kr_utils.h"
#include "krcalc/kr_calc.h"
#include "krparam/kr_param_class_output.h"
#include "kr_output_handle.h"

/*output field define*/
typedef struct _kr_output_field_def_t
{
    int            id;
    char           name[30+1];
    char           desc[100+1];
    E_KRType       type;
    int            length;
    size_t         offset;
    T_KRCalc       *calc;
}T_KROutputFieldDef;


typedef struct _kr_output_define_t
{
    int                iOutputId;
    char               caOutputName[30+1];
    char               caOutputDesc[100+1];
    int                iFieldCnt;         /* field count of this table */
    T_KROutputFieldDef *ptFieldDef;       /* field define of this table */
    int                iRecordSize;       /* record size needed to allocated */
    
    T_KRList           *ptOutputHandleList;
}T_KROutputDefine;


T_KROutputDefine *kr_output_define_new(T_KRParamOutput *ptParamOutput, 
        KRGetTypeFunc pfGetType, KRGetValueFunc pfGetValue);
void kr_output_define_free(T_KROutputDefine *ptOutputDefine);
int kr_output_define_match(T_KROutputDefine *ptOutputDefine, int *piOutputId);
T_KROutputHandle* kr_output_define_get_handle(T_KROutputDefine *ptOutputDefine, char *psFormat, T_KRModule *ptModule);

#endif /* __KR_OUTPUT_DEFINE_H__ */
