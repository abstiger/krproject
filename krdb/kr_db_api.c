#include "kr_db_api.h"

cJSON *kr_db_info(T_KRDB *krdb)
{
	cJSON *db = cJSON_CreateObject();
	cJSON_AddStringToObject(db, "name", krdb->caDBName);
	cJSON_AddNumberToObject(db, "table_number", 
			kr_list_length(krdb->pTableList));
	cJSON_AddNumberToObject(db, "index_number", 
			kr_list_length(krdb->pIndexList));
	return db;
}

cJSON *kr_db_table_info(T_KRTable *krtable)
{
	cJSON *table = cJSON_CreateObject();
	cJSON_AddNumberToObject(table, "id", krtable->iTableId);
	cJSON_AddStringToObject(table, "name", krtable->caTableName);
	cJSON_AddNumberToObject(table, "size_keep_mode", krtable->eSizeKeepMode);
	cJSON_AddNumberToObject(table, "size_keep_value", krtable->lSizeKeepValue);
	cJSON_AddNumberToObject(table, "field_count", krtable->iFieldCnt);
	cJSON_AddNumberToObject(table, "record_size", krtable->iRecordSize);
	cJSON_AddNumberToObject(table, "record_number", krtable->uiRecordNum);
	cJSON_AddNumberToObject(table, "record_location", krtable->uiRecordLoc);

	cJSON *fields = cJSON_CreateArray();
	T_KRFieldDef *ptFieldDef = &krtable->ptFieldDef[0];
	for (int i=0; i<krtable->iFieldCnt; i++, ptFieldDef++) {
		cJSON *field = cJSON_CreateObject();
		cJSON_AddNumberToObject(field, "id", ptFieldDef->id);
		cJSON_AddStringToObject(field, "name", ptFieldDef->name);
		cJSON_AddNumberToObject(field, "type", ptFieldDef->type);
		cJSON_AddNumberToObject(field, "length", ptFieldDef->length);
		cJSON_AddItemToArray(fields, field);
	}
	cJSON_AddItemToObject(table, "fields", fields);
	return table;
}

cJSON *kr_db_index_info(T_KRIndex *krindex)
{
	cJSON *index = cJSON_CreateObject();
	cJSON_AddNumberToObject(index, "id", krindex->iIndexId);
	cJSON_AddStringToObject(index, "name", krindex->caIndexName);
	cJSON_AddNumberToObject(index, "type", krindex->eIndexType);
	cJSON_AddNumberToObject(index, "index_field_id", krindex->iIndexFieldId);
	cJSON_AddNumberToObject(index, "sort_field_id", krindex->iSortFieldId);
	return index;
}

cJSON *kr_db_record_info(T_KRRecord *krrecord)
{
	cJSON *record = cJSON_CreateObject();
	T_KRTable  *ptTable = (T_KRTable *)krrecord->ptTable;
	cJSON_AddNumberToObject(record, "field_count", ptTable->iFieldCnt);
	cJSON *fields = cJSON_CreateArray();
	for (int i=0; i<ptTable->iFieldCnt; i++) {
		cJSON *field = cJSON_CreateObject();
		cJSON_AddNumberToObject(field, "id", ptTable->ptFieldDef[i].id);
		cJSON_AddStringToObject(field, "name", ptTable->ptFieldDef[i].name);
		void *pFieldVal = kr_get_field_value(krrecord, i);
		switch(ptTable->ptFieldDef[i].type)
		{
			case KR_TYPE_INT:
				cJSON_AddNumberToObject(field, "value", *(int *)pFieldVal);
				break;
			case KR_TYPE_LONG:
				cJSON_AddNumberToObject(field, "value", *(long *)pFieldVal);
				break;
			case KR_TYPE_DOUBLE:
				cJSON_AddNumberToObject(field, "value", *(double *)pFieldVal);
				break;
			case KR_TYPE_STRING:
				cJSON_AddStringToObject(field, "value", (char *)pFieldVal);
				break;
			default:
				break;           
		}
		cJSON_AddItemToArray(fields, field);
	}
	cJSON_AddItemToObject(record, "fields", fields);
	return record;
}
