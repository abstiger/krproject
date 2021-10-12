noinst_LTLIBRARIES       = libdbsdbs.la
libdbsdbs_la_SOURCES     = t_dbs001_param_cur.cfg \
                           t_dbs002_param_object_ins.cfg

#                          t_dbs501_record_ins.cfg \
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

