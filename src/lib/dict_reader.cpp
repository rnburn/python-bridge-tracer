#include "dict_reader.h"

#include "python_string_wrapper.h"
#include "python_bridge_error.h"

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
DictReader:: DictReader(PyObject* dict) noexcept
  : dict_{dict} {}

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
