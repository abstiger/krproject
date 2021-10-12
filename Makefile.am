include_HEADERS = kr_config.h

ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -g -I$(includedir) -L$(libdir)

SUBDIRS  = dbs krutils krcalc krparam \
		   krinput krdb krdata kroutput krengine \
		   krserver krshell krtools krweb \
		   tests examples etc doc 

if ENABLE_PYTHON_BINDING
SUBDIRS += bindings/python
endif
if ENABLE_JAVA_BINDING
SUBDIRS += bindings/java
endif

EXTRA_DIST = README.md LICENSE 

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = krengine.pc
