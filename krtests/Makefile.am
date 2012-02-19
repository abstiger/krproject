
TEST_PROGS                      = 

noinst_PROGRAMS                 = $(TEST_PROGS)
progs_ldadd     = $(top_builddir)/krutils/libkrutils.la
progs_ldadd    += $(top_builddir)/krrules/libkrrules.la


TEST_PROGS                     += test_krrules_calc
test_krrules_calc_SOURCES       = test_krrules_calc.c
test_krrules_calc_LDADD         = $(progs_ldadd)
test_krrules_calc_CPPFLAGS      = -g -I$(top_builddir)/krrules


TEST_PROGS                     += test_krutils_zmalloc
test_krutils_zmalloc_SOURCES    = test_krutils_zmalloc.c
test_krutils_zmalloc_LDADD      = $(progs_ldadd)
test_krutils_zmalloc_CPPFLAGS   = -g -I$(top_builddir)/krutils

