#include "python_bridge_tracer/utility.h"

#include "python_bridge_tracer/version.h"

#ifdef PYTHON_BRIDGE_TRACER_PY3

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// isString
//--------------------------------------------------------------------------------------------------
bool isString(PyObject* obj) noexcept {
  return static_cast<bool>(PyUnicode_Check(obj));
}

//--------------------------------------------------------------------------------------------------
// isInt
//--------------------------------------------------------------------------------------------------
bool isInt(PyObject* obj) noexcept {
  return static_cast<bool>(PyLong_Check(obj));
}

//--------------------------------------------------------------------------------------------------
// toLong
//--------------------------------------------------------------------------------------------------
bool toLong(PyObject* obj, long& value) noexcept {
  value = PyLong_AsLong(obj);
  return !(value == -1 && PyErr_Occurred() != nullptr);
}

//--------------------------------------------------------------------------------------------------
// toPyString
//--------------------------------------------------------------------------------------------------
PyObject* toPyString(opentracing::string_view s) noexcept {
  return PyUnicode_FromStringAndSize(s.data(), static_cast<Py_ssize_t>(s.size()));
}

//--------------------------------------------------------------------------------------------------
// freeSelf
//--------------------------------------------------------------------------------------------------
void freeSelf(PyObject* self) noexcept {
  // The limited api doesn't seem to provide any way to access tp_free, but
  // according to
  // https://docs.python.org/3/c-api/typeobj.html#c.PyTypeObject.tp_dealloc,
  // it's safe to free self this way so long as the class isn't used as a base.
  PyObject_Free(static_cast<void*>(self));
}
} // namespace python_bridge_tracer

#endif
