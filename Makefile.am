ACLOCAL_AMFLAGS = -I m4

SUBDIRS  = dbs krutils krcalc krparam krdb krdata krflow krengine \
		   krserver krcoordi krshell krtools krweb \
		   tests examples etc doc 

if ENABLE_PYTHON_BINDING
SUBDIRS += bindings/python
endif
if ENABLE_JAVA_BINDING
SUBDIRS += bindings/java
endif

AM_CPPFLAGS = -g -I$(includedir) -L$(libdir)

include_HEADERS          = kr_config.h

EXTRA_DIST = README LICENSE 

