begin;

DELETE from KR_TBL_SDI_DEF;

insert into KR_TBL_SDI_DEF (SDI_ID, SDI_NAME, SDI_DESC, SDI_TYPE, SDI_VALUE_TYPE, SDI_AGGR_FUNC, SDI_FREE_FUNC, STATISTICS_DATASRC, STATISTICS_INDEX, STATISTICS_FIELD, STATISTICS_LOCATION, LOCATION_PROPERTY, SDI_FILTER_STRING, SDI_STATUS, REC_CRET_DTTM, LST_UPD_DTTM, LST_UPD_USER_ID)
values (1, 'last_trans_time', '上笔交易金额大于500的当笔交易时间', '0', 'S', ' ',' ', 1, 1, 4, 1, 'R', '{"op":9,"child0":{"kind":7,"value":7},"child1":{"kind":3,"value":500}}', '0', '20120523141050   ', '20120523141050   ', 'Tiger');

commit;
