#pragma once

namespace python_bridge_tracer {
/**
 * Lookup the thread local scope manager from the python OpenTracing module.
 * @return the scope manager object
 */
PyObject* getThreadLocalScopeManager() noexcept;

PyObject* getUnsupportedFormatException() noexcept;
} // namespace python_bridge_tracer
