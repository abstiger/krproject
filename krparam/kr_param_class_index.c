#include "kr_param_class_index.h"
#include "krutils/kr_utils.h"

static cJSON *kr_param_index_table_serialize(T_KRParamIndexTable *ptParamIndexTable)
{
    cJSON *index = cJSON_CreateObject();
    cJSON_AddNumberToObject(index, "id", ptParamIndexTable->lTableId);
    cJSON_AddNumberToObject(index, "index_field_id", ptParamIndexTable->lIndexFieldId);
    cJSON_AddNumberToObject(index, "sort_field_id", ptParamIndexTable->lSortFieldId);
    return index;
}

char *kr_param_index_serialize(T_KRParamIndex *ptParamIndex)
{
    cJSON *index = cJSON_CreateObject();
    cJSON_AddNumberToObject(index, "id", ptParamIndex->lIndexId);
    cJSON_AddStringToObject(index, "name", ptParamIndex->caIndexName);
    cJSON_AddStringToObject(index, "desc", ptParamIndex->caIndexDesc);
    cJSON_AddStringToObject(index, "field_type", ptParamIndex->caIndexFieldType);
    cJSON *tables = cJSON_CreateArray();
    for (int i=0; i<ptParamIndex->lTableCnt; ++i) {
        cJSON *table = kr_param_index_table_serialize(&ptParamIndex->ptTableDef[i]);
        cJSON_AddItemToArray(tables, table);
    }
    cJSON_AddItemToObject(index, "tables", tables);
    
    char *str = cJSON_Print(index);
    cJSON_Delete(index);
    return str;
}

void kr_param_index_serialize_free(char *ptParamIndexString)
{
    if (ptParamIndexString) {
        kr_free(ptParamIndexString);
    }
}


static void kr_param_index_table_deserialize(cJSON *table, T_KRParamIndexTable *ptParamIndexTable)
{
    ptParamIndexTable->lTableId = cJSON_GetNumber(table, "id");
    ptParamIndexTable->lIndexFieldId = cJSON_GetNumber(table, "index_field_id");
    ptParamIndexTable->lSortFieldId = cJSON_GetNumber(table, "sort_field_id");
}


T_KRParamIndex *kr_param_index_deserialize(char *ptParamIndexString)
{
    T_KRParamIndex *ptParamIndex = kr_calloc(sizeof(*ptParamIndex));
    
    cJSON *index=cJSON_Parse(ptParamIndexString);
    ptParamIndex->lIndexId = cJSON_GetNumber(index, "id");
    strcpy(ptParamIndex->caIndexName, cJSON_GetString(index, "name"));
    strcpy(ptParamIndex->caIndexDesc, cJSON_GetString(index, "desc"));
    strcpy(ptParamIndex->caIndexFieldType, cJSON_GetString(index, "field_type"));
    cJSON *tables = cJSON_GetObjectItem(index, "tables");
    ptParamIndex->lTableCnt = cJSON_GetArraySize(tables);
    ptParamIndex->ptTableDef = kr_calloc(sizeof(T_KRParamIndexTable)*ptParamIndex->lTableCnt);
    for (int i=0; i<ptParamIndex->lTableCnt; ++i) {
        cJSON *table = cJSON_GetArrayItem(tables, i);
        kr_param_index_table_deserialize(table, &ptParamIndex->ptTableDef[i]);
    }
    
    return ptParamIndex;
}


void kr_param_index_deserialize_free(T_KRParamIndex *ptParamIndex)
{
    if (ptParamIndex) {
        if (ptParamIndex->ptTableDef) {
            kr_free(ptParamIndex->ptTableDef);
        }
        kr_free(ptParamIndex);
    }
}

