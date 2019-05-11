#include "span_context_bridge.h"

#include "utility.h"

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
SpanContextBridge::SpanContextBridge(
    std::shared_ptr<const opentracing::Span> span) noexcept
    : span_{std::move(span)} {}

SpanContextBridge::SpanContextBridge(
    std::unique_ptr<const opentracing::SpanContext>&& span_context) noexcept
    : span_context_{std::move(span_context)} {}

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
  auto result = PyDict_New();
  if (result == nullptr) {
    return nullptr;
  }
  bool error = false;
  span_context().ForeachBaggageItem(
      [&](const std::string& key, const std::string& value) {
        auto py_key = PyUnicode_FromStringAndSize(
            key.data(), static_cast<Py_ssize_t>(key.size()));
        if (py_key == nullptr) {
          error = true;
          return false;
        }
        auto cleanup_key = finally([py_key] { Py_DECREF(py_key); });
        auto py_value = PyUnicode_FromStringAndSize(
            value.data(), static_cast<Py_ssize_t>(value.size()));
        if (py_value == nullptr) {
          error = true;
          return false;
        }
        auto cleanup_value = finally([py_value] { Py_DECREF(py_value); });
        if (PyDict_SetItem(result, py_key, py_value) != 0) {
          error = true;
          return false;
        }
        return true;
      });
  if (error) {
    Py_DECREF(result);
    return nullptr;
  }
  return result;
}
}  // namespace python_bridge_tracer
