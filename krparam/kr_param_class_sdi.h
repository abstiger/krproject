#ifndef __KR_PARAM_SDI_H__
#define __KR_PARAM_SDI_H__

#define KR_PARAM_SDI "sdi"


typedef struct _kr_param_sdi_t
{
    long        lSdiId;
    char        caSdiName[30+1];
    char        caSdiDesc[100+1];
    char        caSdiType[1+1];
    char        caSdiValueType[1+1];
    char        caSdiAggrFunc[50+1];
    char        caSdiFreeFunc[50+1];
    long        lStatisticsDatasrc;
    long        lStatisticsIndex;
    long        lStatisticsField;
    long        lStatisticsLocation;
    char        caLocationProperty[1+1];
    char        caSdiFilterFormat[1+1];
    char        caSdiFilterString[500+1];
}T_KRParamSdi;

char *kr_param_sdi_serialize(T_KRParamSdi *ptParamSdi);
void kr_param_sdi_serialize_free(char *ptParamSdiString);

T_KRParamSdi *kr_param_sdi_deserialize(char *ptParamSdiString);
void kr_param_sdi_deserialize_free(T_KRParamSdi *ptParamSdi);



#endif  /*__KR_PARAM_SDI_H__*/
