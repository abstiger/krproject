#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([krproject], [1.0.0], [huzai9999999990@163.com])
AC_CONFIG_HEADERS([kr_config.h])
AC_CONFIG_SRCDIR([krutils])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

LT_INIT(disable_static)

# Check if we will compile with Oracle
#AC_CHECK_ORACLE
#AC_CHECK_DB2

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_SIZEOF(void *)
AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(int64_t)


# Checks for library functions.
AC_CHECK_FUNCS(epoll_ctl kqueue poll select)

AC_CONFIG_FILES([
Makefile 
krutils/Makefile 
dbs/Makefile 
dbs/dbs/Makefile 
krshm/Makefile 
krcalc/Makefile 
krdb/Makefile 
krrule/Makefile 
krengine/Makefile 
krcoordi/Makefile
krserver/Makefile
krclient/Makefile
krtests/Makefile
etc/Makefile 
doc/Makefile 
])
AC_OUTPUT
