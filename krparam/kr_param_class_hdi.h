#ifndef __KR_PARAM_HDI_H__
#define __KR_PARAM_HDI_H__

#define KR_PARAM_HDI "hdi"
 
typedef struct _kr_param_hdi_def_t
{
    long        lHdiId;
    char        caHdiName[30+1];
    char        caHdiDesc[100+1];
    char        caHdiType[1+1];
    char        caHdiValueType[1+1];
    char        caHdiAggrFunc[50+1];
    char        caHdiFreeFunc[50+1];
    long        lStatisticsDatasrc;
    long        lStatisticsIndex;
    char        caStatisticsType[1+1];
    long        lStatisticsValue;
    char        caStatisticsMethod[1+1];
}T_KRParamHdi;

char *kr_param_hdi_serialize(T_KRParamHdi *ptParamHdi);
void kr_param_hdi_serialize_free(char *ptParamHdiString);

T_KRParamHdi *kr_param_hdi_deserialize(char *ptParamHdiString);
void kr_param_hdi_deserialize_free(T_KRParamHdi *ptParamHdi);



#endif  /*__KR_PARAM_HDI_H__*/
