prompt PL/SQL Developer import file
prompt Created on 2012年5月23日 by 111111
set feedback off
set define off
prompt Disabling triggers for KR_TBL_DDI_DEF...
alter table KR_TBL_DDI_DEF disable all triggers;
prompt Truncating KR_TBL_DDI_DEF...
truncate table KR_TBL_DDI_DEF;
prompt Loading KR_TBL_DDI_DEF...
insert into KR_TBL_DDI_DEF (DDI_ID, DDI_NAME, DDI_DESC, DDI_TYPE, DDI_VALUE_TYPE, DDI_AGGR_FUNC, STATISTICS_DATASRC, STATISTICS_INDEX, STATISTICS_FIELD, STATISTICS_TYPE, STATISTICS_VALUE, STATISTICS_COUNT, DDI_FILTER_STRING, STATISTICS_METHOD, DDI_STATUS, REC_CRET_DTTM, LST_UPD_DTTM, LST_UPD_USER_ID)
values (1, '5mins_trans_amt_total', '5分钟交易金额总和', '0', 'D', '                                                  ', 1, 1, 7, 'I', 300, 50, '(F_6 @@ {''00'',''01'',''02'',} );', '0', '0', '20120523141650   ', '20120523141650   ', 'Tiger');
commit;
prompt 1 records loaded
prompt Enabling triggers for KR_TBL_DDI_DEF...
alter table KR_TBL_DDI_DEF enable all triggers;
set feedback on
set define on
prompt Done.
