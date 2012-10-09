##
## Written By Tiger 
## Check Embedding SQL-C Complile Environment
##

AC_DEFUN([AC_FIND_FILE],
[
$3=no
for i in $2; do
	for j in $1; do
		if test -r "$i/$j"; then
			$3=$i
			break 2
		fi
	done
done
])


## Oracle

AC_DEFUN([AC_CHECK_ORACLE],
[
AC_MSG_CHECKING(For Oracle support)

AM_CONDITIONAL(HAVE_ORACLE, false)

# Check if we should compile with Oracle support
AC_ARG_WITH([oracle],
            AC_HELP_STRING([--with-oracle], [Include Oracle support.(Specify ORACLE_HOME usually)]),
            [ac_oracle_home=$withval], [ac_oracle_home=no]
)
AC_ARG_WITH([oracle-incdir],
            AC_HELP_STRING([--with-oracle-incdir], [Specifies where the Oracle include files are.]),
            [ac_oracle_incdir=$withval], [ac_oracle_incdir=no]
)
AC_ARG_WITH([oracle-libdir],
            AC_HELP_STRING([--with-oracle-libdir], [Specifies where the Oracle libraries are.]),
            [ac_oracle_libdir=$withval], [ac_oracle_libdir=no]
)

if test "$ac_oracle_home" != "no"; then
	#check include and library directory
	if test "$ac_oracle_incdir" = "no" || test "$ac_oracle_libdir" = "no"; then
		oracle_incdirs="$ac_oracle_home/precomp/public $ORACLE_HOME/precomp/public"
		AC_FIND_FILE(sqlca.h, $oracle_incdirs, ac_oracle_incdir)
		oracle_libdirs="$ac_oracle_home/lib $ORACLE_HOME/lib"
		AC_FIND_FILE(libclntsh.so, $oracle_libdirs, ac_oracle_libdir)
		if test "$ac_oracle_incdir" = "no"; then
			AC_MSG_RESULT(no)
			AC_MSG_ERROR([Invalid Oracle directory - include files not found.])
		fi
		if test "$ac_oracle_libdir" = "no"; then
			AC_MSG_RESULT(no)
			AC_MSG_ERROR([Invalid Oracle directory - libraries not found.])
		fi
	fi
	AC_MSG_RESULT([yes: libs in $ac_oracle_libdir, headers in $ac_oracle_incdir])
	
	#check oracle proc precompiler
	AC_CHECK_PROG([ORACLE_PROC], [proc], [proc], no)
	if test "$ORACLE_PROC" = "no"; then
		AC_MSG_ERROR([You Need to add Oracle-bin-dir where "proc" exists to your PATH.])
	fi
	
	AM_CONDITIONAL(HAVE_ORACLE, true)

dnl	ORACLE_LIBS="-lclntsh"
    ORACLE_LIBS="-lclntsh -lclient10"
	ORACLE_INCLUDE="-I$ac_oracle_incdir"
	ORACLE_LDFLAGS=-L$ac_oracle_libdir
	
	AC_SUBST(ORACLE_LIBS)
	AC_SUBST(ORACLE_INCLUDE)
	AC_SUBST(ORACLE_LDFLAGS)
else
	AC_MSG_RESULT(no)
fi
])


## DB2

AC_DEFUN([AC_CHECK_DB2],
[
AC_MSG_CHECKING(For DB2 support)

AM_CONDITIONAL(HAVE_DB2, false)

# Check if we should compile with DB2 support
AC_ARG_WITH([db2],
            AC_HELP_STRING([--with-db2], [Include DB2 support.(Specify DB2_HOME usually)]),
            [ac_db2_home=$withval], [ac_db2_home=no]
)
AC_ARG_WITH([db2-incdir],
            AC_HELP_STRING([--with-db2-incdir], [Specifies where the DB2 include files are.]),
            [ac_db2_incdir=$withval], [ac_db2_incdir=no]
)
AC_ARG_WITH([db2-libdir],
            AC_HELP_STRING([--with-db2-libdir], [Specifies where the DB2 libraries are.]),
            [ac_db2_libdir=$withval], [ac_db2_libdir=no]
)

if test "$ac_db2_home" != "no"; then
    
    #check include and library directory
	if test "$ac_db2_incdir" = "no" || test "$ac_db2_libdir" = "no"; then
		db2_incdirs="$ac_db2_home/include $DB2_HOME/include /opt/ibm/db2/include"
		AC_FIND_FILE(sqlca.h, $db2_incdirs, ac_db2_incdir)
		db2_libdirs="$ac_db2_home/lib $DB2_HOME/lib /opt/ibm/db2/lib"
		AC_FIND_FILE(libdb2.so, $db2_libdirs, ac_db2_libdir)
		if test "$ac_db2_incdir" = "no"; then
			AC_MSG_RESULT(no)
			AC_MSG_ERROR([Invalid DB2 directory - include files not found.])
		fi
		if test "$ac_db2_libdir" = "no"; then
			AC_MSG_RESULT(no)
			AC_MSG_ERROR([Invalid DB2 directory - libraries not found.])
		fi
	fi
	AC_MSG_RESULT([yes: libs in $ac_db2_libdir, headers in $ac_db2_incdir])

    #check db2 command
	AC_CHECK_PROG([DB2CMD], [db2], [db2], no)
	if test "$DB2CMD" = "no"; then
		AC_MSG_ERROR([You Need to add db2-bin-dir where "db2" exists to your PATH.])
	fi

	AM_CONDITIONAL(HAVE_DB2, true)

	DB2_LIBS="-ldb2"
	DB2_INCLUDE="-I$ac_db2_incdir"
	DB2_LDFLAGS="-L$ac_db2_libdir"
	
	AC_SUBST(DB2_LIBS)
	AC_SUBST(DB2_INCLUDE)
	AC_SUBST(DB2_LDFLAGS)
else
	AC_MSG_RESULT(no)
fi
])
