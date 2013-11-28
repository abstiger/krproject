RAPODBC=../rapodbc

lib_LTLIBRARIES          = libdbsdbs.la
libdbsdbs_la_CPPFLAGS    = -I..
libdbsdbs_la_LDFLAGS     = -lodbc 

libdbsdbs_la_SOURCES     = t_dbs101_datasrc_def_cur.cfg \
                           t_dbs102_datasrc_index_cur.cfg \
                           t_dbs103_datasrc_field_cur.cfg \
                           t_dbs104_datasrc_field_cnt_sel.cfg \
                           t_dbs105_datasrc_field_def_sel.cfg \
                           t_dbs201_set_def_cur.cfg \
                           t_dbs202_set_cfg_cur.cfg \
                           t_dbs301_sdi_cur.cfg \
                           t_dbs302_ddi_cur.cfg \
                           t_dbs303_hdi_cur.cfg \
                           t_dbs304_hdi_day_sel.cfg \
                           t_dbs305_hdi_mon_sel.cfg \
                           t_dbs306_hdi_flag_sel.cfg \
                           t_dbs401_rule_cur.cfg \
                           t_dbs402_group_cur.cfg

.cfg.c:
	@echo "rapodbc [$@] ..." 
	$(RAPODBC) $*

clean-local:
	-rm -f *.c *.h
