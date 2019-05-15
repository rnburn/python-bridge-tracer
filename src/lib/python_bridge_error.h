#pragma once

#include <system_error>

namespace python_bridge_tracer {
/**
 * Error category used for python code errors.
 */
const std::error_category& python_bridge_error_category() noexcept;

/**
 * Indicate a failure from the python code.
 *
 * Note: If this std::error_code is used, it means that the active python
 * exception is already set.
 */
const std::error_code python_error(1, python_bridge_error_category());
} // namespace python_bridge_tracer
