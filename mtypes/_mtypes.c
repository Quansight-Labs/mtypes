#include "_mtypes.h"

MTYPES_API static PyModuleDef mtypes_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "mtypes",
    .m_doc = "Creates a meta-type extending the Python type-system for array objects.",
    .m_size = -1,
};

MTYPES_API static int
PyMType_ArgParse(PyObject *args, PyObject *kwds, PyObject **args_out, PyObject **kwds_out, long *custom_out)
{
    static char *kwlist[] = {"", "", "", NULL};

    PyObject *name, *bases, *namespace;

    *args_out = NULL;
    *kwds_out = NULL;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OOO:mtype.__new__", kwlist,
                                     &name, &bases, &namespace))
        goto fail;

    *kwds_out = PyDict_New();
    if (*kwds_out == NULL)
        goto fail;

    *args_out = Py_BuildValue("(OOO)", name, bases, namespace);
    if (*args_out == NULL)
        goto fail;

    return 0;

fail:
    Py_XDECREF(*kwds_out);
    Py_XDECREF(*args_out);
    return -1;
}

MTYPES_API static int
PyMType_Type_init(PyObject *self_obj, PyObject *args, PyObject *kwds)
{
    PyObject *args_out, *kwds_out;
    long custom;
    if (PyMType_ArgParse(args, kwds, &args_out, &kwds_out, &custom) < 0)
        goto fail;

    PyMTypeObject* self = (PyMTypeObject *)self_obj;
    self->mt_data = NULL;
    self->box = NULL;
    self->unbox = NULL;
    self->mt_funcs = NULL;

    return self_obj->ob_type->tp_base->tp_init(self_obj, args_out, kwds_out);
fail:
    return -1;
}

MTYPES_API static PyObject *
PyMType_Type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyObject *args_out, *kwds_out;
    long custom;
    if (PyMType_ArgParse(args, kwds, &args_out, &kwds_out, &custom) < 0)
        goto fail;

    return type->tp_base->tp_new(type, args_out, kwds_out);

fail:
    return NULL;
}

PyMODINIT_FUNC
PyInit__mtypes(void)
{
    PyObject *m;
    if (PyType_Ready((PyTypeObject *)&PyMType_Type) < 0)
    {
        return NULL;
    }

    m = PyModule_Create(&mtypes_module);
    if (m == NULL)
    {
        return NULL;
    }

    Py_INCREF((PyObject *)&PyMType_Type);
    if (PyModule_AddObject(m, "mtype", (PyObject *)&PyMType_Type) < 0)
    {
    }

    m = PyModule_Create(&mtypes_module);
    if (m == NULL)
    {
        return NULL;
    }

    Py_INCREF((PyObject *)&PyMType_Type);
    if (PyModule_AddObject(m, "mtype", (PyObject *)&PyMType_Type) < 0)
    {
    }

    m = PyModule_Create(&mtypes_module);
    if (m == NULL)
    {
        return NULL;
    }

    Py_INCREF((PyObject *)&PyMType_Type);
    if (PyModule_AddObject(m, "mtype", (PyObject *)&PyMType_Type) < 0)
    {
        return NULL;
    }

    return m;
}
