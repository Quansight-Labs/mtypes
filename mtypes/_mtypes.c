/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017-2018, Quansight-Labs
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <Python.h>
#include "pymacro.h"

#ifdef _MSC_VER
  #ifndef UNUSED
    #define UNUSED
  #endif
#else
  #if defined(__GNUC__) && !defined(__INTEL_COMPILER)
    #define UNUSED __attribute__((unused))
  #else
    #define UNUSED
  #endif
#endif


PyDoc_STRVAR(doc_module, "mtypes module");

/****************************************************************************/
/*                                 Xnd type                                 */
/****************************************************************************/

static PyTypeObject MemType_Type =
{
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "mtypes.mtype",
    .tp_basicsize = sizeof(PyMemType_Object),
    .tp_dealloc = (destructor) mtype_dealloc,
    .tp_hash = PyObject_HashNotImplemented,
    .tp_getattro = (getattrofunc) PyObject_GenericGetAttr,
    .tp_as_buffer = &mtype_as_buffer,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_traverse = (traverseproc)pyxnd_traverse,
    .tp_methods = mtype_methods,
    .tp_getset = mtype_getsets,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = mtype_new,
    .tp_free = PyObject_GC_Del
};

/****************************************************************************/
/*                                   C-API                                  */
/****************************************************************************/

static void **xnd_api[XND_MAX_API];

static int
Xnd_CheckExact(const PyObject *v)
{
    return Py_TYPE(v) == &Xnd_Type;
}

static int
Xnd_Check(const PyObject *v)
{
    return PyObject_TypeCheck(v, &Xnd_Type);
}

static const xnd_t *
CONST_XND(const PyObject *v)
{
    assert(Xnd_Check(v));
    return &((XndObject *)v)->xnd;
}

static PyObject *
Xnd_EmptyFromType(PyTypeObject *tp, ndt_t *t)
{
    MemoryBlockObject *mblock;
    PyObject *type;

    type = Ndt_FromType(t);
    if (type == NULL) {
        return NULL;
    }

    mblock = mblock_empty(type);
    Py_DECREF(type);
    if (mblock == NULL) {
        return NULL;
    }

    return pyxnd_from_mblock(tp, mblock);
}

static PyObject *
Xnd_ViewMoveNdt(const PyObject *v, ndt_t *t)
{
    XndObject *src = (XndObject *)v;
    XndObject *view;
    PyObject *type;

    if (!Xnd_Check(v)) {
        PyErr_SetString(PyExc_TypeError, "expected xnd object");
        ndt_del(t);
        return NULL;
    }

    type = Ndt_MoveSubtree(src->type, t);
    if (type == NULL) {
        return NULL;
    }

    view = pyxnd_alloc(Py_TYPE(src));
    if (view == NULL) {
        Py_DECREF(type);
        return NULL;
    }

    Py_INCREF(src->mblock);
    view->mblock = src->mblock;
    view->type = type;
    view->xnd = src->xnd;
    view->xnd.type = t;

    return (PyObject *)view;
}

static PyObject *
Xnd_FromXnd(PyTypeObject *tp, xnd_t *x)
{
    MemoryBlockObject *mblock;

    mblock = mblock_from_xnd(x);
    if (mblock == NULL) {
        return NULL;
    }

    return pyxnd_from_mblock(tp, mblock);
}


static PyObject *
init_api(void)
{
    xnd_api[Xnd_CheckExact_INDEX] = (void *)Xnd_CheckExact;
    xnd_api[Xnd_Check_INDEX] = (void *)Xnd_Check;
    xnd_api[CONST_XND_INDEX] = (void *)CONST_XND;
    xnd_api[Xnd_EmptyFromType_INDEX] = (void *)Xnd_EmptyFromType;
    xnd_api[Xnd_ViewMoveNdt_INDEX] = (void *)Xnd_ViewMoveNdt;
    xnd_api[Xnd_FromXnd_INDEX] = (void *)Xnd_FromXnd;

    return PyCapsule_New(xnd_api, "xnd._xnd._API", NULL);
}


/****************************************************************************/
/*                                  Module                                  */
/****************************************************************************/



static struct PyModuleDef mtypes_module = {
    PyModuleDef_HEAD_INIT,        /* m_base */
    "_mtypes",                       /* m_name */
    doc_module,                   /* m_doc */
    -1,                           /* m_size */
    NULL,                         /* m_methods */
    NULL,                         /* m_slots */
    NULL,                         /* m_traverse */
    NULL,                         /* m_clear */
    NULL                          /* m_free */
};


PyMODINIT_FUNC
PyInit__mtypes(void)
{
    PyObject *m = NULL;

    MemType_Type.tp_base = &PyType_Type;
    if (PyType_Ready(&MemType_Type) < 0) {
        return NULL;
    }

    m = PyModule_Create(&memtype_module);
    if (m == NULL) {
        goto error;
    }

    Py_INCREF(&MemType_Type);
    if (PyModule_AddObject(m, "mtype", (PyObject *)&MemType_Type) < 0) {
        goto error;
    }

    return m;

error:
    Py_CLEAR(m);
    return NULL;
}


