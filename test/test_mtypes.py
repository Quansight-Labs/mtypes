import functools

import pytest

from ndtypes import ndt
from xnd import xnd

from mtypes import mtype, Xnd


def chain_marks(*marks):
    def inner(func):
        for mark in reversed(marks):
            func = mark(func)

        return func

    return inner


mark_input_types = pytest.mark.parametrize('input_type', [
    'int32', 'int64', 'uint32', '2 * int32', '2 * 3 * int32'
])

mark_output_types = pytest.mark.parametrize('output_type', [
    'int32', 'int64', 'uint32', '2 * int32', '2 * 3 * int32'
])

mark_both_types = chain_marks(mark_input_types, mark_output_types)


def value_of_shape(shape):
    value = 0

    for d in reversed(shape):
        value = [value] * d

    return value


def Xnd_of_type(ndt_type):
    shape = ndt(ndt_type).shape
    value = value_of_shape(shape)
    return Xnd(value, type=ndt_type)


@mark_both_types
def test_mtypes(input_type, output_type):
    expected = ndt(input_type) == ndt(output_type)

    assert expected == isinstance(Xnd_of_type(output_type), mtype(input_type))


@mark_both_types
def test_subclass(input_type, output_type):
    expected = ndt(input_type) == ndt(output_type)

    assert expected == issubclass(
        mtype(input_type), mtype(output_type))


@mark_input_types
def test_subclass_from_generic(input_type):
    assert issubclass(mtype(input_type), Xnd)


@mark_input_types
def test_creation(input_type):
    input_mtype = mtype(input_type)
    value = value_of_shape(input_mtype.shape)
    assert isinstance(input_mtype(value), mtype(input_type))


@mark_input_types
def test_type(input_type):
    input_mtype = mtype(input_type)
    value = value_of_shape(input_mtype.shape)
    assert type(input_mtype(value)) is input_mtype