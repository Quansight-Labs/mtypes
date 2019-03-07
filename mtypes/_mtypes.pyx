from ndtypes import ndt

__all__ = ['Xnd', 'mtype']


_mtype_dict = {}


class Xnd:
    def __new__(cls, *args, type=None, **kwargs):
        if type is None:
            raise ValueError('type must be provided.')

        return mtype(type)(*args, **kwargs)


class mtype(type):
    def __new__(cls, t):
        t = ndt(t)

        if t in _mtype_dict:
            return _mtype_dict[t]

        name = f'{cls.__name__}({str(t)})'

        class XndSpecific(Xnd):
            __module__ = __name__
            __qualname__ = name

            _ndt = t

            def __new__(cls, *args, **kwargs):
                return object.__new__(cls)

            def __init__(self, *args, **kwargs):
                t_other = kwargs.pop('type', t)

                if ndt(t_other) != t:
                    raise ValueError(
                        'type Passed in must be consistent with existing type'
                    )

                try:
                    from xnd import xnd
                except ImportError:
                    raise RuntimeError(
                        'xnd must be installed for instantiation to work.')

                self._xnd = xnd(*args, type=t, **kwargs)

            def __getattr__(self, key):
                getattr(self._xnd, key)

            def __getitem__(self, key):
                return self._xnd[key]

            def __setitem__(self, key, value):
                self._xnd[key] = value
        
        return super().__new__(cls, name, (XndSpecific,), {})

    def __init__(self, t: str):
        _mtype_dict[self._ndt] = self

    def __instancecheck__(self, instance):
        return isinstance(type(instance), type(self)) and self._ndt == type(instance)._ndt

    def __subclasscheck__(self, subclass):
        return isinstance(subclass, type(self)) and self._ndt == subclass._ndt

    def __getattr__(self, key):
        return getattr(self._ndt, key)
