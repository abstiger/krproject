noinst_PROGRAMS          = gensql
gensql_SOURCES           = gensql.c
gensql_CPPFLAGS          = @ORACLE_INCLUDE@
gensql_LDFLAGS           = @ORACLE_LDFLAGS@ @ORACLE_LIBS@

PROC=@ORACLE_PROC@

#lib_LTLIBRARIES = libdbsbas.la
#libdbsbas_la_CPPFLAGS = @ORACLE_INCLUDE@
#libdbsbas_la_LDFLAGS = @ORACLE_LDFLAGS@ @ORACLE_LIBS@
#libdbsbas_la_SOURCES = dbs_basopr.c dbs_basopr.h
##libdbsbas_la_DEPENDENCIES = dbs_basopr.h
#

EXTRA_DIST = gensql.pc dbs_basopr.pc dbs_basopr.h

.pc.c:
	@echo "Compiling [$@] ..."
	@$(PROC) hold_cursor=yes lines=yes code=ANSI_C iname=$<
	@rm -f $*.lis

maintainer-clean-local:
	@rm -f gensql.c
