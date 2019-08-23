#include "python_bridge_tracer/module.h"

#include "tracer.h"
#include "span_context.h"
#include "span.h"

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// setupClasses
//--------------------------------------------------------------------------------------------------
bool setupClasses(PyObject* module,
    const std::vector<PyMethodDef>& tracer_extension_methods,
    const std::vector<PyGetSetDef>& tracer_extension_getsets) noexcept {
  if (!setupTracerClass(module, tracer_extension_methods,
                        tracer_extension_getsets)) {
    return false;
  }
  if (!setupSpanContextClass(module)) {
    return false;
  }
  return setupSpanClass(module);
}
} // namespace python_bridge_tracer
