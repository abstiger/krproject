begin;

DELETE from KR_TBL_RULE;

insert into KR_TBL_RULE (RULE_ID, RULE_NAME, RULE_DESC, RULE_DATASRC, RULE_CALC_FORMAT, RULE_CALC_STRING, RULE_TYPE, RULE_FUNC, RULE_WEIGHT, RULE_STATUS, REC_CRET_DTTM, LST_UPD_DTTM, LST_UPD_USER_ID) values (1, 'rule_1', '测试规则1', 1, 'J', '{"op":8,"child":[{"kind":14,"value":1},{"kind":4,"value":200}]}', '1', '', 50, '1', '20120523141650   ', '20120523141650   ', 'Tiger');

commit;
