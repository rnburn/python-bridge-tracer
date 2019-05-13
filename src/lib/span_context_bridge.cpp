#include "span_context_bridge.h"

#include "utility.h"
#include "python_unicode_object.h"

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
        PythonUnicodeObject py_key{key.data(), key.size()};
        if (py_key.object() == nullptr) {
          error = true;
          return false;
        }
        PythonUnicodeObject py_value{value.data(), value.size()};
        if (py_value.object() == nullptr) {
          error = true;
          return false;
        }
        if (PyDict_SetItem(result, py_key.object(), py_value.object()) != 0) {
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
