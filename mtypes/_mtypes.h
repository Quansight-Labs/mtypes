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

// Adding Code Guard 

#include "Python.h"
#ifndef MTYPES_H
#define MTYPES_H

typedef struct _mtypeobject PyMTypeObject;
typedef struct _mobject PyMObject;
typedef struct _mfunc PyMTypeFunction;
typedef struct _margument PyMTypeArgument;
#ifdef MTYPES_API
    #undef MTYPES_API
#endif

#ifdef _MSC_VER
  #define MTYPES_API __declspec(dllexport)
#else
  #define MTYPES_API
#endif

typedef PyObject *(*boxfunction)(PyMTypeObject *type, void *data);
typedef int (*unboxfunction)(PyObject *obj, void *data);
typedef int (*mt_func)(void *self, ...);

PyMODINIT_FUNC PyInit__mtypes(void);
static PyObject *PyMType_Type_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int PyMType_Type_init(PyObject *self, PyObject *args, PyObject *kwds);

typedef struct _mfunc
{
    // Analogous to ht_name, ht_slots and ht_qualname in PyHeapTypeObject
    char *mt_name;
    mt_func mt_slot;
    char *mt_qualname;
    PyMTypeArgument *arguments;
    PyMTypeObject *mt_rettype;
} PyMTypeFunction;

typedef struct _margument
{
    // These fields are used to function signatures for a given function.
    char *name;
    PyMTypeObject *type;
} PyMTypeArgument;

typedef struct _mtypeobject
{
    PyHeapTypeObject ht_obj;
    // The functions used for marshaling this type in and out of Python.
    boxfunction box;
    unboxfunction unbox;
    PyMTypeFunction *mt_funcs;
    void *mt_data;
} PyMTypeObject;

typedef struct _mobject
{
    PyObject obj;
    void *m_data;
} PyMObject;

PyMTypeObject PyMType_Type = {
    .ht_obj = {
        .ht_type = {
            PyVarObject_HEAD_INIT(NULL, 0)
                .tp_name = "mtypes.mtype",
            .tp_doc = "Custom memory type object.",
            .tp_basicsize = sizeof(PyMTypeObject),
            .tp_itemsize = 0,
            .tp_flags = Py_TPFLAGS_DEFAULT,
            .tp_new = PyMType_Type_new,
            .tp_init = PyMType_Type_init,
            .tp_base = &PyType_Type},
    },
    .box = NULL,
    .unbox = NULL,
    .mt_data = NULL,
};

PyAPI_DATA(PyMTypeObject) PyMType_Type;

#endif
