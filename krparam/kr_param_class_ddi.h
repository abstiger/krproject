#ifndef __KR_PARAM_DDI_H__
#define __KR_PARAM_DDI_H__

#define KR_PARAM_DDI "ddi"

typedef struct _kr_param_ddi_t
{
    long        lDdiId;
    char        caDdiName[30+1];
    char        caDdiDesc[100+1];
    char        caDdiType[1+1];
    char        caDdiValueType[1+1];
    char        caDdiAggrFunc[50+1];
    char        caDdiFreeFunc[50+1];
    long        lStatisticsDatasrc;
    long        lStatisticsIndex;
    long        lStatisticsField;
    char        caStatisticsType[1+1];
    long        lStatisticsValue;
    long        lStatisticsCount;
    char        caStatisticsMethod[1+1];
    char        caDdiFilterFormat[1+1];
    char        caDdiFilterString[500+1];
}T_KRParamDdi;

char *kr_param_ddi_serialize(T_KRParamDdi *ptParamDdi);
void kr_param_ddi_serialize_free(char *ptParamDdiString);

T_KRParamDdi *kr_param_ddi_deserialize(char *ptParamDdiString);
void kr_param_ddi_deserialize_free(T_KRParamDdi *ptParamDdi);



#endif  /*__KR_PARAM_DDI_H__*/
