bin_PROGRAMS       = krserver
krserver_SOURCES   = kr_server.h \
                     kr_server.c \
                     kr_server_config.h \
                     kr_server_config.c \
                     kr_server_handle.c 
     
krserver_LDADD     = $(top_srcdir)/krutils/libkrutils.la
krserver_LDADD    += $(top_srcdir)/dbs/libdbsbas.la
krserver_LDADD    += $(top_srcdir)/dbs/dbs/libdbsdbs.la
krserver_LDADD    += $(top_srcdir)/krparam/libkrparam.la
krserver_LDADD    += $(top_srcdir)/krcalc/libkrcalc.la
krserver_LDADD    += $(top_srcdir)/krdb/libkrdb.la
krserver_LDADD    += $(top_srcdir)/krdata/libkrdata.la
krserver_LDADD    += $(top_srcdir)/krengine/libkrengine.la
krserver_LDADD    += $(top_srcdir)/krchannel/libkrchannel.la
krserver_LDFLAGS   = 
krserver_CPPFLAGS  = 

