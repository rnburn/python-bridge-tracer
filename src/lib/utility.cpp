#include "python_bridge_tracer/utility.h"

#include "python_bridge_tracer/python_object_wrapper.h"

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// getModuleAttribute
//--------------------------------------------------------------------------------------------------
PyObject* getModuleAttribute(const char* module_name,
                             const char* attribute) noexcept {
  PythonObjectWrapper module = PyImport_ImportModule(module_name);
  if (module.error()) {
    return nullptr;
  }
  return PyObject_GetAttrString(module, attribute);
}

//--------------------------------------------------------------------------------------------------
// toTimestamp
//--------------------------------------------------------------------------------------------------
std::chrono::system_clock::time_point toTimestamp(double py_timestamp) noexcept {
  auto time_since_epoch =
      std::chrono::nanoseconds{static_cast<uint64_t>(1e9 * py_timestamp)};
  return std::chrono::system_clock::time_point{
      std::chrono::duration_cast<std::chrono::system_clock::duration>(
          time_since_epoch)};
}
} // namespace python_bridge_tracer
