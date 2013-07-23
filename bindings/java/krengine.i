%module krengine
%{
#include "krengine/kr_engine.h"
%}

%include "krengine/kr_engine.h"

%pragma(java) jniclasscode=%{
    static {
        System.out.println("jni"+System.getProperty("java.library.path"));
        try {
            System.loadLibrary("dbsbas");
            System.loadLibrary("dbsdbs");
            System.loadLibrary("krutils");
            System.loadLibrary("krdb");
            System.loadLibrary("krshm");
            System.loadLibrary("krcalc");
            System.loadLibrary("krrule");
            System.loadLibrary("krengine");
            System.loadLibrary("krengine_java");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library failed to load. \n" + e);
            System.exit(1);
        }
    }
%}
