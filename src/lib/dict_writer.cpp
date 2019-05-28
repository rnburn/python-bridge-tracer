#include "dict_writer.h"

#include "python_bridge_tracer/python_object_wrapper.h"
#include "python_bridge_error.h"

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
DictWriter::DictWriter(PyObject* dict) noexcept : dict_{dict} {}

//--------------------------------------------------------------------------------------------------
// Set
//--------------------------------------------------------------------------------------------------
opentracing::expected<void> DictWriter::Set(
       opentracing::string_view key,
       opentracing::string_view value) const {
  PythonObjectWrapper py_key =
      PyUnicode_FromStringAndSize(key.data(), static_cast<Py_ssize_t>(key.size()));
  if (py_key.error()) {
    return opentracing::make_unexpected(python_error);
  }
  PythonObjectWrapper py_value =
      PyUnicode_FromStringAndSize(value.data(), static_cast<Py_ssize_t>(value.size()));
  if (py_value.error()) {
    return opentracing::make_unexpected(python_error);
  }
  if (PyDict_SetItem(dict_, py_key, py_value) != 0) {
    return opentracing::make_unexpected(python_error);
  }
  return {};
}
} // namespace python_bridge_tracer
