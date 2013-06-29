EXTRA_DATA = krengine.i
krenginejardir = $(libdir)
krenginejar_DATA = $(krengine_jarfile)
lib_LTLIBRARIES  = libkrengine_java.la
libkrengine_java_la_SOURCES    = krengine_wrap.c krengine.i
libkrengine_java_la_LIBADD     = $(top_srcdir)/krutils/libkrutils.la
libkrengine_java_la_LIBADD    += $(top_srcdir)/dbs/libdbsbas.la
libkrengine_java_la_LIBADD    += $(top_srcdir)/dbs/dbs/libdbsdbs.la
libkrengine_java_la_LIBADD    += $(top_srcdir)/krshm/libkrshm.la
libkrengine_java_la_LIBADD    += $(top_srcdir)/krcalc/libkrcalc.la
libkrengine_java_la_LIBADD    += $(top_srcdir)/krdb/libkrdb.la
libkrengine_java_la_LIBADD    += $(top_srcdir)/krrule/libkrrule.la
libkrengine_java_la_LIBADD    += $(top_srcdir)/krengine/libkrengine.la
libkrengine_java_la_CPPFLAGS   = $(JNI_CPPFLAGS) 

krengine_wrap.c: krengine.i 
	@mkdir -p org/krproject
	${SWIG} -I${top_srcdir} -java -outdir org/krproject -package org.krproject -o $@ $<

krengine_jarfile = krengine-$(VERSION).jar

krengine-java.stamp: krengine_wrap.c
	@${JAVAC} org/krproject/*.java
	@date > $@

$(krengine_jarfile): krengine-java.stamp
	@$(JAR) cvf $(JARFLAGS) $(krengine_jarfile) org/krproject/*.class org/krproject/*java

clean-local:
	-rm -fr krengine_wrap.c *.jar *.stamp org

