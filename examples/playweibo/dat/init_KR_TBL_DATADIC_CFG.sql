begin;

DELETE from KR_TBL_DATADIC_CFG;

insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (1, '0', '新建');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (1, '1', '已启用');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (1, '2', '已删除');

insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (11, '0', '保持数据表记录条数');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (11, '1', '保持数据表时间间隔');

insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (12, 'B','布尔型');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (12, 'C','字符型');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (12, 'I','整型');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (12, 'L','长整型');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (12, 'D','双精度浮点型');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (12, 'S','字符串类型');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (12, 'P','指针类型');

insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (13, 'S','单表索引');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (13, 'M','多表索引');

insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (21, '0','取自数据源');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (21, '1','自定义获取');

insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (22, 'F','flex and bison');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (22, '1','json');

insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (23, 'A','绝对定位');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (23, 'R','相对定位');

insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (24, 'I','包含当笔');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (24, 'E','不含当笔');

insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (25, '0','日统计量');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (25, '1','月统计量');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (25, '2','标志统计量');

insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (26, '0','sum');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (26, '1','min');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (26, '2','max');
insert into KR_TBL_DATADIC_CFG (DATADIC_ID, DATADIC_KEY, DATADIC_VALUE) values (26, '3','count');

commit;
