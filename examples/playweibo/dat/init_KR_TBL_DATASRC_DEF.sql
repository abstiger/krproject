begin;

DELETE from KR_TBL_DATASRC_DEF;

insert into KR_TBL_DATASRC_DEF (DATASRC_ID, DATASRC_NAME, DATASRC_DESC, MAP_FUNC_PRE, MAP_FUNC, MAP_FUNC_POST, DATASRC_USAGE, MMAP_FILE_NAME, SIZE_KEEP_MODE, SIZE_KEEP_VALUE)
values (2, 'weibo statuses', 'krproject weibo status', 'weibo_map_func_pre', 'weibo_map_func', 'weibo_map_func_post', '1', 'weibo', '0', 1000);

commit;
