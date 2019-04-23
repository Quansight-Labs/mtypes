#include "_mtypes.h"
#include <stdarg.h>

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

//static PyMTypeObject* MTypeObject_Call(PyMTypeObject* callable, PyObject *args, PyObject *kwargs) {
    // This is a function that is *essentially* PyObject_Call but it calls an MTypeObject.
    // ternaryfunc call; // This works
    // // TODO: add args and kwargs into the ht_slots
    // call = callable->ht_obj.ht_type.tp_call;
    // PyMTypeObject* result;
    // assert(!PyErr_Occurred());
    // assert(PyTuple_Check(args));
    // assert(kwargs == NULL || PyDict_Check(kwargs));
    // // PyMTypeObject *test;
    // // return test;
    // // result = (*call)

    

//     return call;
//  };

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



void *mint_unbox(void *o) {
    long* value = malloc(sizeof(long));
    *value = PyLong_AsLong((PyObject *)o);
    return value;
}

PyMObject *mint_box(void *value) {
    PyObject *obj = PyLong_FromLong((long)value);
    PyMObject *mobj = mobj_create(obj, NULL);
    return mobj;
}

PyMODINIT_FUNC
PyInit__mtypes(void)
{
    PyObject *m = NULL;
    PyObject* args = NULL;
    PyMTypeObject *mlong = NULL;
    PyObject* kw = NULL;

    if (PyType_Ready((PyTypeObject *)&PyMType_Type) < 0)
        goto fail;

    m = PyModule_Create(&mtypes_module);
    if (m == NULL)
        goto fail;

    args = Py_BuildValue("(sOO)", "mlong", Py_BuildValue("(O)", PyLong_Type), PyDict_New());
    kw = PyDict_New();
    // // mlong = mtype("mlong", (long,), {})
    // mlong = (PyMTypeObject *) PyObject_Call((PyObject*) (&PyMType_Type), args, kw);
    // if (mlong == NULL)
    //     goto fail;
    // mlong->box = mint_box;
    // mlong->unbox = mint_unbox;

    if (PyModule_AddObject(m, "mtype", (PyObject *)&PyMType_Type) < 0)
        goto fail;

    // if (PyModule_AddObject(m, "mlong", (PyObject *)mlong) < 0)
    //     goto fail;

    // PyObject *mlong = PyObject_Call((PyObject *)&mtlong, args, kw);
    // Py_DECREF(args);
    // Py_DECREF(kw);
    // (PyObject *)mlong;

    return m;
fail:
    Py_XDECREF(m);
    Py_XDECREF(args);
    Py_XDECREF(kw);
    Py_XDECREF(mlong);
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