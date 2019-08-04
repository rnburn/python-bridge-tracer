#pragma once

#include <Python.h>

namespace python_bridge_tracer {
struct TypeDescription {
  const char* name = nullptr;
  size_t size = 0;
  void* doc = nullptr;
  void* dealloc = nullptr;
  void* methods = nullptr;
  void* getset = nullptr;
};

PyObject* makeTypeImpl(const TypeDescription& type_description) noexcept;

template <class T>
PyObject* makeType(const TypeDescription& type_description) noexcept {
  return makeTypeImpl(type_description);
}
} // namespace python_bridge_tracer
