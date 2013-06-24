bin_PROGRAMS       = krserver
krserver_SOURCES   = kr_server.h \
                     kr_server.c \
                     kr_server_message.h \
                     kr_server_message.c \
                     kr_server_handler.c \
                     kr_server_config.c 
     
krserver_LDADD     = $(top_srcdir)/krutils/libkrutils.la
krserver_LDADD    += $(top_srcdir)/dbs/libdbsbas.la
krserver_LDADD    += $(top_srcdir)/dbs/dbs/libdbsdbs.la
krserver_LDADD    += $(top_srcdir)/krshm/libkrshm.la
krserver_LDADD    += $(top_srcdir)/krcalc/libkrcalc.la
krserver_LDADD    += $(top_srcdir)/krdb/libkrdb.la
krserver_LDADD    += $(top_srcdir)/krrule/libkrrule.la
krserver_LDADD    += $(top_srcdir)/krengine/libkrengine.la
krserver_LDFLAGS   = 
krserver_CPPFLAGS  = 
