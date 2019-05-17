#include "python_bridge_tracer/module.h"

#include "python_bridge_tracer/utility.h"
#include "python_object_wrapper.h"

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// getThreadLocalScopeManager
//--------------------------------------------------------------------------------------------------
PyObject* getThreadLocalScopeManager() noexcept {
  PythonObjectWrapper scope_manager = getModuleAttribute(
      "opentracing.scope_managers", "ThreadLocalScopeManager");
  if (scope_manager.error()) {
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
