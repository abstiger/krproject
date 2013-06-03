#include <Python.h>

int weibo_post_status(void *data, char *text)
{
    PyObject *client;

    client = Py_BuildValue("O", data);
    PyObject_CallMethod(client, "post_status", "(s)", text);
    Py_DECREF(client);
    return 0;
}

int weibo_repost_status(void *data, double id, char *text)
{
    PyObject *client;

    client = Py_BuildValue("O", data);
    PyObject_CallMethod(client, "repost_status", "(Ls)", (long long )id, text);
    Py_DECREF(client);
    return 0;
}

int weibo_post_comment(void *data, double id, char *text)
{
    PyObject *client;

    client = Py_BuildValue("O", data);
    PyObject_CallMethod(client, "post_comment", "(Ls)", (long long )id, text);
    Py_DECREF(client);
    return 0;
}

