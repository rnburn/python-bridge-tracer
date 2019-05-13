#pragma once

#include <Python.h>

namespace python_bridge_tracer {
class PythonObjectWrapper {
 public:
   PythonObjectWrapper(PyObject* object) noexcept;

   ~PythonObjectWrapper() noexcept;

   PyObject* release() noexcept;

   operator PyObject*() const noexcept { return object_; }

   operator bool() const noexcept { return object_ != nullptr; }
 private:
  PyObject* object_;
};
} // namespace python_bridge_tracer
