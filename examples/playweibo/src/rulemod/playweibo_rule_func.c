#include <Python.h>

int weibo_post_status(void *data, char *text)
{
    PyObject *client;

    client = Py_BuildValue("O", data);
    PyObject_CallMethod(client, "post_status", "(s)", text);
    Py_DECREF(client);
    return 0;
}
