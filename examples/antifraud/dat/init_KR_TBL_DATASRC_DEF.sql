begin;

DELETE from KR_TBL_DATASRC_DEF;

insert into KR_TBL_DATASRC_DEF (DATASRC_ID, DATASRC_NAME, DATASRC_DESC, MAP_FUNC_PRE, MAP_FUNC, MAP_FUNC_POST, DATASRC_USAGE, MMAP_FILE_NAME, SIZE_KEEP_MODE, SIZE_KEEP_VALUE)
values (1, 'KR_Trans_Flow', 'antifraud trans flow', ' ', 'map_func_1', ' ', '1', 'antifraud', '0', 20);

commit;
