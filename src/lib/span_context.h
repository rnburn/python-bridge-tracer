#pragma once

#include <memory>

#include <Python.h>

#include "span_context_bridge.h"

#include "opentracing/span.h"

namespace python_bridge_tracer {
/**
 * Make a python span context from a span bridge
 * @param span_context_bridge the C++ span context bridge
 * @return an OpenTracing span context object
 */
PyObject* makeSpanContext(
    std::unique_ptr<SpanContextBridge>&& span_context_bridge) noexcept;

/**
 * Check if an object is a span context
 * @param object the object to check
 * @return true if object is a span context
 */
bool isSpanContext(PyObject* object) noexcept;

/**
 * Get the span context bridge associated with a python span context object
 * @param object the python span context
 * @return the associated SpanContextBridge
 */
SpanContextBridge getSpanContext(PyObject* object) noexcept;

/**
 * Setup the python span context class
 * @param module the module to add the class to
 * @return true if succuessful
 */
bool setupSpanContextClass(PyObject* module) noexcept;
} // namespace python_bridge_tracer
