/*==============================================================*/
/* Database name:  KRDB                                         */
/* DBMS name:      ORACLE Version 10g                           */
/* Created on:     2012/10/17 15:59:38                          */
/*==============================================================*/


alter table KR_TBL_DATADIC_CFG
   drop primary key cascade;

drop table KR_TBL_DATADIC_CFG cascade constraints;

alter table KR_TBL_DATADIC_DEF
   drop primary key cascade;

drop table KR_TBL_DATADIC_DEF cascade constraints;

alter table KR_TBL_DATASRC_DEF
   drop primary key cascade;

drop table KR_TBL_DATASRC_DEF cascade constraints;

alter table KR_TBL_DATASRC_FIELD_DEF
   drop primary key cascade;

drop table KR_TBL_DATASRC_FIELD_DEF cascade constraints;

alter table KR_TBL_DATASRC_INDEX_DEF
   drop primary key cascade;

drop table KR_TBL_DATASRC_INDEX_DEF cascade constraints;

alter table KR_TBL_DDI_DEF
   drop primary key cascade;

drop table KR_TBL_DDI_DEF cascade constraints;

alter table KR_TBL_DYNDATA_DAY
   drop primary key cascade;

drop table KR_TBL_DYNDATA_DAY cascade constraints;

alter table KR_TBL_DYNDATA_FLAG
   drop primary key cascade;

drop table KR_TBL_DYNDATA_FLAG cascade constraints;

alter table KR_TBL_DYNDATA_MON
   drop primary key cascade;

drop table KR_TBL_DYNDATA_MON cascade constraints;

alter table KR_TBL_HDI_DEF
   drop primary key cascade;

drop table KR_TBL_HDI_DEF cascade constraints;

alter table KR_TBL_RULE
   drop primary key cascade;

drop table KR_TBL_RULE cascade constraints;

alter table KR_TBL_SDI_DEF
   drop primary key cascade;

drop table KR_TBL_SDI_DEF cascade constraints;

alter table KR_TBL_SET_CFG
   drop primary key cascade;

drop table KR_TBL_SET_CFG cascade constraints;

alter table KR_TBL_SET_DEF
   drop primary key cascade;

drop table KR_TBL_SET_DEF cascade constraints;

alter table KR_TBL_SET_TYPE_DEF
   drop primary key cascade;

drop table KR_TBL_SET_TYPE_DEF cascade constraints;

/*==============================================================*/
/* Table: KR_TBL_DATADIC_CFG                                    */
/*==============================================================*/
create table KR_TBL_DATADIC_CFG  (
   DATADIC_ID           INTEGER                         not null,
   DATADIC_KEY          VARCHAR2(30)                    not null,
   DATADIC_VALUE        VARCHAR2(200)                   not null
);

alter table KR_TBL_DATADIC_CFG
   add constraint PK_KR_TBL_DATADIC_CFG primary key (DATADIC_ID, DATADIC_KEY);

/*==============================================================*/
/* Table: KR_TBL_DATADIC_DEF                                    */
/*==============================================================*/
create table KR_TBL_DATADIC_DEF  (
   DATADIC_ID           INTEGER                         not null,
   DATADIC_NAME         VARCHAR2(30)                    not null,
   DATADIC_DESC         VARCHAR2(100)                   not null,
   REC_CRET_DTTM        CHAR(17)                        not null,
   LST_UPD_DTTM         CHAR(17)                        not null,
   LST_UPD_USER_ID      VARCHAR2(15)                    not null
);

alter table KR_TBL_DATADIC_DEF
   add constraint PK_KR_TBL_DATADIC_DEF primary key (DATADIC_ID);

/*==============================================================*/
/* Table: KR_TBL_DATASRC_DEF                                    */
/*==============================================================*/
create table KR_TBL_DATASRC_DEF  (
   DATASRC_ID           INTEGER                         not null,
   DATASRC_NAME         VARCHAR2(30)                    not null,
   DATASRC_DESC         VARCHAR2(100)                   not null,
   DATASRC_MAP_FUNC     VARCHAR2(50)                    not null,
   DATASRC_USAGE        CHAR(1)                         not null,
   MMAP_FILE_NAME       VARCHAR2(100)                   not null,
   SIZE_KEEP_MODE       CHAR(1)                         not null,
   SIZE_KEEP_VALUE      INTEGER                         not null
);

alter table KR_TBL_DATASRC_DEF
   add constraint PK_KR_TBL_DATASRC_DEF primary key (DATASRC_ID);

/*==============================================================*/
/* Table: KR_TBL_DATASRC_FIELD_DEF                              */
/*==============================================================*/
create table KR_TBL_DATASRC_FIELD_DEF  (
   DATASRC_ID           INTEGER                         not null,
   FIELD_ID             INTEGER                         not null,
   FIELD_NAME           VARCHAR2(30)                    not null,
   FIELD_DESC           VARCHAR2(100)                   not null,
   FIELD_TYPE           CHAR(1)                         not null,
   FIELD_LENGTH         NUMBER(10,0)                    not null,
   IS_USED_BY_FLT       CHAR(1)                         not null,
   IS_USED_BY_RULE      CHAR(1)                         not null,
   FIELD_WEB_TYPE       VARCHAR2(20)                    not null,
   FIELD_TEXT_RECT      VARCHAR2(200)                   not null,
   FIELD_DATADIC_RECT   INTEGER                         not null,
   FIELD_USE_RECT       CHAR(2)                         not null,
   FIELD_SET_RECT       CHAR(2)                         not null
);

alter table KR_TBL_DATASRC_FIELD_DEF
   add constraint PK_KR_TBL_DATASRC_FIELD_DEF primary key (DATASRC_ID, FIELD_ID);

/*==============================================================*/
/* Table: KR_TBL_DATASRC_INDEX_DEF                              */
/*==============================================================*/
create table KR_TBL_DATASRC_INDEX_DEF  (
   DATASRC_ID           INTEGER                         not null,
   INDEX_ID             INTEGER                         not null,
   INDEX_NAME           VARCHAR2(30)                    not null,
   INDEX_DESC           VARCHAR2(100)                   not null,
   INDEX_TYPE           CHAR(1)                         not null,
   INDEX_FIELD_ID       INTEGER                         not null,
   SORT_FIELD_ID        INTEGER                         not null
);

alter table KR_TBL_DATASRC_INDEX_DEF
   add constraint PK_KR_TBL_DATASRC_INDEX_DEF primary key (DATASRC_ID, INDEX_ID);

/*==============================================================*/
/* Table: KR_TBL_DDI_DEF                                        */
/*==============================================================*/
create table KR_TBL_DDI_DEF  (
   DDI_ID               INTEGER                         not null,
   DDI_NAME             VARCHAR2(30)                    not null,
   DDI_DESC             VARCHAR2(100)                   not null,
   DDI_TYPE             CHAR(1)                         not null,
   DDI_VALUE_TYPE       CHAR(1)                         not null,
   DDI_AGGR_FUNC        VARCHAR2(50)                    not null,
   DDI_FREE_FUNC        VARCHAR2(50)                    not null,
   STATISTICS_DATASRC   INTEGER                         not null,
   STATISTICS_INDEX     INTEGER                         not null,
   STATISTICS_FIELD     INTEGER                         not null,
   STATISTICS_TYPE      CHAR(1)                         not null,
   STATISTICS_VALUE     INTEGER                         not null,
   STATISTICS_COUNT     INTEGER                         not null,
   DDI_FILTER_STRING    VARCHAR2(500)                   not null,
   STATISTICS_METHOD    CHAR(1)                         not null,
   DDI_STATUS           CHAR(1)                         not null,
   REC_CRET_DTTM        CHAR(17)                        not null,
   LST_UPD_DTTM         CHAR(17)                        not null,
   LST_UPD_USER_ID      VARCHAR2(15)                    not null
);

alter table KR_TBL_DDI_DEF
   add constraint PK_KR_TBL_DDI_DEF primary key (DDI_ID);

/*==============================================================*/
/* Table: KR_TBL_DYNDATA_DAY                                    */
/*==============================================================*/
create table KR_TBL_DYNDATA_DAY  (
   DATA_DATE            CHAR(8)                         not null,
   DATA_OBJECT          VARCHAR2(30)                    not null,
   DATA_ID              INTEGER                         not null,
   DATA_VALUE           NUMBER(20,2)                    not null
);

alter table KR_TBL_DYNDATA_DAY
   add constraint PK_KR_TBL_DYNDATA_DAY primary key (DATA_DATE, DATA_OBJECT, DATA_ID);

/*==============================================================*/
/* Table: KR_TBL_DYNDATA_FLAG                                   */
/*==============================================================*/
create table KR_TBL_DYNDATA_FLAG  (
   DATA_OBJECT          VARCHAR2(30)                    not null,
   DATA_ID              INTEGER                         not null,
   DATA_FLAG            VARCHAR2(200)                   not null
);

alter table KR_TBL_DYNDATA_FLAG
   add constraint PK_KR_TBL_DYNDATA_FLAG primary key (DATA_OBJECT, DATA_ID);

/*==============================================================*/
/* Table: KR_TBL_DYNDATA_MON                                    */
/*==============================================================*/
create table KR_TBL_DYNDATA_MON  (
   DATA_MONTH           CHAR(6)                         not null,
   DATA_OBJECT          VARCHAR2(30)                    not null,
   DATA_ID              INTEGER                         not null,
   DATA_VALUE           NUMBER(20,2)                    not null
);

alter table KR_TBL_DYNDATA_MON
   add constraint PK_KR_TBL_DYNDATA_MON primary key (DATA_MONTH, DATA_OBJECT, DATA_ID);

/*==============================================================*/
/* Table: KR_TBL_HDI_DEF                                        */
/*==============================================================*/
create table KR_TBL_HDI_DEF  (
   HDI_ID               INTEGER                         not null,
   HDI_NAME             VARCHAR2(30)                    not null,
   HDI_DESC             VARCHAR2(100)                   not null,
   HDI_TYPE             CHAR(1)                         not null,
   HDI_VALUE_TYPE       CHAR(1)                         not null,
   HDI_AGGR_FUNC        VARCHAR2(50)                    not null,
   HDI_FREE_FUNC        VARCHAR2(50)                    not null,
   STATISTICS_DATASRC   INTEGER                         not null,
   STATISTICS_DIMENSION INTEGER                         not null,
   STATISTICS_TYPE      CHAR(1)                         not null,
   STATISTICS_VALUE     INTEGER                         not null,
   STATISTICS_METHOD    CHAR(1)                         not null,
   HDI_STATUS           CHAR(1)                         not null,
   REC_CRET_DTTM        CHAR(17)                        not null,
   LST_UPD_DTTM         CHAR(17)                        not null,
   LST_UPD_USER_ID      VARCHAR2(15)                    not null
);

alter table KR_TBL_HDI_DEF
   add constraint PK_KR_TBL_HDI_DEF primary key (HDI_ID);

/*==============================================================*/
/* Table: KR_TBL_RULE                                           */
/*==============================================================*/
create table KR_TBL_RULE  (
   RULE_ID              INTEGER                         not null,
   RULE_NAME            VARCHAR2(30)                    not null,
   RULE_DESC            VARCHAR2(100)                   not null,
   RULE_STRING          VARCHAR2(500)                   not null,
   RULE_TYPE            CHAR(1)                         not null,
   RULE_WEIGHT          INTEGER                         not null,
   RISK_ADVC_THRESH     NUMBER(20,4)                    not null,
   RISK_NOTIF_THRESH    NUMBER(20,4)                    not null,
   RISK_ALERT_THRESH    NUMBER(20,4)                    not null,
   RISK_WARN_THRESH     NUMBER(20,4)                    not null,
   THRESH_IS_CONFIG     CHAR(4)                         not null,
   RULE_STATUS          CHAR(1)                         not null,
   REC_CRET_DTTM        CHAR(17)                        not null,
   LST_UPD_DTTM         CHAR(17)                        not null,
   LST_UPD_USER_ID      VARCHAR2(15)                    not null
);

alter table KR_TBL_RULE
   add constraint PK_KR_TBL_RULE primary key (RULE_ID);

/*==============================================================*/
/* Table: KR_TBL_SDI_DEF                                        */
/*==============================================================*/
create table KR_TBL_SDI_DEF  (
   SDI_ID               INTEGER                         not null,
   SDI_NAME             VARCHAR2(30)                    not null,
   SDI_DESC             VARCHAR2(100)                   not null,
   SDI_TYPE             CHAR(1)                         not null,
   SDI_VALUE_TYPE       CHAR(1)                         not null,
   SDI_AGGR_FUNC        VARCHAR2(50)                    not null,
   SDI_FREE_FUNC        VARCHAR2(50)                    not null,
   STATISTICS_DATASRC   INTEGER                         not null,
   STATISTICS_INDEX     INTEGER                         not null,
   STATISTICS_FIELD     INTEGER                         not null,
   STATISTICS_LOCATION  INTEGER                         not null,
   LOCATION_PROPERTY    CHAR(1)                         not null,
   SDI_FILTER_STRING    VARCHAR2(500)                   not null,
   SDI_STATUS           CHAR(1)                         not null,
   REC_CRET_DTTM        CHAR(17)                        not null,
   LST_UPD_DTTM         CHAR(17)                        not null,
   LST_UPD_USER_ID      VARCHAR2(15)                    not null
);

alter table KR_TBL_SDI_DEF
   add constraint PK_KR_TBL_SDI_DEF primary key (SDI_ID);

/*==============================================================*/
/* Table: KR_TBL_SET_CFG                                        */
/*==============================================================*/
create table KR_TBL_SET_CFG  (
   SET_ID               INTEGER                         not null,
   ELEMENT_VALUE        VARCHAR2(200)                   not null,
   ELEMENT_DESC         VARCHAR2(200)                   not null
);

alter table KR_TBL_SET_CFG
   add constraint PK_KR_TBL_SET_CFG primary key (SET_ID, ELEMENT_VALUE);

/*==============================================================*/
/* Table: KR_TBL_SET_DEF                                        */
/*==============================================================*/
create table KR_TBL_SET_DEF  (
   SET_ID               INTEGER                         not null,
   SET_NAME             VARCHAR2(30)                    not null,
   SET_DESC             VARCHAR2(100)                   not null,
   SET_USAGE            CHAR(2)                         not null,
   SET_TYPE             CHAR(2)                         not null,
   ELEMENT_TYPE         CHAR(1)                         not null,
   ELEMENT_LENGTH       INTEGER                         not null,
   REC_CRET_DTTM        CHAR(17)                        not null,
   LST_UPD_DTTM         CHAR(17)                        not null,
   LST_UPD_USER_ID      VARCHAR2(15)                    not null
);

alter table KR_TBL_SET_DEF
   add constraint PK_KR_TBL_SET_DEF primary key (SET_ID);

/*==============================================================*/
/* Table: KR_TBL_SET_TYPE_DEF                                   */
/*==============================================================*/
create table KR_TBL_SET_TYPE_DEF  (
   SET_TYPE             CHAR(2)                         not null,
   SET_TYPE_NAME        VARCHAR2(30)                    not null,
   SET_TYPE_DESC        VARCHAR2(100)                   not null,
   ELEMENT_TYPE         CHAR(1)                         not null,
   ELEMENT_LENGTH       INTEGER                         not null,
   REC_CRET_DTTM        CHAR(17)                        not null,
   LST_UPD_DTTM         CHAR(17)                        not null,
   LST_UPD_USER_ID      VARCHAR2(15)                    not null
);

alter table KR_TBL_SET_TYPE_DEF
   add constraint PK_KR_TBL_SET_TYPE_DEF primary key (SET_TYPE);

