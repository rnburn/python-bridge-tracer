#include "python_object_wrapper.h"

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
PythonObjectWrapper::PythonObjectWrapper(PyObject* object) noexcept
    : object_{object} {}

//--------------------------------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------------------------------
PythonObjectWrapper::~PythonObjectWrapper() noexcept {
  if (object_ != nullptr) {
    Py_DECREF(object_);
  }
}

//--------------------------------------------------------------------------------------------------
// release
//--------------------------------------------------------------------------------------------------
PyObject* PythonObjectWrapper::release() noexcept {
  auto result = object_;
  object_ = nullptr;
  return result;
}
} // namespace python_bridge_tracer
