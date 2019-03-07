#include "_mtypes.h"

PyMODINIT_FUNC
PyInit_custom(void)
{
    PyObject *m;
    if (PyType_Ready(&PyMtypeObject) < 0)
        return NULL;

    m = PyModule_Create(&mtypes_module);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PyMtypeObject);

    if (PyModule_AddObject(m, "mtype", (PyObject*) &PyMtypeObject) < 0)
        return NULL;

    return m;
}