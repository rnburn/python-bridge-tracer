#pragma once

#include <Python.h>

#define PY_BRIDGE_DEFINE_MODULE(NAME) PyMODINIT_FUNC PyInit_##NAME() noexcept
