#include "python_bridge_tracer/module.h"
#include "python_bridge_tracer/version.h"

#ifndef PYTHON_BRIDGE_TRACER_PY3

namespace python_bridge_tracer {
PyObject* makeModule(const char* name, const char* doc,
                     PyMethodDef* methods) noexcept {
  return Py_InitModule3(name, methods, doc);
}
}  // namespace python_bridge_tracer

#endif
