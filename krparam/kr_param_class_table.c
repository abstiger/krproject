#include "kr_param_class_table.h"
#include "krutils/kr_utils.h"


char *kr_param_table_serialize(T_KRParamTable *ptParamTable)
{
    cJSON *table = cJSON_CreateObject();
    cJSON_AddNumberToObject(table, "id", ptParamTable->lTableId);
    cJSON_AddStringToObject(table, "name", ptParamTable->caTableName);
    cJSON_AddStringToObject(table, "desc", ptParamTable->caTableDesc);
    cJSON_AddStringToObject(table, "keep_mode", ptParamTable->caKeepMode);
    cJSON_AddNumberToObject(table, "keep_value", ptParamTable->lKeepValue);
    
    char *str = cJSON_Print(table);
    cJSON_Delete(table);
    return str;
}


void kr_param_table_serialize_free(char *ptParamTableString)
{
    if (ptParamTableString) {
        kr_free(ptParamTableString);
    }
}


T_KRParamTable *kr_param_table_deserialize(char *ptParamTableString)
{
    T_KRParamTable *ptParamTable = kr_calloc(sizeof(*ptParamTable));
    
    cJSON *table=cJSON_Parse(ptParamTableString);
    ptParamTable->lTableId = cJSON_GetNumber(table, "id");
    strcpy(ptParamTable->caTableName, cJSON_GetString(table, "name"));
    strcpy(ptParamTable->caTableDesc, cJSON_GetString(table, "desc"));
    strcpy(ptParamTable->caKeepMode, cJSON_GetString(table, "keep_mode"));
    ptParamTable->lKeepValue = cJSON_GetNumber(table, "keep_value");
        
    return ptParamTable;
}


void kr_param_table_deserialize_free(T_KRParamTable *ptParamTable)
{
    if (ptParamTable) {
        kr_free(ptParamTable);
    }
}

