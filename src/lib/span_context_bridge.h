#pragma once

#include <memory>

#include <Python.h>

#include "opentracing/span.h"


namespace python_bridge_tracer {
/**
 * Translates OpenTracing-Python span context methods to their OpenTracing-C++ equivalents.
 */
class SpanContextBridge {
 public:
   explicit SpanContextBridge(std::shared_ptr<const opentracing::Span> span) noexcept;

   explicit SpanContextBridge(
       std::unique_ptr<const opentracing::SpanContext>&& span_context) noexcept;

   /**
    * @return the OpenTracing-C++ span context associated with the bridge.
    */
   const opentracing::SpanContext& span_context() const noexcept;

   /**
    * @return the span context's baggage as a python dictionary.
    */
   PyObject* getBaggageAsPyDict() const noexcept;

 private:
   std::shared_ptr<const opentracing::Span> span_;
   std::shared_ptr<const opentracing::SpanContext> span_context_;
};
} // namespace python_bridge_tracer
