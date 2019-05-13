#include "python_object_wrapper.h"

#include <cassert>

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
PythonObjectWrapper::PythonObjectWrapper(PyObject* object) noexcept
    : object_{object} {}

PythonObjectWrapper::PythonObjectWrapper(PythonObjectWrapper&& other) noexcept
  : object_{other.object_}
{
  other.object_ = nullptr;
}

//--------------------------------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------------------------------
PythonObjectWrapper::~PythonObjectWrapper() noexcept {
  if (object_ != nullptr) {
    Py_DECREF(object_);
  }
}

//--------------------------------------------------------------------------------------------------
// operator=
//--------------------------------------------------------------------------------------------------
PythonObjectWrapper& PythonObjectWrapper::operator=(PythonObjectWrapper&& other) noexcept {
  assert(this != &other);
  if (object_ != nullptr) {
    Py_DECREF(object_);
  }
  object_ = other.object_;
  other.object_ = nullptr;
  return *this;
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
