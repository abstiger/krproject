pkgpython_PYTHON = __init__.py krengine.py
pkgpyexec_LTLIBRARIES   = _krengine.la
nodist__krengine_la_SOURCES    = krengine_wrap.c 
_krengine_la_SOURCES    = krengine.i
_krengine_la_LIBADD     = $(top_srcdir)/krengine/libkrengine.la
_krengine_la_LIBADD    += $(PYTHON_EXTRA_LDFLAGS)
_krengine_la_CPPFLAGS   = $(PYTHON_CPPFLAGS) 
_krengine_la_LDFLAGS    = -module $(PYTHON_LDFLAGS) $(PYTHON_EXTRA_LDFLAGS)

krengine_wrap.c: krengine.i 
	${SWIG} -I${top_srcdir} -python -o $@ $<
	@touch __init__.py

clean-local:
	-rm -f krengine_wrap.c krengine.py

