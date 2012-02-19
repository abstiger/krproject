lib_LTLIBRARIES        = libkrrules.la
libkrrules_la_SOURCES  = \
     kr_dict.c \
     kr_dict.h \
     kr_regex.c \
     kr_regex.h \
     kr_lexer.c    \
     kr_lexer.h    \
     kr_parser.c   \
     kr_parser.h   \
     kr_ruletree.c \
     kr_ruletree.h \
     kr_rules.c \
     kr_rules.h 
libkrrules_la_LIBADD   = $(top_builddir)/krutils/libkrutils.la
libkrrules_la_CPPFLAGS = -I$(top_builddir)/krutils -L$(top_builddir)/krutils

include_HEADERS = kr_rules.h 
