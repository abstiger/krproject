/*==============================================================*/
/* 检测模块使用：	c	                                        */
/* WEB模块使用：		                                        */
/* 采集模块使用：		                                        */
/*==============================================================*/
WHENEVER SQLERROR EXIT FAILURE ROLLBACK;

DELETE from KR_TBL_DATASRC_INDEX_DEF;
commit;

insert into KR_TBL_DATASRC_INDEX_DEF (DATASRC_ID, INDEX_ID, INDEX_NAME, INDEX_DESC, INDEX_TYPE, INDEX_FIELD_ID, SORT_FIELD_ID)
values (1, 1, 'custno_index', '客户号索引', 'S', 2, 0);

commit;
