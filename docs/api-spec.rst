``mtypes`` API Specification
============================

``mtypes.mtype`` is a metaclass that allows you to associate extra
C-level information with your ``type`` objects like classes within
Python. Also, objects instantiated from those types also have C-level
data associated with them.

``PyMTypeObject`` and ``PyMObject``
-----------------------------------

.. code:: c

   typedef struct _mtypeobject {
       PyHeapTypeObject ht_obj;
       boxfunction mt_box;
       unboxfunction mt_unbox;
       void* mt_data;
   } PyMTypeObject;

   typedef struct _mobject {
       PyObject obj;
       void* m_data;
    } PyMObject;

``box`` and ``unbox``
---------------------

These functions will be used as the layer of translation between C
structs and Python objects.

.. code:: c

   typedef int (*unboxfunction)(PyMObject *obj, void *data);
   typedef PyMObject *(*boxfunction)(PyMTypeObject *type, void *data);

``boxfunction``
~~~~~~~~~~~~~~~

The box function is defined as the layer that converts a C struct into a
Python object. The function will perform error checking and will return
an *instance* of a ``PyMType``.

Input Arguments
^^^^^^^^^^^^^^^

-  ``PyMType *type`` : The type that the C struct should be marshalled
   into.
-  ``void *data``: A pointer to the data that needs to be marshalled and
   converted to a Python Object.

Output Argument
^^^^^^^^^^^^^^^

-  ``PyMObject *out``: A pointer to a ``PyMObject`` initalized on
   the heap from the C struct. ``NULL`` indicates failure. If returning
   ``NULL``, a Python exception must be set.

``unboxfunction``
~~~~~~~~~~~~~~~~~

The ``unbox`` function is a C function that takes in the Python object
to be marshalled into a C struct.

.. _input-arguments-1:

Input Arguments
^^^^^^^^^^^^^^^

-  ``PyMObject *obj``: The object to be marshalled into C.
-  ``void *data``: A pointer to the C struct to put the data into.

.. _output-argument-1:

Output Argument
^^^^^^^^^^^^^^^

-  ``int return_code``: Must be ``-1`` on failure and ``0`` on success.
   If returning ``-1``, a Python exception must be set.

``__cdict__`` protocol
----------------------

The ``__cdict__`` protocol will be used as an intermediary between
Python level objects and C structs. This will allow a specific type
signature to be passed from the Python Object *into* the ``unbox``
function, then passing to a ``ctypes`` method, and then calling the
``box`` function eventually returning a ``PyMObject``.

This dict provides the ``boxfunction`` and the ``unboxfunction`` with
the proper associated C type signature.

.. code:: python3

   from typing import Tuple, Dict
   from ctypes import CFUNCTYPE
   from mtypes import mtype

   class MarshalledClass(metaclass=mtype):
       __cdict__:
           Dict[str, # Lookup by name
               Dict[
                   paramflags_type, # Lookup by signature
                   CFUNCTYPE,       # Implementation
               ],
           ]
