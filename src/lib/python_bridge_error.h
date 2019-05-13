#pragma once

#include <system_error>

namespace python_bridge_tracer {
const std::error_category& python_bridge_error_category() noexcept;

const std::error_code python_error(1, python_bridge_error_category());
} // namespace python_bridge_tracer
