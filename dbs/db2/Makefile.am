noinst_PROGRAMS             = gensql
gensql_SOURCES           = gensql.c
gensql_CPPFLAGS          = @DB2_INCLUDE@
gensql_LDFLAGS           = @DB2_LDFLAGS@ @DB2_LIBS@

DB2=@DB2CMD@

#lib_LTLIBRARIES = libdbsbas.la
#libdbsbas_la_CPPFLAGS = @DB2_INCLUDE@
#libdbsbas_la_LDFLAGS = @DB2_LDFLAGS@ @DB2_LIBS@
#libdbsbas_la_SOURCES = dbs_basopr.c dbs_basopr.h
##libdbsbas_la_DEPENDENCIES = dbs_basopr.h
#

EXTRA_DIST = gensql.sqc dbs_basopr.sqc dbs_basopr.h

.sqc.c:
	@echo "db2 precompiling [$@] ..."
	@$(DB2) connect to $(DBNAME)
	@$(DB2) prep $< bindfile
	@$(DB2) bind $*.bnd
	@$(DB2) connect reset
