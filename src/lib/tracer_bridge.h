#pragma once

#include <Python.h>

#include "span_bridge.h"

#include "opentracing/tracer.h"

namespace python_bridge_tracer {
/**
 * Translates OpenTracing-Python tracer methods to their OpenTracing-C++ equivalents.
 */
class TracerBridge {
 public:
   explicit TracerBridge(std::shared_ptr<opentracing::Tracer> tracer) noexcept;

   /**
    * @return the OpenTracing-C++ tracer associated with the bridge.
    */
   opentracing::Tracer& tracer() noexcept { return *tracer_; }

   /**
    * Create a new span.
    *
    * @param operation_name the operation name for the span.
    * @param scope_manager the python scope manager object
    * @param parent an optional parent for the span
    * @param references a list of span references
    * @tags a dictionary of tags to add to the span
    * @param start_time the start time of the span in seconds since epoch
    * @param ignore_active_span whether add a child_of reference to the active span.
    * @param a SpanBridge for the newly created span.
    */
   std::unique_ptr<SpanBridge> makeSpan(opentracing::string_view operation_name,
                                        PyObject* scope_manager,
                                        PyObject* parent, PyObject* references,
                                        PyObject* tags, double start_time,
                                        bool ignore_active_span) noexcept;

  private:
   std::shared_ptr<opentracing::Tracer> tracer_;
};
} // namespace python_bridge_tracer
