#ifndef __KR_PARAM_INDEX_H__
#define __KR_PARAM_INDEX_H__

#define KR_PARAM_INDEX "index"


typedef struct _kr_param_index_table_t
{
    long          lTableId;
    long          lIndexFieldId;
    long          lSortFieldId;
}T_KRParamIndexTable;

typedef struct _kr_param_index_t
{
    long                     lIndexId;
    char                     caIndexName[30+1];
    char                     caIndexDesc[100+1];
    char                     caIndexFieldType[1+1];
    
    long                     lTableCnt;         
    T_KRParamIndexTable      *ptTableDef;
}T_KRParamIndex;



char *kr_param_index_serialize(T_KRParamIndex *ptParamIndex);
void kr_param_index_serialize_free(char *ptParamIndexString);

T_KRParamIndex *kr_param_index_deserialize(char *ptParamIndexString);
void kr_param_index_deserialize_free(T_KRParamIndex *ptParamIndex);


#endif  /*__KR_PARAM_INDEX_H__*/
