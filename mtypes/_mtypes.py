from ndtypes import ndt
from xnd import xnd

__all__ = ['Xnd', 'mtype']


_mtype_dict = {}


class _XndMeta(type):
    def __call__(self, *args, type=None, **kwargs):
        if type is None:
            raise ValueError('type must be provided.')

        return mtype(type)(*args, **kwargs)

    def __instancecheck__(self, instance):
        return isinstance(type(instance), mtype)

    def __subclasscheck__(self, subclass):
        return isinstance(subclass, mtype)


class Xnd(metaclass=_XndMeta):
    pass


class mtype(type):
    def __new__(cls, t: str):
        t = ndt(t)

        if t in _mtype_dict:
            return _mtype_dict[t]

        class XndSpecific:
            def __init__(self, *args, **kwargs):
                self.xnd = xnd(*args, type=t, **kwargs)

            def __getattr__(self, key):
                getattr(self.xnd, key)

        name = f"{cls.__name__}({str(t)})"
        return super().__new__(cls, name, XndSpecific.__bases__, {**XndSpecific.__dict__, '__module__': __name__, '__qualname__': name, 'ndt': t})

    def __init__(self, t: str):
        _mtype_dict[self.ndt] = self

    def __instancecheck__(self, instance):
        return isinstance(type(instance), type(self)) and self.ndt == type(instance).ndt

    def __subclasscheck__(self, subclass):
        return isinstance(subclass, type(self)) and self.ndt == subclass.ndt

    def __getattr__(self, key):
        return getattr(self.ndt, key)
