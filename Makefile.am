ACLOCAL_AMFLAGS = -I m4
SUBDIRS = krutils dbs krshm krcalc krdb krrule krengine \
          krcoordi krserver krclient krtests

AM_CPPFLAGS = -g -I$(includedir) -L$(libdir)

include_HEADERS = kr_config.h

EXTRA_DIST = etc/krproject.env \
             etc/krcoordi.cfg \
             etc/krserver.cfg \
             krcalc/parser \
             interface \
             doc 

