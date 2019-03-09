#include "_mtypes.h"

static PyModuleDef mtypes_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "mtypes",
    .m_doc = "Creates a meta-type extending the Python type-system for array objects.",
    .m_size = -1,
};

static int
PyMType_ArgParse(PyObject *args, PyObject *kwds, PyObject **args_out, PyObject **kwds_out, long *custom_out) {
    static char *kwlist[] = {"", "", "", NULL};

    PyObject *name, *bases, *namespace;
    
    *args_out = NULL;
    *kwds_out = NULL;
    PyObject* namespace_new = NULL;
    PyObject* key = NULL;
    PyObject* custom_obj = NULL;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OOO:mtype.__new__", kwlist,
                                        &name, &bases, &namespace))
        goto fail;

    *kwds_out = PyDict_New();
    if (*kwds_out == NULL)
        goto fail;

    namespace_new = PyDict_Copy(namespace);
    if (namespace_new == NULL)
        goto fail;

    key = PyUnicode_FromString("custom");
    if (key == NULL)
        goto fail;

    custom_obj = PyDict_GetItemWithError(namespace_new, key);
    if (custom_obj == NULL) {
        if (PyErr_Occurred() == NULL)
            PyErr_SetString(PyExc_ValueError, "mtype.__new__ expects a \"custom\" key in its third argument.");
        goto fail;
    }

    *custom_out = PyLong_AsLong(custom_obj);
    if (PyErr_Occurred() != NULL)
        goto fail;

    if (PyDict_DelItem(namespace_new, key) < 0)
        goto fail;

    *args_out = Py_BuildValue("(OOO)", name, bases, namespace_new);
    if (*args_out == NULL)
        goto fail;

    return 0;

    fail:
    Py_XDECREF(*kwds_out);
    Py_XDECREF(*args_out);
    Py_XDECREF(key);
    Py_XDECREF(custom_obj);
    Py_XDECREF(namespace_new);
    return -1;
}

static int
PyMTypeObject_init(PyObject *self, PyObject *args, PyObject *kwds) {
    PyObject *args_out, *kwds_out;
    long custom;
    if (PyMType_ArgParse(args, kwds, &args_out, &kwds_out, &custom) < 0)
        goto fail;
    
    ((PyMTypeObject *) self)->custom = custom;
    
    return self->ob_type->tp_base->tp_init(self, args_out, kwds_out);

    fail:
    return -1;
}

static PyObject*
PyMTypeObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    PyObject *args_out, *kwds_out;
    long custom;
    if (PyMType_ArgParse(args, kwds, &args_out, &kwds_out, &custom) < 0)
        goto fail;

    return type->tp_base->tp_new(type, args_out, kwds_out);

    fail:
    return NULL;
}

PyMTypeObject PyMType_Type = {
    .mt_obj = {
        .ht_type = {
            PyVarObject_HEAD_INIT(&PyType_Type, 0)
            .tp_name = "mtypes.mtype",
            .tp_doc = "Type object for a Python array",
            .tp_basicsize = sizeof(PyMTypeObject),
            .tp_itemsize = 0,
            .tp_flags = Py_TPFLAGS_DEFAULT,
            .tp_base = &PyType_Type,
            .tp_new = PyMTypeObject_new,
            .tp_init = PyMTypeObject_init,
        }
    }
};

PyMODINIT_FUNC
PyInit__mtypes(void) {
    PyObject *m;
    if (PyType_Ready((PyTypeObject*) &PyMType_Type) < 0) {
        return NULL;
    }

    m = PyModule_Create(&mtypes_module);
    if (m == NULL) {
        return NULL;
    }

    Py_INCREF((PyObject*) &PyMType_Type);
    if (PyModule_AddObject(m, "mtype", (PyObject*) &PyMType_Type) < 0) {
        return NULL;
    }

    return m;
}
