#ifndef __KR_INPUT_DEFINE_H__
#define __KR_INPUT_DEFINE_H__

#include "krparam/kr_param_class_input.h"
#include "kr_input_handle.h"


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

/*input field define*/
typedef struct _kr_input_field_def_t
{
    int            id;
    char           name[30+1];
    char           desc[100+1];
    E_KRType       type;
    int            length;
    size_t         offset;
    T_KRDict       *dict;
}T_KRInputFieldDef;


typedef struct _kr_input_def_t
{
    int                iInputId;
    char               caInputName[30+1];
    char               caInputDesc[100+1];
    int                iFieldCnt;         /* field count of this table */
    T_KRInputFieldDef  *ptFieldDef;       /* field define of this table */
    
    T_KRList           *ptInputHandleList;
}T_KRInputDefine;


T_KRInputDefine *kr_input_define_new(T_KRParamInput *ptParamInput);
void kr_input_define_free(T_KRInputDefine *ptInputDefine);
int kr_input_define_match(T_KRInputDefine *ptInputDefine, int *piInputId);

#endif /* __KR_INPUT_DEFINE_H__ */
