#include "to_string.h"

#include <cassert>

#include "python_bridge_tracer/utility.h"
#include "python_bridge_tracer/python_object_wrapper.h"
#include "python_string_wrapper.h"

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// unicodeToString
//--------------------------------------------------------------------------------------------------
static bool unicodeToString(PyObject* object, std::string& result) noexcept {
  PythonStringWrapper str{object};
  if (str.error()) {
    return false;
  }
  auto sv = static_cast<opentracing::string_view>(str);
  result.assign(sv.data(), sv.size());
  return true;
}

//--------------------------------------------------------------------------------------------------
// convertToString
//--------------------------------------------------------------------------------------------------
static bool convertToString(PyObject* object, std::string& result) noexcept {
  PythonObjectWrapper str_function = getModuleAttribute("builtins", "str");
  if (str_function.error()) {
    return false;
  }
  PythonObjectWrapper args = Py_BuildValue("(O)", object);
  if (args.error()) {
    return false;
  }
  PythonObjectWrapper str_result = PyObject_CallObject(str_function, args);
  if (str_result.error()) {
    return false;
  }
  return unicodeToString(str_result, result);
}

//--------------------------------------------------------------------------------------------------
// toString
//--------------------------------------------------------------------------------------------------
bool toString(PyObject* object, std::string& result) noexcept {
  if(PyUnicode_Check(object) == 1) {
    return unicodeToString(object, result);
  }
  if (PyBytes_Check(object) == 1) {
    char* data;
    Py_ssize_t length;
    if (PyBytes_AsStringAndSize(object, &data, &length) == -1) {
      return false;
    }
    result.assign(data, static_cast<size_t>(length));
    return true;
  }
  return convertToString(object, result);
}
} // namespace python_bridge_tracer
