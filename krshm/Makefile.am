lib_LTLIBRARIES        = libkrshm.la
libkrshm_la_SOURCES  = \
     kr_shm.h        \
     kr_shm.c        \
     kr_shm_sdi.h    \
     kr_shm_sdi.c    \
     kr_shm_ddi.h    \
     kr_shm_ddi.c    \
     kr_shm_hdi.h    \
     kr_shm_hdi.c    \
     kr_shm_rule.h   \
     kr_shm_rule.c
     
libkrshm_la_LIBADD    = $(top_srcdir)/krutils/libkrutils.la
libkrshm_la_LIBADD   += $(top_srcdir)/dbs/dbs/libdbsdbs.la
libkrshm_la_LDFLAGS   = 
libkrshm_la_CPPFLAGS  = 


bin_PROGRAMS          = krshm
krshm_SOURCES         = krshm.c
krshm_LDADD           = $(top_srcdir)/krutils/libkrutils.la
krshm_LDADD          += $(top_srcdir)/dbs/dbs/libdbsdbs.la
krshm_LDADD          += $(top_srcdir)/dbs/libdbsbas.la
krshm_LDADD          += $(top_srcdir)/krshm/libkrshm.la
krshm_CPPFLAGS        = 
krshm_LDFLAGS         = 

