#pragma once

#include <string>

#include <Python.h>

namespace python_bridge_tracer {
bool toString(PyObject* object, std::string& result) noexcept;
} // namespace python_bridge_tracer
