#include "span_context_bridge.h"

#include "python_object_wrapper.h"
#include "python_bridge_tracer/utility.h"

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
SpanContextBridge::SpanContextBridge(
    std::shared_ptr<const opentracing::Span> span) noexcept
    : span_{std::move(span)} {}

SpanContextBridge::SpanContextBridge(
    std::unique_ptr<const opentracing::SpanContext>&& span_context) noexcept
    : span_context_{span_context.release()} {}

//--------------------------------------------------------------------------------------------------
// span_context
//--------------------------------------------------------------------------------------------------
const opentracing::SpanContext& SpanContextBridge::span_context() const
    noexcept {
  if (span_ != nullptr) {
    return span_->context();
  }
  return *span_context_;
}

//--------------------------------------------------------------------------------------------------
// getBaggageAsPyDict
//--------------------------------------------------------------------------------------------------
PyObject* SpanContextBridge::getBaggageAsPyDict() const noexcept {
  PythonObjectWrapper result = PyDict_New();
  if (result.error()) {
    return nullptr;
  }
  bool error = false;
  span_context().ForeachBaggageItem(
      [&](const std::string& key, const std::string& value) {
        PythonObjectWrapper py_key = PyUnicode_FromStringAndSize(
            key.data(), static_cast<Py_ssize_t>(key.size()));
        if (py_key.error()) {
          error = true;
          return false;
        }
        PythonObjectWrapper py_value = PyUnicode_FromStringAndSize(
            value.data(), static_cast<Py_ssize_t>(value.size()));
        if (py_value.error()) {
          error = true;
          return false;
        }
        if (PyDict_SetItem(result, py_key, py_value) != 0) {
          error = true;
          return false;
        }
        return true;
      });
  if (error) {
    return nullptr;
  }
  return result.release();
}
}  // namespace python_bridge_tracer
