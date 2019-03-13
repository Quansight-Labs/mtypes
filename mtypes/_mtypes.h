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

#include "Python.h"

typedef struct _mtypestruct PyMTypeStruct;
typedef struct _mtypeobject PyMTypeObject;
typedef struct _mobject PyMObject;

typedef PyMObject *(*boxfunction)(PyMTypeObject *type, void *data);
typedef int (*unboxfunction)(PyMObject *obj, void *data);
typedef PyMObject *(*mt_boxfunction)(PyMTypeStruct *metatype, void *data);
typedef int (*mt_unboxfunction)(PyMTypeObject *type, void *data);

PyMODINIT_FUNC PyInit__mtypes(void);
static PyObject *PyMType_Type_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int PyMType_Type_init(PyObject *self, PyObject *args, PyObject *kwds);

typedef struct _mtypestruct
{
    PyHeapTypeObject tp_obj;
    mt_boxfunction mt_box;
    mt_unboxfunction mt_unbox;
    void *mtt_data;
} PyMTypeStruct;

typedef struct _mtypeobject
{
    PyHeapTypeObject ht_obj;
    boxfunction obj_box;
    unboxfunction obj_unbox;
    void *mt_data;
} PyMTypeObject;

typedef struct _mobject
{
    PyObject obj;
    void *m_data;
} PyMObject;

PyMTypeStruct PyMType_Type = {
    .tp_obj = {
        .ht_type = {
            PyVarObject_HEAD_INIT(NULL, 0)
            .tp_name = "mtypes.mtype",
            .tp_doc = "Custom memory type object.",
            .tp_basicsize = sizeof(PyMTypeObject),
            .tp_itemsize = 0,
            .tp_flags = Py_TPFLAGS_DEFAULT,
            .tp_new = PyMType_Type_new,
            .tp_init = PyMType_Type_init,
            .tp_base = &PyType_Type
        },
    },
    .mt_box = NULL,
    .mt_unbox = NULL,
    .mtt_data = NULL,
};

PyAPI_DATA(PyMTypeStruct) PyMType_Type;