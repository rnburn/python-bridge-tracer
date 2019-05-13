#pragma once

#include <Python.h>

#include <opentracing/propagation.h>

namespace python_bridge_tracer {
class DictReader final : public opentracing::HTTPHeadersReader {
 public:
  using Callback = std::function<opentracing::expected<void>(
      opentracing::string_view, opentracing::string_view)>;

  explicit DictReader(PyObject* dict) noexcept;

  opentracing::expected<void> ForeachKey(Callback callback) const override;

 private:
  PyObject* dict_;
};
} // namespace python_bridge_tracer
