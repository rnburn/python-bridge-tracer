#pragma once

#include <Python.h>

#include "python_object_wrapper.h"
#include "opentracing/string_view.h"

namespace python_bridge_tracer {
/**
 * Support accessing a python string object as a opentracing::string_view.
 */
class PythonStringWrapper {
 public:
  PythonStringWrapper() noexcept = default;

  PythonStringWrapper(PyObject* object) noexcept;

  operator bool() const noexcept { return data_ != nullptr; }

  operator opentracing::string_view() const noexcept;

 private:
  PythonObjectWrapper utf8_;
  char* data_{nullptr};
  Py_ssize_t length_{0};
};
}  // namespace python_bridge_tracer
