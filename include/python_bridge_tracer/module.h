#pragma once

#ifndef PYTHON_BRIDGE_TRACER_MODULE
#define PYTHON_BRIDGE_TRACER_MODULE "bridge_tracer"
#endif

#include <Python.h>
#include <opentracing/tracer.h>

namespace python_bridge_tracer {
/**
 * Make an OpenTracing python tracer from a C++ tracer and a scope manager
 * @param tracer the C++ tracer
 * @param scope_manager a scope manager object
 * @return the OpenTracing python tracer object
 */
PyObject* makeTracer(std::shared_ptr<opentracing::Tracer> tracer, PyObject* scope_manager) noexcept;

/**
 * Setup the OpenTracing python classes.
 * @param module the module to add the classes to
 * @return true if successful
 */
bool setupClasses(PyObject* module) noexcept;
} // namespace python_bridge_tracer
