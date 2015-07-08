#ifndef __KR_PARAM_TABLE_H__
#define __KR_PARAM_TABLE_H__

#define KR_PARAM_TABLE "table"

typedef struct _kr_param_table_t
{
    long                     lTableId;
    char                     caTableName[30+1];
    char                     caTableDesc[100+1];
    char                     caKeepMode[1+1];
    long                     lKeepValue;
}T_KRParamTable;



char *kr_param_table_serialize(T_KRParamTable *ptParamTable);
void kr_param_table_serialize_free(char *ptParamTableString);

T_KRParamTable *kr_param_table_deserialize(char *ptParamTableString);
void kr_param_table_deserialize_free(T_KRParamTable *ptParamTable);


#endif  /*__KR_PARAM_TABLE_H__*/
