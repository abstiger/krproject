begin;

DELETE from KR_TBL_DATASRC;

insert into KR_TBL_DATASRC (DATASRC_ID, DATASRC_NAME, DATASRC_DESC, DATASRC_FORMAT, DATASRC_USAGE, SIZE_KEEP_MODE, SIZE_KEEP_VALUE) values (1, 'kr_trans_flow', 'antifraud trans flow', 'json', '1', '0', 200);

commit;
