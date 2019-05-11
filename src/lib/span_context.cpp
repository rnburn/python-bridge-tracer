#include "span_context.h"

#include <cassert>

#include "python_bridge_tracer/module.h"

#include "utility.h"

static PyObject* SpanContextType;

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// SpanContextObject
//--------------------------------------------------------------------------------------------------
namespace {
struct SpanContextObject {
  PyObject_HEAD
  SpanContextBridge* span_context_bridge;
};
} // namespace

//--------------------------------------------------------------------------------------------------
// deallocSpanContext
//--------------------------------------------------------------------------------------------------
static void deallocSpanContext(SpanContextObject* self) noexcept {
  delete self->span_context_bridge;
}

//--------------------------------------------------------------------------------------------------
// makeSpanContext
//--------------------------------------------------------------------------------------------------
PyObject* makeSpanContext(
    std::unique_ptr<SpanContextBridge>&& span_context_bridge) noexcept {
  auto result = newPythonObject<SpanContextObject>(SpanContextType);
  if (result == nullptr) {
    return nullptr;
  }
  result->span_context_bridge = span_context_bridge.release();
  return reinterpret_cast<PyObject*>(result);
}

//--------------------------------------------------------------------------------------------------
// isSpanContext
//--------------------------------------------------------------------------------------------------
bool isSpanContext(PyObject* object) noexcept {
  return object->ob_type == reinterpret_cast<PyTypeObject*>(SpanContextType);
}

//--------------------------------------------------------------------------------------------------
// getSpanContext
//--------------------------------------------------------------------------------------------------
SpanContextBridge getSpanContext(PyObject* object) noexcept {
  assert(isSpanContext(object));
  return *reinterpret_cast<SpanContextObject*>(object)->span_context_bridge;
}

//--------------------------------------------------------------------------------------------------
// getBaggage
//--------------------------------------------------------------------------------------------------
static PyObject* getBaggage(SpanContextObject* self, PyObject* /*ignored*/) noexcept {
  return self->span_context_bridge->getBaggageAsPyDict();
}


//--------------------------------------------------------------------------------------------------
// SpanContextGetSetList
//--------------------------------------------------------------------------------------------------
static PyGetSetDef SpanContextGetSetList[] = {
    {"baggage", reinterpret_cast<getter>(getBaggage), nullptr,
     PyDoc_STR("return the context's baggage")},
    {nullptr}};

//--------------------------------------------------------------------------------------------------
// SpanContextTypeSlots
//--------------------------------------------------------------------------------------------------
static PyType_Slot SpanContextTypeSlots[] = {
    {Py_tp_doc, toVoidPtr("CppBridgeSpanContext")},
    {Py_tp_dealloc, toVoidPtr(deallocSpanContext)},
    {Py_tp_getset, toVoidPtr(SpanContextGetSetList)},
    {0, nullptr}};

//--------------------------------------------------------------------------------------------------
// SpanContextTypeSpec
//--------------------------------------------------------------------------------------------------
static PyType_Spec SpanContextTypeSpec = {PYTHON_BRIDGE_TRACER_MODULE "._SpanContext",
                                     sizeof(SpanContextObject), 0,
                                     Py_TPFLAGS_DEFAULT, SpanContextTypeSlots};

//--------------------------------------------------------------------------------------------------
// setupSpanContextClass
//--------------------------------------------------------------------------------------------------
bool setupSpanContextClass(PyObject* module) noexcept {
  auto span_context_type = PyType_FromSpec(&SpanContextTypeSpec);
  if (span_context_type == nullptr) {
    return false;
  }
  SpanContextType = span_context_type;
  auto rcode = PyModule_AddObject(module, "_SpanContext", span_context_type);
  return rcode == 0;
}
} // namespace python_bridge_tracer
