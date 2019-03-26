#if defined(__cplusplus)
extern "C" {
#endif

#include "_typemap.hpp"

#include <unordered_map>

MTYPES_API int MLookupTable_add(PyTypeObject *type, PyMTypeObject *mtype)
{
   if (m_lookuptable.find(type) != m_lookuptable.end())
   {
       return -1;
   }

   m_lookuptable[type] = mtype;
   return 0;
}

MTYPES_API PyMTypeObject *MLookupTable_get(PyTypeObject *type) {
    if (m_lookuptable.find(type) == m_lookuptable.end())
    {
        return NULL;
    }

    return m_lookuptable[type];
}

#if defined(__cplusplus)
}
#endif