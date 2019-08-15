#include "python_bridge_tracer/python_string_wrapper.h"

#include "python_bridge_tracer/version.h"

#ifndef PYTHON_BRIDGE_TRACER_PY3

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
PythonStringWrapper::PythonStringWrapper(PyObject* object) noexcept 
{
  if (PyString_AsStringAndSize(object, &data_, &length_) == -1) {
    data_ = nullptr;
    return;
  }
}
} // namespace python_bridge_tracer

#endif
