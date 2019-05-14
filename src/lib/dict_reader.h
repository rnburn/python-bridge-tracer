#pragma once

#include <Python.h>

#include "python_string_wrapper.h"

#include <opentracing/propagation.h>

namespace python_bridge_tracer {
class DictReader final : public opentracing::HTTPHeadersReader {
 public:
  using Callback = std::function<opentracing::expected<void>(
      opentracing::string_view, opentracing::string_view)>;

  explicit DictReader(PyObject* dict) noexcept;

  opentracing::expected<opentracing::string_view> LookupKey(opentracing::string_view key) const override;

  opentracing::expected<void> ForeachKey(Callback callback) const override;

 private:
  PyObject* dict_;
  mutable PythonStringWrapper lookup_value_;
};
} // namespace python_bridge_tracer
