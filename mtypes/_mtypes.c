#include "_mtypes.h"
#include <stdarg.h>

static PyModuleDef mtypes_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "mtypes",
    .m_doc = "Creates a meta-type extending the Python type-system to be cooperative with a variety of types.",
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

static int
PyMType_Type_init(PyObject *self_obj, PyObject *args, PyObject *kwds)
{
    if (self_obj->ob_type->tp_base->tp_init(self_obj, args, kwds) < 0)
        goto fail;

    PyMTypeObject *self = (PyMTypeObject *)self_obj;
    self->box = NULL;
    self->unbox = NULL;
    self->mt_data = NULL;
    return 0;
fail:
    return -1;
}


/*
 * Takes in a generic type of specific degree
 * (i.e. JS Long, Java Long)
 * and 'unboxes' to a C long.
 */
long mint_unbox(void *o) {
    // this is going to take in a generic object,
    // and return a C object.
    long value = PyLong_AsLong((PyObject *)o);
    return value;
}

PyMObject *mint_box(PyObject *value) {
    PyObject *obj = PyLong_FromLong((long)value);
    PyMObject *mobj = mobj_create(obj, NULL);
    return mobj;
}

PyMODINIT_FUNC
PyInit__mtypes(void)
{
    PyObject *m = NULL;
    PyObject *args = NULL;
    PyMTypeObject *mtlong = NULL;
    PyMObject *molong = NULL;
    PyObject* kw = NULL;

    if (PyType_Ready((PyTypeObject *)&PyMType_Type) < 0)
        goto fail;

    m = PyModule_Create(&mtypes_module);
    if (m == NULL)
        goto fail;

    args = Py_BuildValue("(sOO)", "mlong", PyLong_Type, PyDict_New());
    kw = PyDict_New();
    molong->obj = *PyObject_Call((PyObject *)&mtlong, args, kw);
    mtlong->box = mint_box;
    mtlong->unbox = mint_unbox;
    if (mtlong == NULL)
        goto fail;
    if (PyModule_AddObject(m, "mtype", (PyObject *)&PyMType_Type) < 0)
        goto fail;
    if (PyModule_AddObject(m, "mlong", (PyObject *)mtlong) < 0)
        goto fail;

    
    // Py_DECREF(args);
    // Py_DECREF(kw);
    // (PyObject *)mlong;

    return m;
fail:
    Py_XDECREF(m);
    Py_XDECREF(args);
    Py_XDECREF(kw);
    //Py_XDECREF(mlong);
    return NULL;
}



PyObject* MFunction_Call(PyObject* o, PyObject *a, PyObject *kw)
{
    

    // Figure out number of arguments
    // - Number and Types for Args -> match to each signature in function // a = mtypes.mtype(mtypes.mint(5), '5.1')
    // Allocate the void**
    // - void **ll_args = malloc(sizeof(void *) * (nargs + 1))
    // Unbox stuff, and put in in the void**
    // for (int i = 0; i < nargs; i++) {
    //     ll_args[i] = o.type[i].unbox(py_arg[i]) // From selected sig
    // }
    // ll_args[nargs] = NULL;
    // void *ll_out = o.mt_slot(ll_args) // From selected sig
    // PyMObject* a = o.ret_type.box(ll_out);
    // Return the boxed stuff
    // PyMTypeObject *self = (PyMTypeObject *)a;
    return o;
};
