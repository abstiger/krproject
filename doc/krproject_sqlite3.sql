--==============================================================
-- Database name:  KRDB
-- DBMS name:      Sqlite3 Server
-- Created on:     2012/11/1 17:49:32
--==============================================================


drop table KR_TBL_DATADIC_CFG;

drop table KR_TBL_DATADIC_DEF;

drop table KR_TBL_DATASRC_DEF;

drop table KR_TBL_DATASRC_FIELD_DEF;

drop table KR_TBL_DATASRC_INDEX_DEF;

drop table KR_TBL_DDI_DEF;

drop table KR_TBL_DYNDATA_DAY;

drop table KR_TBL_DYNDATA_FLAG;

drop table KR_TBL_DYNDATA_MON;

drop table KR_TBL_HDI_DEF;

drop table KR_TBL_RULE;

drop table KR_TBL_SDI_DEF;

drop table KR_TBL_SET_CFG;

drop table KR_TBL_SET_DEF;

drop table KR_TBL_SET_TYPE_DEF;

--==============================================================
-- Table: KR_TBL_DATADIC_CFG
--==============================================================
create table KR_TBL_DATADIC_CFG
(
   DATADIC_ID           INTEGER                not null,
   DATADIC_KEY          VARCHAR(30)            not null,
   DATADIC_VALUE        VARCHAR(200)           not null,
   constraint P_PK_TBL_DATADIC primary key (DATADIC_ID, DATADIC_KEY)
);

--==============================================================
-- Table: KR_TBL_DATADIC_DEF
--==============================================================
create table KR_TBL_DATADIC_DEF
(
   DATADIC_ID           INTEGER                not null,
   DATADIC_NAME         VARCHAR(30)            not null,
   DATADIC_DESC         VARCHAR(100)           not null,
   REC_CRET_DTTM        CHAR(17)               not null,
   LST_UPD_DTTM         CHAR(17)               not null,
   LST_UPD_USER_ID      VARCHAR(15)            not null,
   constraint P_PK_DATADIC_ID primary key (DATADIC_ID)
);

--==============================================================
-- Table: KR_TBL_DATASRC_DEF
--==============================================================
create table KR_TBL_DATASRC_DEF
(
   DATASRC_ID           INTEGER                not null,
   DATASRC_NAME         VARCHAR(30)            not null,
   DATASRC_DESC         VARCHAR(100)           not null,
   MAP_FUNC_PRE         VARCHAR(50)            not null,
   MAP_FUNC             VARCHAR(50)            not null,
   MAP_FUNC_POST        VARCHAR(50)            not null,
   DATASRC_USAGE        CHAR(1)                not null,
   MMAP_FILE_NAME       VARCHAR(100)           not null,
   SIZE_KEEP_MODE       CHAR(1)                not null,
   SIZE_KEEP_VALUE      INTEGER                not null,
   constraint P_PK_DATASRC_ID primary key (DATASRC_ID)
);

--==============================================================
-- Table: KR_TBL_DATASRC_FIELD_DEF
--==============================================================
create table KR_TBL_DATASRC_FIELD_DEF
(
   DATASRC_ID           INTEGER                not null,
   FIELD_ID             INTEGER                not null,
   FIELD_NAME           VARCHAR(30)            not null,
   FIELD_DESC           VARCHAR(100)           not null,
   FIELD_TYPE           CHAR(1)                not null,
   FIELD_LENGTH         NUMERIC(10,0)          not null,
   IS_USED_BY_FLT       CHAR(1)                not null,
   IS_USED_BY_RULE      CHAR(1)                not null,
   FIELD_WEB_TYPE       VARCHAR(20)            not null,
   FIELD_TEXT_RECT      VARCHAR(200)           not null,
   FIELD_DATADIC_RECT   INTEGER                not null,
   FIELD_USE_RECT       CHAR(2)                not null,
   FIELD_SET_RECT       CHAR(2)                not null,
   constraint P_PK_TBL_DATASRC_F primary key (DATASRC_ID, FIELD_ID)
);

--==============================================================
-- Table: KR_TBL_DATASRC_INDEX_DEF
--==============================================================
create table KR_TBL_DATASRC_INDEX_DEF
(
   DATASRC_ID           INTEGER                not null,
   INDEX_ID             INTEGER                not null,
   INDEX_NAME           VARCHAR(30)            not null,
   INDEX_DESC           VARCHAR(100)           not null,
   INDEX_TYPE           CHAR(1)                not null,
   INDEX_FIELD_ID       INTEGER                not null,
   SORT_FIELD_ID        INTEGER                not null,
   constraint P_PK_TBL_DATASRC_I primary key (DATASRC_ID, INDEX_ID)
);

--==============================================================
-- Table: KR_TBL_DDI_DEF
--==============================================================
create table KR_TBL_DDI_DEF
(
   DDI_ID               INTEGER                not null,
   DDI_NAME             VARCHAR(30)            not null,
   DDI_DESC             VARCHAR(100)           not null,
   DDI_TYPE             CHAR(1)                not null,
   DDI_VALUE_TYPE       CHAR(1)                not null,
   DDI_AGGR_FUNC        VARCHAR(50)            not null,
   DDI_FREE_FUNC        VARCHAR(50)            not null,
   STATISTICS_DATASRC   INTEGER                not null,
   STATISTICS_INDEX     INTEGER                not null,
   STATISTICS_FIELD     INTEGER                not null,
   STATISTICS_TYPE      CHAR(1)                not null,
   STATISTICS_VALUE     INTEGER                not null,
   STATISTICS_COUNT     INTEGER                not null,
   DDI_FILTER_STRING    VARCHAR(500)           not null,
   STATISTICS_METHOD    CHAR(1)                not null,
   DDI_STATUS           CHAR(1)                not null,
   REC_CRET_DTTM        CHAR(17)               not null,
   LST_UPD_DTTM         CHAR(17)               not null,
   LST_UPD_USER_ID      VARCHAR(15)            not null,
   constraint P_PK_DDI_ID primary key (DDI_ID)
);

--==============================================================
-- Table: KR_TBL_DYNDATA_DAY
--==============================================================
create table KR_TBL_DYNDATA_DAY
(
   DATA_DATE            CHAR(8)                not null,
   DATA_OBJECT          VARCHAR(30)            not null,
   DATA_ID              INTEGER                not null,
   DATA_VALUE           NUMERIC(20,2)          not null,
   constraint "P_pk_tbl_dyndata_d" primary key (DATA_DATE, DATA_OBJECT, DATA_ID)
);

--==============================================================
-- Table: KR_TBL_DYNDATA_FLAG
--==============================================================
create table KR_TBL_DYNDATA_FLAG
(
   DATA_OBJECT          VARCHAR(30)            not null,
   DATA_ID              INTEGER                not null,
   DATA_FLAG            VARCHAR(200)           not null,
   constraint "P_pk_tbl_dyndata_d" primary key (DATA_OBJECT, DATA_ID)
);

--==============================================================
-- Table: KR_TBL_DYNDATA_MON
--==============================================================
create table KR_TBL_DYNDATA_MON
(
   DATA_MONTH           CHAR(6)                not null,
   DATA_OBJECT          VARCHAR(30)            not null,
   DATA_ID              INTEGER                not null,
   DATA_VALUE           NUMERIC(20,2)          not null,
   constraint "P_pk_tbl_dyndata_m" primary key (DATA_MONTH, DATA_OBJECT, DATA_ID)
);

--==============================================================
-- Table: KR_TBL_HDI_DEF
--==============================================================
create table KR_TBL_HDI_DEF
(
   HDI_ID               INTEGER                not null,
   HDI_NAME             VARCHAR(30)            not null,
   HDI_DESC             VARCHAR(100)           not null,
   HDI_TYPE             CHAR(1)                not null,
   HDI_VALUE_TYPE       CHAR(1)                not null,
   HDI_AGGR_FUNC        VARCHAR(50)            not null,
   HDI_FREE_FUNC        VARCHAR(50)            not null,
   STATISTICS_DATASRC   INTEGER                not null,
   STATISTICS_INDEX     INTEGER                not null,
   STATISTICS_TYPE      CHAR(1)                not null,
   STATISTICS_VALUE     INTEGER                not null,
   STATISTICS_METHOD    CHAR(1)                not null,
   HDI_STATUS           CHAR(1)                not null,
   REC_CRET_DTTM        CHAR(17)               not null,
   LST_UPD_DTTM         CHAR(17)               not null,
   LST_UPD_USER_ID      VARCHAR(15)            not null,
   constraint P_PK_DDI_ID primary key (HDI_ID)
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
   GROUP_STATUS         char(1) not null,
   REC_CRET_DTTM        char(17) not null,
   LST_UPD_DTTM         char(17) not null,
   LST_UPD_USER_ID      varchar(15) not null,
   constraint P_PK_GROUP_ID primary key (GROUP_ID)
);

--==============================================================
-- Table: KR_TBL_RULE
--==============================================================
create table KR_TBL_RULE
(
   GROUP_ID             INTEGER                not null,
   RULE_ID              INTEGER                not null,
   RULE_NAME            VARCHAR(30)            not null,
   RULE_DESC            VARCHAR(100)           not null,
   RULE_DATASRC         INTEGER                not null,
   RULE_STRING          VARCHAR(500)           not null,
   RULE_TYPE            CHAR(1)                not null,
   RULE_FUNC            VARCHAR(50)            not null,
   RULE_WEIGHT          INTEGER                not null,
   RISK_ADVC_THRESH     NUMERIC(20,4)          not null,
   RISK_NOTIF_THRESH    NUMERIC(20,4)          not null,
   RISK_ALERT_THRESH    NUMERIC(20,4)          not null,
   RISK_WARN_THRESH     NUMERIC(20,4)          not null,
   THRESH_IS_CONFIG     CHAR(4)                not null,
   RULE_STATUS          CHAR(1)                not null,
   REC_CRET_DTTM        CHAR(17)               not null,
   LST_UPD_DTTM         CHAR(17)               not null,
   LST_UPD_USER_ID      VARCHAR(15)            not null,
   constraint P_PK_RULE_ID primary key (RULE_ID)
);

--==============================================================
-- Table: KR_TBL_SDI_DEF
--==============================================================
create table KR_TBL_SDI_DEF
(
   SDI_ID               INTEGER                not null,
   SDI_NAME             VARCHAR(30)            not null,
   SDI_DESC             VARCHAR(100)           not null,
   SDI_TYPE             CHAR(1)                not null,
   SDI_VALUE_TYPE       CHAR(1)                not null,
   SDI_AGGR_FUNC        VARCHAR(50)            not null,
   SDI_FREE_FUNC        VARCHAR(50)            not null,
   STATISTICS_DATASRC   INTEGER                not null,
   STATISTICS_INDEX     INTEGER                not null,
   STATISTICS_FIELD     INTEGER                not null,
   STATISTICS_LOCATION  INTEGER                not null,
   LOCATION_PROPERTY    CHAR(1)                not null,
   SDI_FILTER_STRING    VARCHAR(500)           not null,
   SDI_STATUS           CHAR(1)                not null,
   REC_CRET_DTTM        CHAR(17)               not null,
   LST_UPD_DTTM         CHAR(17)               not null,
   LST_UPD_USER_ID      VARCHAR(15)            not null,
   constraint P_PK_SDI_ID primary key (SDI_ID)
);

--==============================================================
-- Table: KR_TBL_SET_CFG
--==============================================================
create table KR_TBL_SET_CFG
(
   SET_ID               INTEGER                not null,
   ELEMENT_VALUE        VARCHAR(200)           not null,
   ELEMENT_DESC         VARCHAR(200)           not null,
   constraint P_PK_TBL_SET_ELEME primary key (SET_ID, ELEMENT_VALUE)
);

--==============================================================
-- Table: KR_TBL_SET_DEF
--==============================================================
create table KR_TBL_SET_DEF
(
   SET_ID               INTEGER                not null,
   SET_NAME             VARCHAR(30)            not null,
   SET_DESC             VARCHAR(100)           not null,
   SET_USAGE            CHAR(2)                not null,
   SET_TYPE             CHAR(2)                not null,
   ELEMENT_TYPE         CHAR(1)                not null,
   ELEMENT_LENGTH       INTEGER                not null,
   REC_CRET_DTTM        CHAR(17)               not null,
   LST_UPD_DTTM         CHAR(17)               not null,
   LST_UPD_USER_ID      VARCHAR(15)            not null,
   constraint P_PK_SET_ID primary key (SET_ID)
);

--==============================================================
-- Table: KR_TBL_SET_TYPE_DEF
--==============================================================
create table KR_TBL_SET_TYPE_DEF
(
   SET_TYPE             CHAR(2)                not null,
   SET_TYPE_NAME        VARCHAR(30)            not null,
   SET_TYPE_DESC        VARCHAR(100)           not null,
   ELEMENT_TYPE         CHAR(1)                not null,
   ELEMENT_LENGTH       INTEGER                not null,
   REC_CRET_DTTM        CHAR(17)               not null,
   LST_UPD_DTTM         CHAR(17)               not null,
   LST_UPD_USER_ID      VARCHAR(15)            not null,
   constraint P_PK_SET_TYPE primary key (SET_TYPE)
);

