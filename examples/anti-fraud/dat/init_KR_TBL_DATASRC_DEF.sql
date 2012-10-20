DELETE from KR_TBL_DATASRC_DEF;

insert into KR_TBL_DATASRC_DEF (DATASRC_ID, DATASRC_NAME, DATASRC_DESC, DATASRC_MAP_FUNC, DATASRC_USAGE, MMAP_FILE_NAME, SIZE_KEEP_MODE, SIZE_KEEP_VALUE)
values (1, 'KR_Trans_Flow', 'krproject trans flow', 'map_func_1', '1', 'kr_trans_flow_mmap.db', '0', 20);

commit;
