#ifndef __KR_OUTPUT_DEFINE_H__
#define __KR_OUTPUT_DEFINE_H__

#include "kr_output.h"
#include "krparam/kr_param_class_output.h"


typedef struct _kr_dict_value_t
{
    char           key[30+1];
    char           value[200+1];
}T_KRDictValue;

/*dictionary define*/
typedef struct _kr_dict_t
{
    int            id;
    char           name[30+1];
    char           desc[100+1];
    E_KRType       type;
    T_KRList       *list;
}T_KRDict;

/*output field define*/
typedef struct _kr_output_field_def_t
{
    int            id;
    char           name[30+1];
    char           desc[100+1];
    E_KRType       type;
    int            length;
    size_t         offset;
    T_KRDict       *dict;
}T_KROutputFieldDef;


struct _kr_output_define_t
{
    int                iOutputId;
    char               caOutputName[30+1];
    char               caOutputDesc[100+1];
    int                iFieldCnt;         /* field count of this table */
    T_KROutputFieldDef  *ptFieldDef;       /* field define of this table */
    int                iRecordSize;       /* record size needed to allocated */
    
    T_KRList           *ptOutputHandleList;
};


T_KROutputDefine *kr_output_define_new(T_KRParamOutput *ptParamOutput);
void kr_output_define_free(T_KROutputDefine *ptOutputDefine);
int kr_output_define_match(T_KROutputDefine *ptOutputDefine, int *piOutputId);
T_KROutputHandle* kr_output_define_get_handle(T_KROutputDefine *ptOutputDefine, char *psFormat, T_KRModule *ptModule);

#endif /* __KR_OUTPUT_DEFINE_H__ */
