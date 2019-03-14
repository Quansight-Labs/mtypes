``mtypes`` API Specification
============================

``mtypes.mtype`` is a metaclass that allows you to associate extra
C-level information with your ``type`` objects like classes within
Python. Also, objects instantiated from those types also have C-level
data associated with them.

``PyMType_Type``
-----------------

.. code:: c

    PyMTypeObject PyMType_Type;

``PyMType_Type`` is the base class for everything. This will be used
as the main ``type`` to generate all other ``mtypes``.

``PyMTypeObject``
-----------------

.. code:: c

    typedef struct _mtypeobject
    {
        PyHeapTypeObject ht_obj;
        boxfunction box;
        unboxfunction unbox;
        void *mt_data;
    } PyMTypeObject;

``PyMTypeObject`` is essentially a ``PyTypeObject`` that allows for extra
functionality, effectively:

- Storing extra data within the object within the ``mt_data`` member.
- Marshalling to and from C with the ``__cdict__`` protocol via the
  ``box`` and ``unbox`` methods.


``PyMObject``
--------------
.. code:: c

    typedef struct _mobject
    {
        PyObject obj;
        void *m_data;
    } PyMObject;

``PyMObject`` It acts as an intermediary between the Python level
object and the C level object. It contains:

- ``PyObject``, which is the Python-Level Object
- ``void *m_data`` which stores the C-level data that is needed
  for this object to be represented.

We will use the ``box`` and ``unbox`` methods to interface between
the Pythonic and C level objects via the ``__cdict__`` protocol.

``box`` and ``unbox``
---------------------

These functions will be used as the layer of translation between C
structs and Python objects.

.. code:: c

    typedef PyObject *(*boxfunction)(PyMTypeObject *type, void *data);
    typedef int (*unboxfunction)(PyObject *obj, void *data);

``boxfunction``
~~~~~~~~~~~~~~~

The box function is defined as the layer that converts a C struct into a
Python object. The function will perform error checking and will return
an *instance* of a ``PyMType``.

Input Arguments
^^^^^^^^^^^^^^^

-  ``PyMTypeObject *type`` : The type that the C struct should be marshalled
   into.
-  ``void *data``: A pointer to the data that needs to be marshalled and
   converted to a Python Object.

Output Argument
^^^^^^^^^^^^^^^

- ``PyObject *out``: A pointer to a ``PyObject`` initalized on
  the heap from the C struct. ``NULL`` indicates failure. If returning
  ``NULL``, a Python exception must be set. The returned object must be
  a pointer to either a ``PyMTypeObject`` or ``PyMObject`` which has
  the Python type of ``type``.

``unboxfunction``
~~~~~~~~~~~~~~~~~

The ``unbox`` function is a C function that takes in the Python object
to be marshalled into a C struct.

.. _input-arguments-1:

Input Arguments
^^^^^^^^^^^^^^^

- ``PyObject *obj``: The object to be marshalled into C. Its type
  must be an instance of ``PyMTypeObject``.
- ``void *data``: A pointer to the C struct to put the data into.

.. _output-argument-1:

Output Argument
^^^^^^^^^^^^^^^

- ``int return_code``: Must be ``-1`` on failure and ``0`` on success.
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
