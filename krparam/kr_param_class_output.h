#ifndef __KR_PARAM_OUTPUT_H__
#define __KR_PARAM_OUTPUT_H__

#define KR_PARAM_OUTPUT "output"


/*field description*/
typedef struct _kr_param_output_field_t
{
    long          lFieldId;
    char          caFieldName[30+1];
    char          caFieldDesc[100+1];
    char          caFieldType[1+1];
    long          lFieldLength;
    char          caCalcFormat[1+1];
    char          caCalcString[500+1];
}T_KRParamOutputField;

typedef struct _kr_param_output_t
{
    long                     lOutputId;
    char                     caOutputName[30+1];
    char                     caOutputDesc[100+1];
    long                     lFieldCnt;
    T_KRParamOutputField     *ptFieldDef;
}T_KRParamOutput;



char *kr_param_output_serialize(T_KRParamOutput *ptParamOutput);
void kr_param_output_serialize_free(char *ptParamOutputString);

T_KRParamOutput *kr_param_output_deserialize(char *ptParamOutputString);
void kr_param_output_deserialize_free(T_KRParamOutput *ptParamOutput);


#endif  /*__KR_PARAM_OUTPUT_H__*/
