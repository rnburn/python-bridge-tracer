#pragma once

#include <Python.h>

#include <opentracing/propagation.h>

namespace python_bridge_tracer {
class DictWriter final : public opentracing::HTTPHeadersWriter {
 public:
   explicit DictWriter(PyObject* dict) noexcept;

   opentracing::expected<void> Set(
       opentracing::string_view key,
       opentracing::string_view value) const override;

  private:
   PyObject* dict_;
};
} // namespace python_bridge_tracer
