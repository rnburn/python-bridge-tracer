#include "span_context.h"

#include <cassert>

#include "python_bridge_tracer/module.h"

#include "python_bridge_tracer/utility.h"
#include "python_bridge_tracer/type.h"

static PyObject* SpanContextType;

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// SpanContextObject
//--------------------------------------------------------------------------------------------------
namespace {
struct SpanContextObject {
  // clang-format off
  PyObject_HEAD
  SpanContextBridge* span_context_bridge;
  // clang-format off
};
} // namespace

//--------------------------------------------------------------------------------------------------
// deallocSpanContext
//--------------------------------------------------------------------------------------------------
static void deallocSpanContext(SpanContextObject* self) noexcept {
  delete self->span_context_bridge;
  freeSelf(reinterpret_cast<PyObject*>(self));
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
    {const_cast<char*>("baggage"), reinterpret_cast<getter>(getBaggage), nullptr,
     const_cast<char*>(PyDoc_STR("return the context's baggage"))},
    {nullptr}};

//--------------------------------------------------------------------------------------------------
// setupSpanContextClass
//--------------------------------------------------------------------------------------------------
bool setupSpanContextClass(PyObject* module) noexcept {
  TypeDescription type_description;
  type_description.name = PYTHON_BRIDGE_TRACER_MODULE "._SpanContext";
  type_description.size = sizeof(SpanContextObject);
  type_description.doc = toVoidPtr("CppBridgeSpanContext");
  type_description.dealloc = toVoidPtr(deallocSpanContext);
  type_description.getset = toVoidPtr(SpanContextGetSetList);
  auto span_context_type = makeType<SpanContextObject>(type_description);
  if (span_context_type == nullptr) {
    return false;
  }
  SpanContextType = span_context_type;
  auto rcode = PyModule_AddObject(module, "_SpanContext", span_context_type);
  return rcode == 0;
}
} // namespace python_bridge_tracer
