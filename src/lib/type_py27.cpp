#include "python_bridge_tracer/type.h"
#include "python_bridge_tracer/version.h"

#ifndef PYTHON_BRIDGE_TRACER_PY3

namespace python_bridge_tracer {
PyObject* makeTypeImpl(PyTypeObject& type) noexcept {
  if (PyType_Ready(&type) < 0) {
    return nullptr;
  }
  auto type_obj = reinterpret_cast<PyObject*>(&type);
  Py_INCREF(type_obj);
  return type_obj;
}
}  // namespace python_bridge_tracer
#endif
