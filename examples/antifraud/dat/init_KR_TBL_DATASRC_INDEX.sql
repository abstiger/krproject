begin;

DELETE from KR_TBL_DATASRC_INDEX;

insert into KR_TBL_DATASRC_INDEX (DATASRC_ID, INDEX_ID, INDEX_NAME, INDEX_DESC, INDEX_TYPE, INDEX_FIELD_ID, SORT_FIELD_ID) values (1, 1, 'custno_index', '客户统计维度', 'S', 2, 0);

commit;
