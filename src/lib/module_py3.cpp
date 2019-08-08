#include "python_bridge_tracer/module.h"
#include "python_bridge_tracer/version.h"

#ifdef PYTHON_BRIDGE_TRACER_PY3

namespace python_bridge_tracer {
PyObject* makeModule(const char* name, const char* doc,
                     PyMethodDef* methods) noexcept {
  static PyModuleDef module_definition = {PyModuleDef_HEAD_INIT, name, doc, -1,
                                          methods};
  return PyModule_Create(&module_definition);
}
}  // namespace python_bridge_tracer

#endif
