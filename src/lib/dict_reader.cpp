#include "dict_reader.h"

#include "python_object_wrapper.h"
#include "python_string_wrapper.h"
#include "python_bridge_error.h"

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
DictReader:: DictReader(PyObject* dict) noexcept
  : dict_{dict} {}

//--------------------------------------------------------------------------------------------------
// LookupKey
//--------------------------------------------------------------------------------------------------
opentracing::expected<opentracing::string_view> DictReader::LookupKey(
    opentracing::string_view key) const {
  PythonObjectWrapper py_key =
      PyUnicode_FromStringAndSize(key.data(), static_cast<Py_ssize_t>(key.size()));
  if (!py_key) {
    return opentracing::make_unexpected(python_error);
  }
  auto value = PyDict_GetItem(dict_, py_key);
  if (value == nullptr) {
    return opentracing::make_unexpected(opentracing::key_not_found_error);
  }
  PythonStringWrapper value_str{value};
  if (!value_str) {
    return opentracing::make_unexpected(python_error);
  }
  lookup_value_ = static_cast<opentracing::string_view>(value_str);
  return opentracing::string_view{lookup_value_};
}

//--------------------------------------------------------------------------------------------------
// ForeachKey
//--------------------------------------------------------------------------------------------------
opentracing::expected<void> DictReader::ForeachKey(Callback callback) const {
  PyObject* key;
  PyObject* value;
  Py_ssize_t position = 0;
  while (PyDict_Next(dict_, &position, &key, &value) == 1) {
    PythonStringWrapper key_str{key};
    if (!key_str) {
      return opentracing::make_unexpected(python_error);
    }
    PythonStringWrapper value_str{value};
    if (!value_str) {
      return opentracing::make_unexpected(python_error);
    }
    auto was_successful = callback(key_str, value_str);
    if (!was_successful) {
      return was_successful;
    }
  }
  return {};
}
} // namespace python_bridge_tracer
