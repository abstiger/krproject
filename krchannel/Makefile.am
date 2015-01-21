lib_LTLIBRARIES           = libkrchannel.la
include_HEADERS           = kr_channel.h 

libkrchannel_la_SOURCES   = kr_channel.h \
							kr_channel.c \
							kr_transport.h \
							kr_transport.c \
							kr_protocol.h \
							kr_protocol.c 
     
libkrchannel_la_LIBADD    = $(top_srcdir)/krutils/libkrutils.la
libkrchannel_la_LIBADD   += $(top_srcdir)/krengine/libkrengine.la
libkrchannel_la_CPPFLAGS  = 
libkrchannel_la_LDFLAGS   = 
