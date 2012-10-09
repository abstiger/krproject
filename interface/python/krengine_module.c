#include "kr_engine.h"
#include "Python.h"


static PyObject *
pykrengine_external_startup(PyObject *self, PyObject *args)
{
    char *logpath;
    char *logname;
    int loglevel;
    int ret;
    
    if (!PyArg_ParseTuple(args, "ssi", &logpath, &logname, &loglevel))
        return NULL;
    ret =kr_engine_external_startup(logpath, logname, loglevel);
    return Py_BuildValue("i", ret);
}


static PyObject *
pykrengine_external_shutdown(PyObject *self, PyObject *args)
{
    T_KREngine *krengine;

    kr_engine_external_shutdown();
    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject *
pykrengine_startup(PyObject *self, PyObject *args)
{
    int shmkey;
    char *dbname;
    int hdicachesize;
    int threadcnt;
    
    if (!PyArg_ParseTuple(args, "isii", &shmkey, &dbname, &hdicachesize, &threadcnt))
        return NULL;
    T_KREngine *krengine = \
        kr_engine_startup(shmkey, dbname, hdicachesize, threadcnt);
    return Py_BuildValue("l", krengine);
}

static PyObject *
pykrengine_run(PyObject *self, PyObject *args)
{
    T_KREngine *krengine;
    int oprcode;
    int datasrc;
    char *msgbuf;
    int ret;
    
    if (!PyArg_ParseTuple(args, "liis", &krengine, &oprcode, &datasrc, &msgbuf))
        return NULL;
    ret=kr_engine_run(krengine, oprcode, datasrc, msgbuf);
    return Py_BuildValue("i", ret);
}


static PyObject *
pykrengine_shutdown(PyObject *self, PyObject *args)
{
    T_KREngine *krengine;

    if (!PyArg_ParseTuple(args, "l", &krengine))
        return NULL;

    kr_engine_shutdown(krengine);
    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject *
pykrengine_info(PyObject *self, PyObject *args)
{
    T_KREngine *krengine;
    char *krinfo;
    
    if (!PyArg_ParseTuple(args, "l", &krengine))
        return NULL;
    krinfo=kr_engine_info(krengine);
    PyObject *info=PyString_FromString(krinfo);
    return info;
}


static PyMethodDef KREngineMethods[] = {
    {"external_startup",  pykrengine_external_startup, METH_VARARGS,""},
    {"external_shutdown",  pykrengine_external_shutdown, METH_VARARGS,""},
    {"startup",  pykrengine_startup, METH_VARARGS,""},
    {"run",  pykrengine_run, METH_VARARGS,""},
    {"shutdown",  pykrengine_shutdown, METH_VARARGS,""},
    {"info",  pykrengine_info, METH_VARARGS,""},
    {NULL, NULL, 0, NULL}
};


PyMODINIT_FUNC
init_krengine(void)
{
    (void) Py_InitModule("_krengine", KREngineMethods);
}

