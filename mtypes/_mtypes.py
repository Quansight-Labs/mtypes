from ndtypes import ndt
from xnd import xnd

__all__ = ['mtype']


class _mclass(type):
    def __class_getitem__(cls, key):
        key = ndt(key)

        class mclass_specific(_mclass):
            ndt = key

            def __instancecheck__(self, instance):
                return isinstance(instance, xnd) and instance.type == self.ndt

            def __subclasscheck__(self, subclass):
                return self.ndt == subclass.ndt

            def __eq__(self, other):
                return self.ndt == other.ndt

            def __neq__(self, other):
                return self.ndt != other.ndt

        class NdtGeneric(xnd, metaclass=mclass_specific):
            def __new__(cls, *args, **kwargs):
                return xnd(*args, **kwargs, type=key)

        return NdtGeneric

    def __instancecheck__(self, instance):
        return isinstance(self, ndt)

    def __subclasscheck__(self, subclass):
            return issubclass(subclass, ndt) or isinstance(subclass, _mclass)


class mtype(ndt, metaclass=_mclass):
    def __new__(cls, *args, **kwargs):
        return ndt(*args, **kwargs)
