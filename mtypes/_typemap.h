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

// This file is used to contain the methods to check and return
// types -- both to and from Python -> C.

// This is the inital C DICT Impletmentation.

#include <stdio.h>
#include <stdlib.h>
#include "_mtypes.h"

#ifndef TPMAP_H
#define TPMAP_H
// TODO: ADD API Funtions to ADD types manually

// Int, Float, Complex <-- this will change in final version...have a couple ideas; but 21 ctypes.
// https://docs.python.org/3/library/ctypes.html#fundamental-data-types
#define ARR_INIT_SZ 21 

/* mtypeNode is a node whose pointer is cast to the mapping function.
 * 
 *
 */
typedef struct
{
    void *type;       // Pointer to the type that is being input; this can be anything.
    PyMTypeObject *obj; // Pointer to Corresponding mtypeobject
} mtypeNode;

typedef struct
{
    int elements;   // Current size of the array that is filled
    int size;       // total available spaces
    mtypeNode *data; // node pointer to the proper node that contains the data
                    //...I don't think void* is needed.
} marray;

int marray_init(marray *array)
{
    array->elements = 0;
    array->size = ARR_INIT_SZ;
    array->data = malloc(sizeof(mtypeNode) * array->size);
    return 0;
}

void marray_resize(marray *array)
{
    if (array->elements >= array->size)
    {
        array->size *= 2;
        array->data = realloc(array->data, sizeof(mtypeNode) * array->size);
    }
}

void marray_add(marray *array, mtypeNode *node)
{
    if (array->elements < array->size)
    {
        array->data[array->elements++] = *node;
    }
    else if (array->elements >= array->size)
    {
        marray_resize(array);
        array->data[array->elements++] = *node;
    }
}

#endif