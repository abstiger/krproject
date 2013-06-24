lib_LTLIBRARIES          = libkrengine.la
include_HEADERS          = kr_engine.h

libkrengine_la_SOURCES   = \
						   kr_engine.h \
						   kr_engine.c \
						   kr_engine_thread.c 
     
libkrengine_la_LIBADD    = $(top_srcdir)/krutils/libkrutils.la
libkrengine_la_LIBADD   += $(top_srcdir)/dbs/libdbsbas.la
libkrengine_la_LIBADD   += $(top_srcdir)/dbs/dbs/libdbsdbs.la
libkrengine_la_LIBADD   += $(top_srcdir)/krshm/libkrshm.la
libkrengine_la_LIBADD   += $(top_srcdir)/krcalc/libkrcalc.la
libkrengine_la_LIBADD   += $(top_srcdir)/krdb/libkrdb.la
libkrengine_la_LIBADD   += $(top_srcdir)/krrule/libkrrule.la
libkrengine_la_LDFLAGS   = -lpthread
libkrengine_la_CPPFLAGS  = 
