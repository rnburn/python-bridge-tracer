#pragma once

#include <Python.h>

namespace python_bridge_tracer {
/**
 * Small RAII wrapper class for python objects.
 */
class PythonObjectWrapper {
 public:
   PythonObjectWrapper() noexcept = default;

   PythonObjectWrapper(PyObject* object) noexcept;

   PythonObjectWrapper(const PythonObjectWrapper&) = delete;

   PythonObjectWrapper(PythonObjectWrapper&& other) noexcept;

   ~PythonObjectWrapper() noexcept;

   PythonObjectWrapper& operator=(const PythonObjectWrapper&) = delete;

   PythonObjectWrapper& operator=(PythonObjectWrapper&& other) noexcept;

   /**
    * Return the wrapped PyObject* and don't decrement the reference count on destruction.
    * @return teh wrapped PyObject*
    */
   PyObject* release() noexcept;

   operator PyObject*() const noexcept { return object_; }

   operator bool() const noexcept { return object_ != nullptr; }
 private:
  PyObject* object_{nullptr};
};
} // namespace python_bridge_tracer
