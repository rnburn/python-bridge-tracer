#pragma once

#ifndef PYTHON_BRIDGE_TRACER_MODULE
#define PYTHON_BRIDGE_TRACER_MODULE "bridge_tracer"
#endif

#include <Python.h>
#include <opentracing/tracer.h>

#include "python_bridge_tracer/module_py3.h"

namespace python_bridge_tracer {
/**
 * Make an OpenTracing python tracer from a C++ tracer and a scope manager
 * @param tracer the C++ tracer
 * @param scope_manager a scope manager object
 * @return the OpenTracing python tracer object
 */
PyObject* makeTracer(std::shared_ptr<opentracing::Tracer> tracer, PyObject* scope_manager) noexcept;

/**
 * An extension method not part of the official OpenTracing API but commonly
 * added. Tracers can implement this function or optionally do nothing if flush
 * isn't supported.
 *
 * @param tracer the tracer
 * @param the flush timeout or 0 if none was provided.
 */
void flush(opentracing::Tracer& tracer, std::chrono::microseconds timeout) noexcept;

/**
 * Setup the OpenTracing python classes.
 * @param module the module to add the classes to
 * @return true if successful
 */
bool setupClasses(PyObject* module) noexcept;

PyObject* makeModule(const char* name, const char* doc, PyMethodDef* methods) noexcept;
} // namespace python_bridge_tracer
