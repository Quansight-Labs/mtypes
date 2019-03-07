#include "_mtypes.h"

static PyTypeObject PyMtypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "mtypes.mtype",
    .tp_doc = "Memory type of a Python array",
    .tp_basicsize = sizeof(PyNdtObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_base = &PyType_Type,
};

static PyModuleDef mtypes_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "mtypes",
    .m_doc = "Creates a meta-type extending the Python type-system for array objects.",
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit__mtypes(void)
{
    PyObject *m;
    if (PyType_Ready(&PyMtypeObject) < 0) {
        return NULL;
    }

    m = PyModule_Create(&mtypes_module);
    if (m == NULL) {
        return NULL;
    }

    Py_INCREF(&PyMtypeObject);

    // It is importing, so this ran successfully.
    if (PyModule_AddObject(m, "mtype", (PyObject*) &PyMtypeObject) < 0) {
        return NULL;
    }

    return m;
}