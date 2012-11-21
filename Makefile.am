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
			 examples \
             doc/krproject_database.doc \
             doc/krproject_db2.sql \
             doc/krproject_oracle.sql \
             doc/krproject_mysql.sql \
             doc/krproject_sqlite3.sql 


