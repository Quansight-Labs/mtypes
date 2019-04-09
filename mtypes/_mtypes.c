#include "_mtypes.h"

PyObject *MType_Type_getattro(PyObject *o, PyObject *attr_name)
{
    PyMTypeObject *self = (PyMTypeObject *)o;

    const char *c_name = PyUnicode_AsUTF8(attr_name);
    if (c_name == NULL)
        goto fail;

    Py_INCREF(attr_name);
    for (PyMTypeFunction *iter = self->mt_funcs; iter->mt_name != NULL; iter++)
    {
        if (strcmp(c_name, iter->mt_name) == 0)
        {
            return iter->mt_rettype;
            Py_DECREF(attr_name);
        }
    }

fail:
    return NULL;

    Py_XDECREF(attr_name);
    Py_DECREF(attr_name);
}

static PyModuleDef mtypes_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "mtypes",
    .m_doc = "Creates a meta-type extending the Python type-system for array objects.",
    .m_size = -1,
};

static int
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

// int _funcslotgen(PyObject *args, PyObject *kwds) {
//     // Function Generateor for Number of Slots
//     // There should be a function for every
// }
static int PyMTypeFunction_Set(PyMTypeFunction *self, PyObject *item)
{
    self->mt_rettype = item;
    return 0;
}

static int
PyMType_Type_init(PyObject *self_obj, PyObject *args, PyObject *kwds)
{
    PyObject *args_out, *kwds_out;
    long custom;
    if (PyMType_ArgParse(args, kwds, &args_out, &kwds_out, &custom) < 0)
        goto fail;

    PyMTypeObject *self = (PyMTypeObject *)self_obj;
    self->mt_data = NULL;
    self->box = NULL;
    self->unbox = NULL;

    self->mt_funcs = malloc(sizeof(PyMTypeFunction) * 10);
    //int _funcs = 3;
    // for(int i = 0; i < _funcs; i++) {
    self->mt_funcs[0].mt_name = "potato";
    PyMTypeFunction_Set(&self->mt_funcs[0], PyUnicode_FromString("Fries"));
    self->mt_funcs[1].mt_name = "tomato";
    PyMTypeFunction_Set(&self->mt_funcs[1], PyUnicode_FromString("Ketchup"));
    self->mt_funcs[2].mt_name = NULL;
    //}

    return self_obj->ob_type->tp_base->tp_init(self_obj, args_out, kwds_out);
fail:
    return -1;
}

static PyObject *
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
