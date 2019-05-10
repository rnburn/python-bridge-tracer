#pragma once

#include <Python.h>

#include "span_bridge.h"
#include "span_context_bridge.h"

#include "opentracing/span.h"
#include "opentracing/tracer.h"

namespace python_bridge_tracer {
/**
 * Make a python span context from a span bridge
 * @param span_bridge the C++ span bridge
 * @param tracer the tracer used to create the span
 * @return an OpenTracing span object
 */
PyObject* makeSpan(std::unique_ptr<SpanBridge>&& span_bridge, PyObject* tracer) noexcept;

/**
 * Check if an object is a span
 * @param object the object to check
 * @return true if object is a span
 */
bool isSpan(PyObject* object) noexcept;

/**
 * Get the span context bridge associated with a python span object
 * @param object the python span
 * @return an associated SpanContextBridge
 */
SpanContextBridge getSpanContextFromSpan(PyObject* object) noexcept;

/**
 * Setup the python span class
 * @param module the module to add the class to
 * @return true if succuessful
 */
bool setupSpanClass(PyObject* module) noexcept;
} // namespace python_bridge_tracer
