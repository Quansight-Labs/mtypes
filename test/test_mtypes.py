import pytest

from ndtypes import ndt
from xnd import xnd

import mtypes

@pytest.mark.parametrize('input_type',[
    'int32', 'int64', 'uint32'
])
@pytest.mark.parametrize('output_type',[
    'int32', 'int64', 'uint32'
])
def test_mtypes(input_type, output_type):
    expected = ndt(input_type) == ndt(output_type)

    x = xnd(0, type=output_type)

    assert expected == isinstance(x, mtypes.Ndt[input_type])