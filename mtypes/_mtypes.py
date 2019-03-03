from ndtypes import ndt
from xnd import xnd

__all__ = ['Ndt']


class mtype(type):
    def __class_getitem__(cls, key):
        key = ndt(key)

        class mtype_specific(mtype):
            ndt = key

            def __instancecheck__(self, instance):
                return isinstance(instance, xnd) and instance.type == self.ndt

            def __subclasscheck__(self, subclass):
                return super().__subclasscheck__(subclass) and self.ndt == subclass.ndt

            def __eq__(self, other):
                return self.ndt == other.ndt

            def __neq__(self, other):
                return self.ndt != other.ndt

        class NdtGeneric(xnd, metaclass=mtype_specific):
            def __new__(cls, *args, **kwargs):
                return xnd(*args, **kwargs, type=key)

        return NdtGeneric

    def __instancecheck__(self, instance):
        return isinstance(self, ndt)

    def __subclasscheck__(self, subclass):
            return isinstance(subclass, mtype)


class Ndt(ndt, metaclass=mtype):
    pass
