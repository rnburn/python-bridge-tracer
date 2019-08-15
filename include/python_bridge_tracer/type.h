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
      PyVarObject_HEAD_INIT(nullptr, 0)
      type_description.name,     /* tp_name */
      static_cast<Py_ssize_t>(type_description.size),     /* tp_basicsize */
      0,                         /* tp_itemsize */
      reinterpret_cast<destructor>(type_description.dealloc),  /* tp_dealloc */
      nullptr,                         /* tp_print */
      nullptr,                         /* tp_getattr */
      nullptr,                         /* tp_setattr */
      nullptr,                         /* tp_compare */
      nullptr,                         /* tp_repr */
      nullptr,                         /* tp_as_number */
      nullptr,                         /* tp_as_sequence */
      nullptr,                         /* tp_as_mapping */
      nullptr,                         /* tp_hash */
      nullptr,                         /* tp_call */
      nullptr,                         /* tp_str */
      nullptr,                         /* tp_getattro */
      nullptr,                         /* tp_setattro */
      nullptr,                         /* tp_as_buffer */
      Py_TPFLAGS_DEFAULT,        /* tp_flags */
      static_cast<char*>(type_description.doc),      /* tp_doc */
      nullptr,                         /* tp_traverse */
      nullptr,                         /* tp_clear */
      nullptr,                         /* tp_richcompare */
      0,                         /* tp_weaklistoffset */
      nullptr,                         /* tp_iter */
      nullptr,                         /* tp_iternext */
      static_cast<PyMethodDef*>(type_description.methods),  /* tp_methods */
      nullptr,                         /* tp_members */
      static_cast<PyGetSetDef*>(type_description.getset),   /* tp_getset */
      nullptr,                         /* tp_base */
      nullptr,                         /* tp_dict */
      nullptr,                         /* tp_descr_get */
      nullptr,                         /* tp_descr_set */
      0,                         /* tp_dictoffset */
      nullptr,                         /* tp_init */
      nullptr,                         /* tp_alloc */
      nullptr,                         /* tp_new */
    };
// clang-format on
  return makeTypeImpl(type);
}
#endif
}  // namespace python_bridge_tracer
