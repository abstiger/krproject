prompt PL/SQL Developer import file
prompt Created on 2012年5月23日 by 111111
set feedback off
set define off
prompt Disabling triggers for KR_TBL_RULE...
alter table KR_TBL_RULE disable all triggers;
prompt Truncating KR_TBL_RULE...
truncate table KR_TBL_RULE;
prompt Loading KR_TBL_RULE...
insert into KR_TBL_RULE (RULE_ID, RULE_NAME, RULE_DESC, RULE_STRING, RULE_TYPE, RULE_WEIGHT, RISK_ADVC_THRESH, RISK_NOTIF_THRESH, RISK_ALERT_THRESH, RISK_WARN_THRESH, THRESH_IS_CONFIG, RULE_STATUS, REC_CRET_DTTM, LST_UPD_DTTM, LST_UPD_USER_ID)
values (4, 'rule_4', '测试规则', '(S_1>''091913'')&&(S_1<''091930'')&&(D_1>30)&&(C_3 >=''20120501'');', '1', 50, 0, 0, 0, 0, '0000', '1', '20120523141650   ', '20120523141650   ', 'Tiger');
commit;
prompt 1 records loaded
prompt Enabling triggers for KR_TBL_RULE...
alter table KR_TBL_RULE enable all triggers;
set feedback on
set define on
prompt Done.
