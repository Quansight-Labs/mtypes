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
                return issubclass(subclass, mtype) and self.ndt == subclass.ndt
        
        class NdtGeneric(ndt, metaclass=mtype_specific):
            def __init__(self, *args, **kwargs):
                super().__init__(*args, **kwargs, type=key)
        
        return NdtGeneric
            

class Ndt(ndt, metaclass=mtype):
    pass