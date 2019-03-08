#include "_mtypes.h"

static PyModuleDef mtypes_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "mtypes",
    .m_doc = "Creates a meta-type extending the Python type-system for array objects.",
    .m_size = -1,
};

static int
PyMtype_ArgParse(PyObject *args, PyObject *kwds, PyObject**args_out, PyObject** kwds_out, long* thing_out) {
    static char *kwlist[] = {"", "", "", "custom", NULL};

    PyObject *name, *bases, *namespace;
    
    *args_out = NULL;
    *kwds_out = NULL;
    long thing;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OOOl:mtype.__init__", kwlist,
                                        &name, &bases, &namespace, &thing))
        goto fail;

    *kwds_out = PyDict_New();
    if (*kwds_out == NULL)
        goto fail;

    *args_out = Py_BuildValue("(OOO)", name, bases, namespace);
    if (*args_out == NULL)
        goto fail;

    *thing_out = thing;
    
    Py_DECREF(args);
    Py_DECREF(kwds);

    return 0;

    fail:
    Py_XDECREF(*kwds_out);
    Py_XDECREF(*args_out);
    return -1;
}

static
int PyMtypeObject_init(PyObject *self, PyObject *args, PyObject *kwds) {
    printf("mtypes.__init__\n");
    PyObject *args_out, *kwds_out;
    long thing;
    if (PyMtype_ArgParse(args, kwds, &args_out, &kwds_out, &thing) < 0)
        goto fail;
    
    ((PyMtypeObject *) self)->stuff = thing;
    
    return PyType_Type.tp_init(self, args_out, kwds_out);

    fail:
    return -1;
}

static
PyObject* PyMtypeObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    printf("mtypes.__new__\n");
    PyObject *args_out, *kwds_out;
    long thing;
    if (PyMtype_ArgParse(args, kwds, &args_out, &kwds_out, &thing) < 0)
        goto fail;

    return PyType_Type.tp_new(type, args_out, kwds_out);

    fail:
    return NULL;
}

static PyMtype_Type PyMtype_TypeObject = {
    .mt_type = {
        PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "mtypes.mtype",
        .tp_doc = "Memory type of a Python array",
        .tp_basicsize = sizeof(PyMtypeObject),
        .tp_itemsize = 0,
        .tp_flags = Py_TPFLAGS_DEFAULT,
        .tp_base = &PyType_Type,
        .tp_new = PyMtypeObject_new,
        .tp_init = PyMtypeObject_init,
    }
};

PyMODINIT_FUNC
PyInit__mtypes(void) {
    PyObject *m;
    if (PyType_Ready((PyTypeObject*) &PyMtype_TypeObject) < 0) {
        return NULL;
    }

    m = PyModule_Create(&mtypes_module);
    if (m == NULL) {
        return NULL;
    }

    Py_INCREF((PyObject*) &PyMtype_TypeObject);
    if (PyModule_AddObject(m, "mtype", (PyObject*) &PyMtype_TypeObject) < 0) {
        return NULL;
    }

    return m;
}