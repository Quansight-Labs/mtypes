#include "_mtypes.h"

static PyModuleDef mtypes_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "mtypes",
    .m_doc = "Creates a meta-type extending the Python type-system for array objects.",
    .m_size = -1,
};

static
int PyMtypeObject_init(PyObject *self, PyObject *args, PyObject *kwds) {
    printf("mtypes.__init__\n");
    static char* kw = "custom";
    static char *kwlist[] = {"custom", NULL};

    PyMtypeObject* mtypeobj = (PyMtypeObject*) self;
    PyObject* dummy1, dummy2, dummy3;
    long thing;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OOOl:mtype.__new__", kwlist,
                                        dummy1, dummy2, dummy3, &thing))
        goto fail;

    mtypeobj->stuff = thing;

    if (PyDict_DelItemString(kwds, kw) < 0)
        goto fail;

    PyObject* key = PyUnicode_FromString(kw);
    PyObject* value = PyLong_FromLong(thing);

    if (key == NULL || value == NULL)
        goto fail;

    if (PyObject_SetItem(self, key, value) < 0)
        goto fail;

    Py_DECREF(key);
    Py_DECREF(value);
    PyType_Type.tp_init(self, args, kwds);
    return 0;


    fail:
    Py_XDECREF(key);
    Py_XDECREF(value);
    return -1;
}

static
PyObject* PyMtypeObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    printf("mtypes.__new__\n");

    if (PyDict_DelItemString(kwds, "custom") < 0)
        goto fail;
    
    return PyType_Type.tp_new(type, args, kwds);

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