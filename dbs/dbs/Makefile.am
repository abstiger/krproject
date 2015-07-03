noinst_LTLIBRARIES       = libdbsdbs.la
libdbsdbs_la_SOURCES     = t_dbs001_param_cur.cfg \
                           t_dbs002_param_object_ins.cfg



#                          t_dbs101_input_cur.cfg \
                           t_dbs102_input_field_cur.cfg \
                           t_dbs104_input_field_cnt_sel.cfg \
                           t_dbs105_input_field_def_sel.cfg \
                           t_dbs111_output_cur.cfg \
                           t_dbs112_output_field_cur.cfg \
                           t_dbs131_datadic_def_sel.cfg \
                           t_dbs132_datadic_cfg_cur.cfg \
                           t_dbs201_index_cur.cfg \
                           t_dbs202_index_table_cur.cfg \
                           t_dbs301_set_def_cur.cfg \
                           t_dbs302_set_cfg_cur.cfg \
                           t_dbs303_set_cfg_ins.cfg \
                           t_dbs311_sdi_cur.cfg \
                           t_dbs321_ddi_cur.cfg \
                           t_dbs331_hdi_cur.cfg \
                           t_dbs401_rule_cur.cfg \
                           t_dbs402_group_cur.cfg \
                           t_dbs501_record_ins.cfg \
                           t_dbs502_record_proctime_cur.cfg \
                           t_dbs503_record_transtime_cur.cfg \
                           t_dbs504_hdi_day_sel.cfg \
                           t_dbs505_hdi_mon_sel.cfg \
                           t_dbs506_hdi_flag_sel.cfg 

libdbsdbs_la_CPPFLAGS    = -I..
libdbsdbs_la_LDFLAGS     = -lodbc 

RAPODBC=../rapodbc

.cfg.c:
	@echo "rapodbc [$@] ..." 
	$(RAPODBC) $*

clean-local:
	-rm -f *.c *.h
