#include "python_unicode_object.h"

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
PythonUnicodeObject::PythonUnicodeObject(const char* data,
                                         size_t length) noexcept {
  object_ = PyUnicode_FromStringAndSize(data, static_cast<Py_ssize_t>(length));
}

//--------------------------------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------------------------------
PythonUnicodeObject::~PythonUnicodeObject() noexcept {
  if (object_ != nullptr) {
    Py_DECREF(object_);
  }
}
}  // namespace python_bridge_tracer
