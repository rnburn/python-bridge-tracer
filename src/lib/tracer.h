#pragma once

#include <Python.h>

namespace python_bridge_tracer {
/**
 * Setup the python tracer class.
 * @param module the module to add the class to
 * @return true if successful
 */
bool setupTracerClass(PyObject* module) noexcept;
} // namespace python_bridge_tracer
