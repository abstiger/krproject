create table KR_TBL_DATADIC_CFG (
REC_ID               INTEGER                        not null,
DATADIC_ID           INTEGER                        not null,
DATADIC_KEY          VARCHAR(30)                    not null,
DATADIC_VALUE        VARCHAR(200)                   not null,
primary key (REC_ID)
);

create unique index IDX_DATADIC_CFG on KR_TBL_DATADIC_CFG (
DATADIC_ID ASC,
DATADIC_KEY ASC
);

create table KR_TBL_DATADIC_DEF (
DATADIC_ID           INTEGER                        not null,
DATADIC_NAME         VARCHAR(30)                    not null,
DATADIC_DESC         VARCHAR(100)                   not null,
REC_CRET_DTTM        TIMESTAMP                      not null,
LST_UPD_DTTM         TIMESTAMP                      not null,
LST_UPD_USER_ID      VARCHAR(15)                    not null,
primary key (DATADIC_ID)
);

create table KR_TBL_DATASRC (
DATASRC_ID           INTEGER                        not null,
DATASRC_NAME         VARCHAR(30)                    not null,
DATASRC_DESC         VARCHAR(100)                   not null,
DATASRC_FORMAT       VARCHAR(10)                    not null,
DATASRC_USAGE        CHAR(1)                        not null,
SIZE_KEEP_MODE       CHAR(1)                        not null,
SIZE_KEEP_VALUE      INTEGER                        not null,
primary key (DATASRC_ID)
);

create table KR_TBL_DATASRC_FIELD (
REC_ID               INTEGER                        not null,
DATASRC_ID           INTEGER                        not null,
FIELD_ID             INTEGER                        not null,
FIELD_NAME           VARCHAR(30)                    not null,
FIELD_DESC           VARCHAR(100)                   not null,
FIELD_TYPE           CHAR(1)                        not null,
FIELD_LENGTH         NUMERIC(10,0)                  not null,
FIELD_DATADIC        INTEGER                        not null,
primary key (REC_ID)
);

create unique index IDX_DATASRC_FIELD on KR_TBL_DATASRC_FIELD (
DATASRC_ID ASC,
FIELD_ID ASC
);

create table KR_TBL_DATASRC_INDEX (
REC_ID               INTEGER                        not null,
DATASRC_ID           INTEGER                        not null,
INDEX_ID             INTEGER                        not null,
INDEX_NAME           VARCHAR(30)                    not null,
INDEX_DESC           VARCHAR(100)                   not null,
INDEX_TYPE           CHAR(1)                        not null,
INDEX_FIELD_ID       INTEGER                        not null,
SORT_FIELD_ID        INTEGER                        not null,
primary key (REC_ID)
);

create unique index IDX_DATASRC_INDEX on KR_TBL_DATASRC_INDEX (
DATASRC_ID ASC,
INDEX_ID ASC
);

create table KR_TBL_DDI (
DDI_ID               INTEGER                        not null,
DDI_NAME             VARCHAR(30)                    not null,
DDI_DESC             VARCHAR(100)                   not null,
DDI_TYPE             CHAR(1)                        not null,
DDI_VALUE_TYPE       CHAR(1)                        not null,
DDI_AGGR_FUNC        VARCHAR(50)                    not null,
DDI_FREE_FUNC        VARCHAR(50)                    not null,
STATISTICS_DATASRC   INTEGER                        not null,
STATISTICS_INDEX     INTEGER                        not null,
STATISTICS_FIELD     INTEGER                        not null,
STATISTICS_TYPE      CHAR(1)                        not null,
STATISTICS_VALUE     INTEGER                        not null,
STATISTICS_COUNT     INTEGER                        not null,
DDI_FILTER_FORMAT    CHAR(1)                        not null,
DDI_FILTER_STRING    VARCHAR(500)                   not null,
STATISTICS_METHOD    CHAR(1)                        not null,
DDI_STATUS           CHAR(1)                        not null,
REC_CRET_DTTM        TIMESTAMP                      not null,
LST_UPD_DTTM         TIMESTAMP                      not null,
LST_UPD_USER_ID      VARCHAR(15)                    not null,
primary key (DDI_ID)
);

create table KR_TBL_DYNDATA_DAY (
DATA_DATE            CHAR(8)                        not null,
DATA_OBJECT          VARCHAR(30)                    not null,
DATA_ID              INTEGER                        not null,
DATA_VALUE           NUMERIC(20,2)                  not null,
primary key (DATA_DATE, DATA_OBJECT, DATA_ID)
);

create table KR_TBL_DYNDATA_FLAG (
DATA_OBJECT          VARCHAR(30)                    not null,
DATA_ID              INTEGER                        not null,
DATA_FLAG            VARCHAR(200)                   not null,
primary key (DATA_OBJECT, DATA_ID)
);

create table KR_TBL_DYNDATA_MON (
DATA_MONTH           CHAR(6)                        not null,
DATA_OBJECT          VARCHAR(30)                    not null,
DATA_ID              INTEGER                        not null,
DATA_VALUE           NUMERIC(20,2)                  not null,
primary key (DATA_MONTH, DATA_OBJECT, DATA_ID)
);

create table KR_TBL_GROUP (
GROUP_ID             INTEGER                        not null,
GROUP_NAME           VARCHAR(30)                    not null,
GROUP_DESC           VARCHAR(100)                   not null,
GROUP_CALC_FORMAT    CHAR(1)                        not null,
GROUP_CALC_STRING    VARCHAR(500)                   not null,
GROUP_FUNC           VARCHAR(50)                    not null,
GROUP_BLOCK_LEFT     INTEGER                        not null,
GROUP_BLOCK_RIGHT    INTEGER                        not null,
GROUP_ALERT_LEFT     INTEGER                        not null,
GROUP_ALERT_RIGHT    INTEGER                        not null,
GROUP_STATUS         CHAR(1)                        not null,
REC_CRET_DTTM        TIMESTAMP                      not null,
LST_UPD_DTTM         TIMESTAMP                      not null,
LST_UPD_USER_ID      VARCHAR(15)                    not null,
primary key (GROUP_ID)
);

create table KR_TBL_GROUP_RULE (
REC_ID               INTEGER                        not null,
GROUP_ID             INTEGER                        not null,
RULE_ID              INTEGER                        not null,
RULE_NAME            VARCHAR(30)                    not null,
RULE_WEIGHT          INTEGER                        not null,
RULE_STATUS          CHAR(1)                        not null,
REC_CRET_DTTM        TIMESTAMP                      not null,
LST_UPD_DTTM         TIMESTAMP                      not null,
LST_UPD_USER_ID      VARCHAR(15)                    not null,
primary key (REC_ID)
);

create unique index IDX_GROUP_RULE on KR_TBL_GROUP_RULE (
GROUP_ID ASC,
RULE_ID ASC
);

create table KR_TBL_HDI (
HDI_ID               INTEGER                        not null,
HDI_NAME             VARCHAR(30)                    not null,
HDI_DESC             VARCHAR(100)                   not null,
HDI_TYPE             CHAR(1)                        not null,
HDI_VALUE_TYPE       CHAR(1)                        not null,
HDI_AGGR_FUNC        VARCHAR(50)                    not null,
HDI_FREE_FUNC        VARCHAR(50)                    not null,
STATISTICS_DATASRC   INTEGER                        not null,
STATISTICS_INDEX     INTEGER                        not null,
STATISTICS_TYPE      CHAR(1)                        not null,
STATISTICS_VALUE     INTEGER                        not null,
STATISTICS_METHOD    CHAR(1)                        not null,
HDI_STATUS           CHAR(1)                        not null,
REC_CRET_DTTM        TIMESTAMP                      not null,
LST_UPD_DTTM         TIMESTAMP                      not null,
LST_UPD_USER_ID      VARCHAR(15)                    not null,
primary key (HDI_ID)
);

create table KR_TBL_RULE (
RULE_ID              INTEGER                        not null,
RULE_NAME            VARCHAR(30)                    not null,
RULE_DESC            VARCHAR(100)                   not null,
RULE_DATASRC         INTEGER                        not null,
RULE_CALC_FORMAT     CHAR(1)                        not null,
RULE_CALC_STRING     VARCHAR(500)                   not null,
RULE_TYPE            CHAR(1)                        not null,
RULE_FUNC            VARCHAR(50)                    not null,
RULE_WEIGHT          INTEGER                        not null,
RULE_STATUS          CHAR(1)                        not null,
REC_CRET_DTTM        TIMESTAMP                      not null,
LST_UPD_DTTM         TIMESTAMP                      not null,
LST_UPD_USER_ID      VARCHAR(15)                    not null,
primary key (RULE_ID)
);

create table KR_TBL_SDI (
SDI_ID               INTEGER                        not null,
SDI_NAME             VARCHAR(30)                    not null,
SDI_DESC             VARCHAR(100)                   not null,
SDI_TYPE             CHAR(1)                        not null,
SDI_VALUE_TYPE       CHAR(1)                        not null,
SDI_AGGR_FUNC        VARCHAR(50)                    not null,
SDI_FREE_FUNC        VARCHAR(50)                    not null,
STATISTICS_DATASRC   INTEGER                        not null,
STATISTICS_INDEX     INTEGER                        not null,
STATISTICS_FIELD     INTEGER                        not null,
STATISTICS_LOCATION  INTEGER                        not null,
LOCATION_PROPERTY    CHAR(1)                        not null,
SDI_FILTER_FORMAT    CHAR(1)                        not null,
SDI_FILTER_STRING    VARCHAR(500)                   not null,
SDI_STATUS           CHAR(1)                        not null,
REC_CRET_DTTM        TIMESTAMP                      not null,
LST_UPD_DTTM         TIMESTAMP                      not null,
LST_UPD_USER_ID      VARCHAR(15)                    not null,
primary key (SDI_ID)
);

create table KR_TBL_SET_CFG (
REC_ID               INTEGER                        not null,
SET_ID               INTEGER                        not null,
ELEMENT_VALUE        VARCHAR(200)                   not null,
ELEMENT_DESC         VARCHAR(200)                   not null,
primary key (REC_ID)
);

create unique index IDX_SET_CFG on KR_TBL_SET_CFG (
SET_ID ASC,
ELEMENT_VALUE ASC
);

create table KR_TBL_SET_DEF (
SET_ID               INTEGER                        not null,
SET_NAME             VARCHAR(30)                    not null,
SET_DESC             VARCHAR(100)                   not null,
SET_USAGE            CHAR(2)                        not null,
SET_TYPE             CHAR(2)                        not null,
ELEMENT_TYPE         CHAR(1)                        not null,
ELEMENT_LENGTH       INTEGER                        not null,
REC_CRET_DTTM        TIMESTAMP                      not null,
LST_UPD_DTTM         TIMESTAMP                      not null,
LST_UPD_USER_ID      VARCHAR(15)                    not null,
primary key (SET_ID)
);

create table KR_TBL_SET_TYPE_DEF (
SET_TYPE             CHAR(2)                        not null,
SET_TYPE_NAME        VARCHAR(30)                    not null,
SET_TYPE_DESC        VARCHAR(100)                   not null,
ELEMENT_TYPE         CHAR(1)                        not null,
ELEMENT_LENGTH       INTEGER                        not null,
REC_CRET_DTTM        CHAR(17)                       not null,
LST_UPD_DTTM         CHAR(17)                       not null,
LST_UPD_USER_ID      VARCHAR(15)                    not null,
primary key (SET_TYPE)
);

