begin;

DELETE from KR_TBL_GROUP;

insert into KR_TBL_GROUP (GROUP_ID, GROUP_NAME, GROUP_DESC, GROUP_STRING, GROUP_FUNC, GROUP_STATUS, REC_CRET_DTTM, LST_UPD_DTTM, LST_UPD_USER_ID)
values (1, 'default_group', '默认群组', '(3 >= 1);', 'group_func', '1', '20120523141650   ', '20120523141650   ', 'Tiger');

commit;
