#include <iostream>

#include <Python.h>

#include "python_bridge_tracer/module.h"

#include "dynamic_tracer.h"

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// loadTracer
//--------------------------------------------------------------------------------------------------
static PyObject* loadTracer(PyObject* /*self*/, PyObject* args, PyObject* keywords) noexcept try {
  static char* keyword_names[] = {const_cast<char*>("library"),
                                  const_cast<char*>("config"),
                                  const_cast<char*>("scope_manager"), nullptr};
  char* library;
  char* config;
  PyObject* scope_manager = nullptr;
  if (PyArg_ParseTupleAndKeywords(args, keywords, "ss|O:load_tracer", keyword_names, 
        &library, &config, &scope_manager) == 0) {
    return nullptr;
  }
  return makeTracer(makeDynamicTracer(library, config), scope_manager);
} catch(const std::exception& e) {
  PyErr_Format(PyExc_RuntimeError, "failed to load tracer: %s", e.what());
  return nullptr;
}

//--------------------------------------------------------------------------------------------------
// flush
//--------------------------------------------------------------------------------------------------
void flush(opentracing::Tracer& /*tracer*/, std::chrono::microseconds /*timeout*/) noexcept {
  // Do nothing not part of the OpenTracing API.
}

//--------------------------------------------------------------------------------------------------
// ModuleMethods
//--------------------------------------------------------------------------------------------------
static PyMethodDef ModuleMethods[] = {
    {"load_tracer", reinterpret_cast<PyCFunction>(loadTracer),
     METH_VARARGS | METH_KEYWORDS, PyDoc_STR("loads a C++ opentracing plugin")},
    {nullptr, nullptr}};
} // namespace python_bridge_tracer

//--------------------------------------------------------------------------------------------------
// PyInit_bridge_tracer
//--------------------------------------------------------------------------------------------------
extern "C" {
PY_BRIDGE_DEFINE_MODULE(bridge_tracer) {
  using namespace python_bridge_tracer;
  auto module = makeModule("bridge_tracer", "bridge a c++ tracer", ModuleMethods);
  if (module == nullptr) {
    return nullptr;
  }
  if (!setupClasses(module)) {
    return nullptr;
  }
  return module;
}
} // extern "C"
