#include "python_bridge_tracer/python_string_wrapper.h"

#include "python_bridge_tracer/version.h"

#ifdef PYTHON_BRIDGE_TRACER_PY3

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
PythonStringWrapper::PythonStringWrapper(PyObject* object) noexcept 
  : utf8_{PyUnicode_AsUTF8String(object)}
{
  if (utf8_.error()) {
    return;
  }
  if (PyBytes_AsStringAndSize(utf8_, &data_, &length_) == -1) {
    data_ = nullptr;
    return;
  }
}
} // namespace python_bridge_tracer

#endif
