#include "_mtypes.h"

static PyTypeObject PyNdt_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "mtypes.ndt",
    .tp_doc = "Memory type of a Python array",
    .tp_basicsize = sizeof(PyNdtObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_init = PyNdt_Type_init,
    .tp_base = &PyType_Type,
};

static PyModuleDef mtypes_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "mtypes",
    .m_doc = "Creates a meta-type extending the Python type-system for array objects.",
    .m_size = -1,
};

PyObject* PyNdt_Type_init(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    printf("Creating ndt instance.");

    PyNdtObject *self;
    self = (PyNdtObject*) type->tp_alloc(type, 0);

    if (self != NULL) {
        PyTypeObject* typeobj = &(self->pytypeobj.ht_type);
        typeobj->tp_name = "mtypes.ndt('int64')";
        typeobj->tp_doc = "Type of a Python array";
        typeobj->tp_basicsize = sizeof(PyXndObject);
        typeobj->tp_itemsize = 0;
        typeobj->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HEAPTYPE;
        typeobj->tp_new = PyType_GenericNew;

        if (PyType_Ready(self) < 0) {
            return NULL;
        }
        
        return self;
    }

    return NULL;
}

PyMODINIT_FUNC
PyInit__mtypes(void) {
    PyObject *m;
    if (PyType_Ready(&PyNdt_Type) < 0) {
        return NULL;
    }

    m = PyModule_Create(&mtypes_module);
    if (m == NULL) {
        return NULL;
    }

    Py_INCREF(&PyNdt_Type);

    // It is importing, so this ran successfully.
    if (PyModule_AddObject(m, "ndt", (PyObject*) &PyNdt_Type) < 0) {
        return NULL;
    }

    return m;
}