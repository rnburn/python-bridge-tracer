#include "dict_writer.h"

#include "python_unicode_object.h"
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
  PythonUnicodeObject py_key{key.data(), key.size()};
  if (py_key.object() == nullptr) {
    return opentracing::make_unexpected(python_error);
  }
  PythonUnicodeObject py_value{value.data(), value.size()};
  if (py_value.object() == nullptr) {
    return opentracing::make_unexpected(python_error);
  }
  if (PyDict_SetItem(dict_, py_key.object(), py_value.object()) != 0) {
    return opentracing::make_unexpected(python_error);
  }
  return {};
}
} // namespace python_bridge_tracer
