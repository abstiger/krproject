if HAVE_DB2
BUILD_DBS = db2
DBS_INCLUDE   = @DB2_INCLUDE@
DBS_LDFLAGS   = @DB2_LDFLAGS@
DBS_LIBS      = @DB2_LIBS@
DB2           = @DB2CMD@
endif


if HAVE_ORACLE
BUILD_DBS = oracle
DBS_INCLUDE   = @ORACLE_INCLUDE@
DBS_LDFLAGS   = @ORACLE_LDFLAGS@
DBS_LIBS      = @ORACLE_LIBS@
PROC            = @ORACLE_PROC@
endif


GENSQL    = $(BUILD_DBS)/gensql
SUBDIRS   = $(BUILD_DBS) .

lib_LTLIBRARIES          = libdbs.la
libdbs_la_CPPFLAGS     = $(DBS_INCLUDE)
libdbs_la_LDFLAGS      = $(DBS_LDFLAGS) $(DBS_LIBS)
libdbs_la_SOURCES      = dbs_basopr.c \
                           t_dbs101_datasrc_def_cur.c \
                           t_dbs102_datasrc_index_cur.c \
                           t_dbs103_datasrc_field_cur.c \
                           t_dbs104_datasrc_field_cnt_sel.c \
                           t_dbs105_datasrc_field_def_sel.c \
                           t_dbs201_set_def_sel.c \
                           t_dbs202_set_cfg_cur.c \
                           t_dbs301_sdi_cur.c \
                           t_dbs302_ddi_cur.c \
                           t_dbs303_hdi_cur.c \
                           t_dbs304_hdi_day_sel.c \
                           t_dbs305_hdi_mon_sel.c \
                           t_dbs306_hdi_flag_sel.c \
                           t_dbs401_rule_cur.c

EXTRA_DIST               = t_dbs101_datasrc_def_cur.cfg \
                           t_dbs102_datasrc_index_cur.cfg \
                           t_dbs103_datasrc_field_cur.cfg \
                           t_dbs104_datasrc_field_cnt_sel.cfg \
                           t_dbs105_datasrc_field_def_sel.cfg \
                           t_dbs201_set_def_sel.cfg \
                           t_dbs202_set_cfg_cur.cfg \
                           t_dbs301_sdi_cur.cfg \
                           t_dbs302_ddi_cur.cfg \
                           t_dbs303_hdi_cur.cfg \
                           t_dbs304_hdi_day_sel.cfg \
                           t_dbs305_hdi_mon_sel.cfg \
                           t_dbs306_hdi_flag_sel.cfg \
                           t_dbs401_rule_cur.cfg

clean-local:
	-rm -f $(libdbs_la_SOURCES)

if HAVE_DB2	

dbs_basopr.c:$(BUILD_DBS)/dbs_basopr.sqc
	@cp -f $(BUILD_DBS)/$*.h $*.h	
	@cp -f $(BUILD_DBS)/$*.sqc $*.sqc
	@echo "db2 precompiling [$@] ..."
	@rm -f $*.bnd
	@$(DB2) connect to $(DBNAME)
	@$(DB2) prep $*.sqc bindfile 
	@$(DB2) bind $*.bnd
	@$(DB2) connect reset
	@$(DB2) terminate	

.cfg.c:
	@echo "gensql db2 [$@] ..."
	@$(GENSQL) $*	
	@echo "db2 precompiling [$@] ..."
	@rm -f $*.bnd
	@$(DB2) connect to $(DBNAME)
	@$(DB2) prep $*.sqc bindfile 
	@$(DB2) bind $*.bnd
	@$(DB2) connect reset
	@$(DB2) terminate

endif

if HAVE_ORACLE	

dbs_basopr.c:$(BUILD_DBS)/dbs_basopr.pc
	@cp -f $(BUILD_DBS)/$*.h $*.h
	@cp -f $(BUILD_DBS)/$*.pc $*.pc
	@$(PROC) hold_cursor=yes lines=yes code=ANSI_C include=$(top_srcdir) iname=$*.pc
	@rm -f $*.lis tp*

.cfg.c:
	@echo "gensql oracle [$@] ..."
	@$(GENSQL) $*	
	@echo "oracle precompiling [$@] ..."
	@$(PROC) hold_cursor=yes lines=yes code=ANSI_C include=$(top_srcdir) iname=$*.pc
	@rm -f $*.lis tp*
	
endif
