#
# BSD 3-Clause License
#
# Copyright (c) 2017-2018, Quansight Labs
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

from distutils.sysconfig import get_python_lib
import sys
import os
import platform
import warnings

if "bdist_wheel" in sys.argv:
    from setuptools import setup, Extension
else:
    from distutils.core import setup, Extension


DESCRIPTION = """\
Meta-class of type that extends Python type system to create dtype-like memory-types\
"""

LONG_DESCRIPTION = """
Overview
--------

The "mtypes" package provides a memory-type system suitable for use by any container, object, or 
library where the memory-layout matters or access to fast native-code implementations is needed.

Specifically it creates a new sub-class of type at the CPython layer so that every mtype is a type
but with special function pointers needed for manipulating the raw memory that the type implies.

Links
-----

* https://github.com/Quansight-Labs/
* http://mtypes.readthedocs.io/en/latest/

"""

setup(
    name="mtypes",
    version="0.1.1",
    description=DESCRIPTION,
    long_description=LONG_DESCRIPTION,
    url="https://github.com/Quansight-Labs/mtypes",
    author='Quansight Labs',
    author_email='labs@quansight.com',
    license="BSD License",
    keywords=["xnd", "array computing", "types", "memory blocks"],
    platforms=["Many"],
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "Intended Audience :: Education",
        "Intended Audience :: Science/Research",
        "Intended Audience :: Financial and Insurance Industry",
        "License :: OSI Approved :: BSD License",
        "Programming Language :: C",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Operating System :: OS Independent",
        "Topic :: Scientific/Engineering :: Mathematics",
        "Topic :: Software Development"
    ],
    packages=["mtypes"],
    package_data={"mtypes": ["_mtypes.h"]},
    ext_modules=[Extension(name="mtypes._mtypes",
                           sources=["mtypes/_mtypes.c"],
                           extra_compile_args=[ "-stdlib=libc++"],
                           language="c++20")],
    python_requires=">=3.5, <4",
)
