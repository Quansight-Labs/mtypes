#include "_mtypes.h"

static PyModuleDef mtypes_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "mtypes",
    .m_doc = "Creates a meta-type extending the Python type-system for array objects.",
    .m_size = -1,
};

static int
PyMType_ArgParse(PyObject *args, PyObject *kwds, PyObject**args_out, PyObject** kwds_out, long* thing_out) {
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

    return 0;

    fail:
    Py_XDECREF(*kwds_out);
    Py_XDECREF(*args_out);
    return -1;
}

static
int PyMTypeObject_init(PyObject *self, PyObject *args, PyObject *kwds) {
    printf("mtypes.__init__\n");
    PyObject *args_out, *kwds_out;
    long thing;
    if (PyMType_ArgParse(args, kwds, &args_out, &kwds_out, &thing) < 0)
        goto fail;
    
    ((PyMTypeObject *) self)->stuff = thing;
    
    return ((PyTypeObject *)self)->tp_base->tp_init(self, args_out, kwds_out);

    fail:
    return -1;
}

static
PyObject* PyMTypeObject_new(PyTypeObject *self, PyObject *args, PyObject *kwds) {
    printf("mtypes.__new__\n");
    PyObject *args_out, *kwds_out;
    long thing;
    if (PyMType_ArgParse(args, kwds, &args_out, &kwds_out, &thing) < 0)
        goto fail;

    return self->tp_base->tp_new(self, args_out, kwds_out);

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