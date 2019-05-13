#pragma once

#include <Python.h>

namespace python_bridge_tracer {
class PythonUnicodeObject {
 public:
   PythonUnicodeObject(const char* data, size_t length) noexcept;

   ~PythonUnicodeObject() noexcept;

   PyObject* object() noexcept { return object_; }
 private:
   PyObject* object_;
};
} // namespace python_bridge_tracer
