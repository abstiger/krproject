begin;

DELETE from KR_TBL_DATASRC_INDEX_DEF;

insert into KR_TBL_DATASRC_INDEX_DEF (DATASRC_ID, INDEX_ID, INDEX_NAME, INDEX_DESC, INDEX_TYPE, INDEX_FIELD_ID, SORT_FIELD_ID)
values (2, 1, 'user_id_index', '用户Id维度', 'S', 2, 0);

commit;
