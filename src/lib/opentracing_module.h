#pragma once

#include <Python.h>

namespace python_bridge_tracer {
/**
 * Lookup the thread local scope manager from the python OpenTracing module.
 * @return the scope manager object
 */
PyObject* getThreadLocalScopeManager() noexcept;

/**
 * @return the python object for opentracing.UnsupportedFormatException
 */
PyObject* getUnsupportedFormatException() noexcept;

/**
 * @return the python object for opentracing.InvalidCarrierException
 */
PyObject* getInvalidCarrierException() noexcept;

/**
 * @return the python object for opentracing.SpanContextCorruptedException
 */
PyObject* getSpanContextCorruptedException() noexcept;
} // namespace python_bridge_tracer
