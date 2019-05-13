#include "python_bridge_tracer/module.h"

#include "utility.h"
#include "python_object_wrapper.h"

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// getModuleAttribute
//--------------------------------------------------------------------------------------------------
static PyObject* getModuleAttribute(const char* module_name,
                                    const char* attribute) noexcept {
  PythonObjectWrapper module = PyImport_ImportModule(module_name);
  if (!module) {
    return nullptr;
  }
  return PyObject_GetAttrString(module, attribute);
}

//--------------------------------------------------------------------------------------------------
// getThreadLocalScopeManager
//--------------------------------------------------------------------------------------------------
PyObject* getThreadLocalScopeManager() noexcept {
  PythonObjectWrapper scope_manager = getModuleAttribute(
      "opentracing.scope_managers", "ThreadLocalScopeManager");
  if (!scope_manager) {
    return nullptr;
  }
  return PyObject_CallObject(scope_manager, nullptr);
}

//--------------------------------------------------------------------------------------------------
// getUnsupportedFormatException
//--------------------------------------------------------------------------------------------------
PyObject* getUnsupportedFormatException() noexcept {
  return getModuleAttribute("opentracing", "UnsupportedFormatException");
}

//--------------------------------------------------------------------------------------------------
// getInvalidCarrierException
//--------------------------------------------------------------------------------------------------
PyObject* getInvalidCarrierException() noexcept {
  return getModuleAttribute("opentracing", "InvalidCarrierException");
}

//--------------------------------------------------------------------------------------------------
// getSpanContextCorruptedException
//--------------------------------------------------------------------------------------------------
PyObject* getSpanContextCorruptedException() noexcept {
  return getModuleAttribute("opentracing", "SpanContextCorruptedException");
}
} // namespace python_bridge_tracer
