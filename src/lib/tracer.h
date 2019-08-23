#pragma once

#include <vector>

#include "tracer_bridge.h"

#include <Python.h>

namespace python_bridge_tracer {
/**
 * Setup the python tracer class.
 * @param module the module to add the class to
 * @param extension_methods additional methods a vendor tracer can add
 * @param extension_getsets additional getsets a vendor tracer can add
 * @return true if successful
 */
bool setupTracerClass(
    PyObject* module, const std::vector<PyMethodDef>& extension_methods = {},
    const std::vector<PyGetSetDef>& extension_getsets = {}) noexcept;
}  // namespace python_bridge_tracer
