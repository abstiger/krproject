lib_LTLIBRARIES          = libkrengine.la
include_HEADERS          = kr_engine.h

libkrengine_la_SOURCES   = kr_engine.h \
						   kr_engine.c \
						   kr_engine_context.h \
						   kr_engine_context.c \
						   kr_engine_handle.c 
     
libkrengine_la_LIBADD    = $(top_srcdir)/dbs/libdbsbas.la
libkrengine_la_LIBADD   += $(top_srcdir)/dbs/dbs/libdbsdbs.la
libkrengine_la_LIBADD   += $(top_srcdir)/krutils/libkrutils.la
libkrengine_la_LIBADD   += $(top_srcdir)/krparam/libkrparam.la
libkrengine_la_LIBADD   += $(top_srcdir)/krcalc/libkrcalc.la
libkrengine_la_LIBADD   += $(top_srcdir)/krdb/libkrdb.la
libkrengine_la_LIBADD   += $(top_srcdir)/krdata/libkrdata.la
libkrengine_la_LIBADD   += $(top_srcdir)/krflow/libkrflow.la
libkrengine_la_LIBADD   += $(top_srcdir)/kriface/libkriface.la
libkrengine_la_CPPFLAGS  = 
libkrengine_la_LDFLAGS   = 
