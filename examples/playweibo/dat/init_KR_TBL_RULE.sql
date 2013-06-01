begin;

DELETE from KR_TBL_RULE;

insert into KR_TBL_RULE (GROUP_ID, RULE_ID, RULE_NAME, RULE_DESC, RULE_DATASRC, RULE_STRING, RULE_TYPE, RULE_FUNC, RULE_WEIGHT, RULE_STATUS, REC_CRET_DTTM, LST_UPD_DTTM, LST_UPD_USER_ID)
values (1, 1, 'he is lufang', '卢放的微博', 2, '{"op":10,"child":[{"kind":11,"value":2},{"kind":4,"value":2398032034}]}', '1', '', 50, '1', '20120523141650   ', '20120523141650   ', 'Tiger');

insert into KR_TBL_RULE (GROUP_ID, RULE_ID, RULE_NAME, RULE_DESC, RULE_DATASRC, RULE_STRING, RULE_TYPE, RULE_FUNC, RULE_WEIGHT, RULE_STATUS, REC_CRET_DTTM, LST_UPD_DTTM, LST_UPD_USER_ID)
values (1, 2, 'she is female', '女性微博', 2, '{"op":10,"child":[{"kind":11,"value":5},{"kind":5,"value":"f"}]}', '1', '', 50, '1', '20120523141650   ', '20120523141650   ', 'Tiger');

insert into KR_TBL_RULE (GROUP_ID, RULE_ID, RULE_NAME, RULE_DESC, RULE_DATASRC, RULE_STRING, RULE_TYPE, RULE_FUNC, RULE_WEIGHT, RULE_STATUS, REC_CRET_DTTM, LST_UPD_DTTM, LST_UPD_USER_ID)
values (1, 3, 'he is male', '男性微博', 2, '{"op":10,"child":[{"kind":11,"value":5},{"kind":5,"value":"m"}]}', '1', '', 50, '1', '20120523141650   ', '20120523141650   ', 'Tiger');

commit;

