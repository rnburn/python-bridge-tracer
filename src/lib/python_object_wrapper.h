#pragma once

#include <Python.h>

namespace python_bridge_tracer {
class PythonObjectWrapper {
 public:
   PythonObjectWrapper() noexcept = default;

   PythonObjectWrapper(PyObject* object) noexcept;

   PythonObjectWrapper(const PythonObjectWrapper&) = delete;

   PythonObjectWrapper(PythonObjectWrapper&& other) noexcept;

   ~PythonObjectWrapper() noexcept;

   PythonObjectWrapper& operator=(const PythonObjectWrapper&) = delete;

   PythonObjectWrapper& operator=(PythonObjectWrapper&& other) noexcept;

   PyObject* release() noexcept;

   operator PyObject*() const noexcept { return object_; }

   operator bool() const noexcept { return object_ != nullptr; }
 private:
  PyObject* object_{nullptr};
};
} // namespace python_bridge_tracer
