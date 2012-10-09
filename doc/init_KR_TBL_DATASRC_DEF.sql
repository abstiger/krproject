WHENEVER SQLERROR EXIT FAILURE ROLLBACK;

DELETE from KR_TBL_DATASRC_DEF;
commit;

insert into KR_TBL_DATASRC_DEF (DATASRC_ID, DATASRC_NAME, DATASRC_DESC, DATASRC_USAGE, MMAP_FILE_NAME, SIZE_KEEP_MODE, SIZE_KEEP_VALUE)
values (1, 'KR_Trans_Flow', 'krproject trans flow', '1', 'kr_trans_flow_mmap.db', '0', 20);
commit;
