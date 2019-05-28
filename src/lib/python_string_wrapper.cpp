#include "python_bridge_tracer/python_string_wrapper.h"

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

//--------------------------------------------------------------------------------------------------
// operator opentracing::string_view
//--------------------------------------------------------------------------------------------------
PythonStringWrapper::operator opentracing::string_view() const noexcept {
  return opentracing::string_view{data_, static_cast<size_t>(length_)};
}
} // namespace python_bridge_tracer
