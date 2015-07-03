#ifndef __KR_PARAM_INPUT_H__
#define __KR_PARAM_INPUT_H__

#define KR_PARAM_INPUT "input"


/*field description*/
typedef struct _kr_param_input_field_t
{
    long          lFieldId;
    char          caFieldName[30+1];
    char          caFieldDesc[100+1];
    char          caFieldType[1+1];
    long          lFieldLength;
}T_KRParamInputField;

typedef struct _kr_param_input_t
{
    long                     lInputId;
    char                     caInputName[30+1];
    char                     caInputDesc[100+1];
    char                     caKeepMode[1+1];
    long                     lKeepValue;
    long                     lFieldCnt;         
    T_KRParamInputField      *ptFieldDef;
}T_KRParamInput;



char *kr_param_input_serialize(T_KRParamInput *ptParamInput);
void kr_param_input_serialize_free(char *ptParamInputString);

T_KRParamInput *kr_param_input_deserialize(char *ptParamInputString);
void kr_param_input_deserialize_free(T_KRParamInput *ptParamInput);


#endif  /*__KR_PARAM_INPUT_H__*/
