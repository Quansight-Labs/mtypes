# `mtypes`
Memory Types for Python

## Table of Contents
- [`mtypes`](#mtypes)
  - [Table of Contents](#table-of-contents)
  - [Abstract](#abstract)
  - [Use Cases](#use-cases)
  - [Current Status](#current-status)
  - [Roadmap](#roadmap)

## Abstract
`mtypes` aims to provide two critical things that are useful for C extension types in Python:

- Easy marshalling of python types between Python and C.
- A universal mechanism for easily calling out to C code from Python.

`mtypes` does not aim to be another monolithic Python-to-C library, instead it focuses on extending the Python type system to make wrapping C libraries easier. It aims to do this by making it possible to associate C information with a type, and C information with objects, in addition to mechanisms that allow calling out to C natively from Python (the `__cdict__` protocol).

## Use Cases
`mtypes` intends to let objects work seamlessly between C and Python, meaning that there are several use cases. Common ones include:

- Wrapping numerical libraries.
  - So it could be of use to NumPy, PyTorch, Tensorflow, etc.
- Wrapping C utility libraries or device libraries.
  - So it can can be used to, for example, wrap common CUDA calls.
- Application Migration
  - So you can wrap existing applications or libraries, and give them an easy Python API.

Since C has a well-defined ABI, you can wrap any kind of library that's compatible with the C ABI. Examples include Fortran numerical libraries, C++ libraries that have a C-compatible API, and more.

## Current Status
Currently, `mtypes` can marshall a single C `long` to and from C, more to come.

## Roadmap
 - [ ] Specify API interface
 - [ ] Build out the `__cdict__` protocol.
   - [ ] Build out the `box` and `unbox` methods.
   - [ ] Build out implementation-selection mechanism.
   - [ ] Build out marshalling for more complex types
   - [ ] Documentation