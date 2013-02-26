/*==============================================================*/
/* Database name:  KRDB                                         */
/* DBMS name:      MySQL 5.0                                    */
/* Created on:     2013/2/4 16:44:37                            */
/*==============================================================*/


drop database if exists KRDB;

use KRDB;

/*==============================================================*/
/* Table: KR_TBL_DATADIC_CFG                                    */
/*==============================================================*/
create table KR_TBL_DATADIC_CFG
(
   DATADIC_ID           int not null,
   DATADIC_KEY          varchar(30) not null,
   DATADIC_VALUE        varchar(200) not null,
   primary key (DATADIC_ID, DATADIC_KEY)
);

/*==============================================================*/
/* Table: KR_TBL_DATADIC_DEF                                    */
/*==============================================================*/
create table KR_TBL_DATADIC_DEF
(
   DATADIC_ID           int not null,
   DATADIC_NAME         varchar(30) not null,
   DATADIC_DESC         varchar(100) not null,
   REC_CRET_DTTM        char(17) not null,
   LST_UPD_DTTM         char(17) not null,
   LST_UPD_USER_ID      varchar(15) not null,
   primary key (DATADIC_ID)
);

/*==============================================================*/
/* Table: KR_TBL_DATASRC_DEF                                    */
/*==============================================================*/
create table KR_TBL_DATASRC_DEF
(
   DATASRC_ID           int not null,
   DATASRC_NAME         varchar(30) not null,
   DATASRC_DESC         varchar(100) not null,
   MAP_FUNC_PRE         varchar(50) not null,
   MAP_FUNC             varchar(50) not null,
   MAP_FUNC_POST        varchar(50) not null,
   DATASRC_USAGE        char(1) not null comment '‘0’-批量数据源，‘1’-准实时数据源',
   MMAP_FILE_NAME       varchar(100) not null,
   SIZE_KEEP_MODE       char(1) not null,
   SIZE_KEEP_VALUE      int not null,
   primary key (DATASRC_ID)
);

/*==============================================================*/
/* Table: KR_TBL_DATASRC_FIELD_DEF                              */
/*==============================================================*/
create table KR_TBL_DATASRC_FIELD_DEF
(
   DATASRC_ID           int not null,
   FIELD_ID             int not null,
   FIELD_NAME           varchar(30) not null,
   FIELD_DESC           varchar(100) not null,
   FIELD_TYPE           char(1) not null,
   FIELD_LENGTH         numeric(10,0) not null,
   IS_USED_BY_FLT       char(1) not null,
   IS_USED_BY_RULE      char(1) not null,
   FIELD_WEB_TYPE       varchar(20) not null,
   FIELD_TEXT_RECT      varchar(200) not null,
   FIELD_DATADIC_RECT   int not null,
   FIELD_USE_RECT       char(2) not null,
   FIELD_SET_RECT       char(2) not null,
   primary key (DATASRC_ID, FIELD_ID)
);

/*==============================================================*/
/* Table: KR_TBL_DATASRC_INDEX_DEF                              */
/*==============================================================*/
create table KR_TBL_DATASRC_INDEX_DEF
(
   DATASRC_ID           int not null,
   INDEX_ID             int not null,
   INDEX_NAME           varchar(30) not null,
   INDEX_DESC           varchar(100) not null,
   INDEX_TYPE           char(1) not null,
   INDEX_FIELD_ID       int not null,
   SORT_FIELD_ID        int not null,
   primary key (DATASRC_ID, INDEX_ID)
);

/*==============================================================*/
/* Table: KR_TBL_DDI_DEF                                        */
/*==============================================================*/
create table KR_TBL_DDI_DEF
(
   DDI_ID               int not null,
   DDI_NAME             varchar(30) not null,
   DDI_DESC             varchar(100) not null,
   DDI_TYPE             char(1) not null comment '‘0’-批量统计
            ‘1’-集中实时统计
            ‘9’-动态计算统计
            ',
   DDI_VALUE_TYPE       char(1) not null,
   DDI_AGGR_FUNC        varchar(50) not null,
   DDI_FREE_FUNC        varchar(50) not null,
   STATISTICS_DATASRC   int not null,
   STATISTICS_INDEX     int not null,
   STATISTICS_FIELD     int not null,
   STATISTICS_TYPE      char(1) not null,
   STATISTICS_VALUE     int not null,
   STATISTICS_COUNT     int not null,
   DDI_FILTER_STRING    varchar(500) not null,
   STATISTICS_METHOD    char(1) not null,
   DDI_STATUS           char(1) not null,
   REC_CRET_DTTM        char(17) not null,
   LST_UPD_DTTM         char(17) not null,
   LST_UPD_USER_ID      varchar(15) not null,
   primary key (DDI_ID)
);

/*==============================================================*/
/* Table: KR_TBL_DYNDATA_DAY                                    */
/*==============================================================*/
create table KR_TBL_DYNDATA_DAY
(
   DATA_DATE            char(8) not null,
   DATA_OBJECT          varchar(30) not null,
   DATA_ID              int not null,
   DATA_VALUE           numeric(20,2) not null,
   primary key (DATA_DATE, DATA_OBJECT, DATA_ID)
);

/*==============================================================*/
/* Table: KR_TBL_DYNDATA_FLAG                                   */
/*==============================================================*/
create table KR_TBL_DYNDATA_FLAG
(
   DATA_OBJECT          varchar(30) not null,
   DATA_ID              int not null,
   DATA_FLAG            varchar(200) not null,
   primary key (DATA_OBJECT, DATA_ID)
);

/*==============================================================*/
/* Table: KR_TBL_DYNDATA_MON                                    */
/*==============================================================*/
create table KR_TBL_DYNDATA_MON
(
   DATA_MONTH           char(6) not null,
   DATA_OBJECT          varchar(30) not null,
   DATA_ID              int not null,
   DATA_VALUE           numeric(20,2) not null,
   primary key (DATA_MONTH, DATA_OBJECT, DATA_ID)
);

/*==============================================================*/
/* Table: KR_TBL_GROUP                                          */
/*==============================================================*/
create table KR_TBL_GROUP
(
   GROUP_ID             int not null,
   GROUP_NAME           varchar(30) not null,
   GROUP_DESC           varchar(100) not null,
   GROUP_STRING         varchar(500) not null,
   GROUP_FUNC           varchar(50) not null,
   GROUP_STATUS         char(1) not null comment '1-已启用
            2-暂停
            3-待复核
            4-已复核
            5-复核拒绝
            6-已删除
            ',
   REC_CRET_DTTM        char(17) not null,
   LST_UPD_DTTM         char(17) not null,
   LST_UPD_USER_ID      varchar(15) not null,
   primary key (GROUP_ID)
);

/*==============================================================*/
/* Table: KR_TBL_HDI_DEF                                        */
/*==============================================================*/
create table KR_TBL_HDI_DEF
(
   HDI_ID               int not null,
   HDI_NAME             varchar(30) not null,
   HDI_DESC             varchar(100) not null,
   HDI_TYPE             char(1) not null comment '‘0’-批量统计
            ‘1’-集中实时统计
            ‘9’-动态计算统计
            ',
   HDI_VALUE_TYPE       char(1) not null,
   HDI_AGGR_FUNC        varchar(50) not null,
   HDI_FREE_FUNC        varchar(50) not null,
   STATISTICS_DATASRC   int not null,
   STATISTICS_INDEX     int not null,
   STATISTICS_TYPE      char(1) not null,
   STATISTICS_VALUE     int not null,
   STATISTICS_METHOD    char(1) not null,
   HDI_STATUS           char(1) not null,
   REC_CRET_DTTM        char(17) not null,
   LST_UPD_DTTM         char(17) not null,
   LST_UPD_USER_ID      varchar(15) not null,
   primary key (HDI_ID)
);

/*==============================================================*/
/* Table: KR_TBL_RULE                                           */
/*==============================================================*/
create table KR_TBL_RULE
(
   GROUP_ID             int not null,
   RULE_ID              int not null,
   RULE_NAME            varchar(30) not null,
   RULE_DESC            varchar(100) not null,
   RULE_DATASRC         int not null,
   RULE_STRING          varchar(500) not null,
   RULE_TYPE            char(1) not null,
   RULE_FUNC            varchar(50) not null,
   RULE_WEIGHT          int not null,
   RISK_ADVC_THRESH     decimal(20,4) not null,
   RISK_NOTIF_THRESH    decimal(20,4) not null,
   RISK_ALERT_THRESH    decimal(20,4) not null,
   RISK_WARN_THRESH     decimal(20,4) not null,
   THRESH_IS_CONFIG     char(4) not null,
   RULE_STATUS          char(1) not null comment '1-已启用
            2-暂停
            3-待复核
            4-已复核
            5-复核拒绝
            6-已删除
            ',
   REC_CRET_DTTM        char(17) not null,
   LST_UPD_DTTM         char(17) not null,
   LST_UPD_USER_ID      varchar(15) not null,
   primary key (GROUP_ID, RULE_ID)
);

/*==============================================================*/
/* Table: KR_TBL_SDI_DEF                                        */
/*==============================================================*/
create table KR_TBL_SDI_DEF
(
   SDI_ID               int not null comment '针对每个数据源都需要从0开始以1为步长递增
            主要是方便后台的数组定位
            ',
   SDI_NAME             varchar(30) not null,
   SDI_DESC             varchar(100) not null,
   SDI_TYPE             char(1) not null comment '''0''—取自数据源
            ''1''—自定义获取
            ',
   SDI_VALUE_TYPE       char(1) not null,
   SDI_AGGR_FUNC        varchar(50) not null,
   SDI_FREE_FUNC        varchar(50) not null,
   STATISTICS_DATASRC   int not null,
   STATISTICS_INDEX     int not null,
   STATISTICS_FIELD     int not null,
   STATISTICS_LOCATION  int not null comment '当笔为0，
            上笔为1，
            以此类推
            ',
   LOCATION_PROPERTY    char(1) not null comment '''0''-绝对定位 ''1''-相对定位',
   SDI_FILTER_STRING    varchar(500) not null comment '取自过滤条件表的FILTER_ID',
   SDI_STATUS           char(1) not null comment '‘1’-已删除
            ‘0’-正常
            ',
   REC_CRET_DTTM        char(17) not null,
   LST_UPD_DTTM         char(17) not null,
   LST_UPD_USER_ID      varchar(15) not null,
   primary key (SDI_ID)
);

/*==============================================================*/
/* Table: KR_TBL_SET_CFG                                        */
/*==============================================================*/
create table KR_TBL_SET_CFG
(
   SET_ID               int not null,
   ELEMENT_VALUE        varchar(200) not null,
   ELEMENT_DESC         varchar(200) not null,
   primary key (SET_ID, ELEMENT_VALUE)
);

/*==============================================================*/
/* Table: KR_TBL_SET_DEF                                        */
/*==============================================================*/
create table KR_TBL_SET_DEF
(
   SET_ID               int not null,
   SET_NAME             varchar(30) not null,
   SET_DESC             varchar(100) not null,
   SET_USAGE            char(2) not null,
   SET_TYPE             char(2) not null,
   ELEMENT_TYPE         char(1) not null,
   ELEMENT_LENGTH       int not null,
   REC_CRET_DTTM        char(17) not null,
   LST_UPD_DTTM         char(17) not null,
   LST_UPD_USER_ID      varchar(15) not null,
   primary key (SET_ID)
);

/*==============================================================*/
/* Table: KR_TBL_SET_TYPE_DEF                                   */
/*==============================================================*/
create table KR_TBL_SET_TYPE_DEF
(
   SET_TYPE             char(2) not null,
   SET_TYPE_NAME        varchar(30) not null,
   SET_TYPE_DESC        varchar(100) not null,
   ELEMENT_TYPE         char(1) not null,
   ELEMENT_LENGTH       int not null,
   REC_CRET_DTTM        char(17) not null,
   LST_UPD_DTTM         char(17) not null,
   LST_UPD_USER_ID      varchar(15) not null,
   primary key (SET_TYPE)
);

