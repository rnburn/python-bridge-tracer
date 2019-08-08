#include "python_bridge_tracer/type.h"
#include "python_bridge_tracer/version.h"

#ifndef PYTHON_BRIDGE_TRACER_PY3

namespace python_bridge_tracer {
PyObject* makeTypeImpl(PyTypeObject& type) noexcept {
  if (PyType_Ready(&type) < 0) {
    return nullptr;
  }
  Py_INCREF(&type);
  return reinterpret_cast<PyObject*>(&type);
}
}  // namespace python_bridge_tracer
#endif
