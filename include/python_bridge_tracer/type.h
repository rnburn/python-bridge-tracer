#pragma once

#include <Python.h>

#include "python_bridge_tracer/version.h"

namespace python_bridge_tracer {
struct TypeDescription {
  const char* name = nullptr;
  size_t size = 0;
  void* doc = nullptr;
  void* dealloc = nullptr;
  void* methods = nullptr;
  void* getset = nullptr;
};

#ifdef PYTHON_BRIDGE_TRACER_PY3
PyObject* makeTypeImpl(const TypeDescription& type_description) noexcept;

template <class T>
PyObject* makeType(const TypeDescription& type_description) noexcept {
  return makeTypeImpl(type_description);
}
#else
PyObject* makeTypeImpl(PyTypeObject& type) noexcept;

template <class T>
PyObject* makeType(const TypeDescription& type_description) noexcept {
// clang-format off
  static PyTypeObject type = {
      PyVarObject_HEAD_INIT(NULL, 0)
      type_description.name,     /* tp_name */
      static_cast<Py_ssize_t>(type_description.size),     /* tp_basicsize */
      0,                         /* tp_itemsize */
      reinterpret_cast<destructor>(type_description.dealloc),  /* tp_dealloc */
      0,                         /* tp_print */
      0,                         /* tp_getattr */
      0,                         /* tp_setattr */
      0,                         /* tp_compare */
      0,                         /* tp_repr */
      0,                         /* tp_as_number */
      0,                         /* tp_as_sequence */
      0,                         /* tp_as_mapping */
      0,                         /* tp_hash */
      0,                         /* tp_call */
      0,                         /* tp_str */
      0,                         /* tp_getattro */
      0,                         /* tp_setattro */
      0,                         /* tp_as_buffer */
      Py_TPFLAGS_DEFAULT,        /* tp_flags */
      static_cast<char*>(type_description.doc),      /* tp_doc */
      0,                         /* tp_traverse */
      0,                         /* tp_clear */
      0,                         /* tp_richcompare */
      0,                         /* tp_weaklistoffset */
      0,                         /* tp_iter */
      0,                         /* tp_iternext */
      static_cast<PyMethodDef*>(type_description.methods),  /* tp_methods */
      0,                         /* tp_members */
      static_cast<PyGetSetDef*>(type_description.getset),   /* tp_getset */
      0,                         /* tp_base */
      0,                         /* tp_dict */
      0,                         /* tp_descr_get */
      0,                         /* tp_descr_set */
      0,                         /* tp_dictoffset */
      0,                         /* tp_init */
      0,                         /* tp_alloc */
      0,                         /* tp_new */
    };
// clang-format on
  return makeTypeImpl(type);
}
#endif
}  // namespace python_bridge_tracer
