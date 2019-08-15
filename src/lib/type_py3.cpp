#include "python_bridge_tracer/type.h"

#include <cassert>
#include <vector>

#include "python_bridge_tracer/version.h"

#ifdef PYTHON_BRIDGE_TRACER_PY3

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// makeTypeSlots
//--------------------------------------------------------------------------------------------------
static std::vector<PyType_Slot> makeTypeSlots(
    const TypeDescription& type_description) noexcept {
  std::vector<PyType_Slot> result;
  if (type_description.doc != nullptr) {
    result.push_back(PyType_Slot{Py_tp_doc, type_description.doc});
  }
  if (type_description.dealloc != nullptr) {
    result.push_back(PyType_Slot{Py_tp_dealloc, type_description.dealloc});
  }
  if (type_description.methods != nullptr) {
    result.push_back(PyType_Slot{Py_tp_methods, type_description.methods});
  }
  if (type_description.getset != nullptr) {
    result.push_back(PyType_Slot{Py_tp_getset, type_description.getset});
  }
  result.push_back(PyType_Slot{0, nullptr});
  return result;
}

//--------------------------------------------------------------------------------------------------
// makeType
//--------------------------------------------------------------------------------------------------
PyObject* makeTypeImpl(const TypeDescription& type_description) noexcept {
  assert(type_description.name != nullptr);
  assert(type_description.size > 0);
  auto type_slots = makeTypeSlots(type_description);
  PyType_Spec type_spec = {type_description.name,
                           static_cast<int>(type_description.size), 0,
                           Py_TPFLAGS_DEFAULT, type_slots.data()};
  return PyType_FromSpec(&type_spec);
}
}  // namespace python_bridge_tracer
#endif
