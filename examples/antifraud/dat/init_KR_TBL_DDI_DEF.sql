begin;

DELETE from KR_TBL_DDI_DEF;

insert into KR_TBL_DDI_DEF (DDI_ID, DDI_NAME, DDI_DESC, DDI_TYPE, DDI_VALUE_TYPE, DDI_AGGR_FUNC, DDI_FREE_FUNC, STATISTICS_DATASRC, STATISTICS_INDEX, STATISTICS_FIELD, STATISTICS_TYPE, STATISTICS_VALUE, STATISTICS_COUNT, DDI_FILTER_STRING, STATISTICS_METHOD, DDI_STATUS, REC_CRET_DTTM, LST_UPD_DTTM, LST_UPD_USER_ID)
values (1, '5mins_trans_amt_total', '5分钟交易金额总和', '0', 'D', ' ',' ', 1, 1, 7, 'I', 300, 50, '{"op":15,"child0":{"kind":7,"value":6},"child1":{"kind":14,"value":"00,01,02,"}}', '0', '0', '20120523141650   ', '20120523141650   ', 'Tiger');

commit;
