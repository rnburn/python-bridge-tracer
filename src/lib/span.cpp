#include "span.h"

#include "python_bridge_tracer/module.h"

#include "span_bridge.h"
#include "span_context.h"
#include "python_bridge_tracer/utility.h"
#include "python_bridge_tracer/type.h"

#include <iostream>

static PyObject* SpanType;

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// SpanObject
//--------------------------------------------------------------------------------------------------
namespace {
struct SpanObject {
  // clang-format off
  PyObject_HEAD 
  SpanBridge* span_bridge;
  PyObject* tracer;
  // clang-formst on
};
}  // namespace

//--------------------------------------------------------------------------------------------------
// deallocSpan
//--------------------------------------------------------------------------------------------------
static void deallocSpan(SpanObject* self) noexcept {
  delete self->span_bridge;
  Py_DECREF(self->tracer);
  PyObject_Free(static_cast<void*>(self));
}

//--------------------------------------------------------------------------------------------------
// setOperationName
//--------------------------------------------------------------------------------------------------
static SpanObject* setOperationName(SpanObject* self, PyObject* args,
                                    PyObject* keywords) noexcept {
  if (!self->span_bridge->setOperationName(args, keywords)) {
    return nullptr;
  }
  Py_INCREF(reinterpret_cast<PyObject*>(self));
  return self;
}

//--------------------------------------------------------------------------------------------------
// setTag
//--------------------------------------------------------------------------------------------------
static SpanObject* setTag(SpanObject* self, PyObject* args,
                          PyObject* keywords) noexcept {
  if (!self->span_bridge->setTag(args, keywords)) {
    return nullptr;
  }
  Py_INCREF(reinterpret_cast<PyObject*>(self));
  return self;
}

//--------------------------------------------------------------------------------------------------
// logKeyValues
//--------------------------------------------------------------------------------------------------
static SpanObject* logKeyValues(SpanObject* self, PyObject* args,
                              PyObject* keywords) noexcept {
  if (!self->span_bridge->logKeyValues(args, keywords)) {
    return nullptr;
  }
  Py_INCREF(reinterpret_cast<PyObject*>(self));
  return self;
}

//--------------------------------------------------------------------------------------------------
// setBaggageItem
//--------------------------------------------------------------------------------------------------
static SpanObject* setBaggageItem(SpanObject* self, PyObject* args,
                                  PyObject* keywords) noexcept {
  if (!self->span_bridge->setBaggageItem(args, keywords)) {
    return nullptr;
  }
  Py_INCREF(reinterpret_cast<PyObject*>(self));
  return self;
}

//--------------------------------------------------------------------------------------------------
// getBaggageItem
//--------------------------------------------------------------------------------------------------
static PyObject* getBaggageItem(SpanObject* self, PyObject* args,
                                PyObject* keywords) noexcept {
  return self->span_bridge->getBaggageItem(args, keywords);
}

//--------------------------------------------------------------------------------------------------
// logEvent
//--------------------------------------------------------------------------------------------------
static SpanObject* logEvent(SpanObject* self, PyObject* args,
                          PyObject* keywords) noexcept {
  if (!self->span_bridge->logEvent(args, keywords)) {
    return nullptr;
  }
  Py_INCREF(reinterpret_cast<PyObject*>(self));
  return self;
}

//--------------------------------------------------------------------------------------------------
// log
//--------------------------------------------------------------------------------------------------
static SpanObject* log(SpanObject* self, PyObject* args,
                     PyObject* keywords) noexcept {
  if (!self->span_bridge->log(args, keywords)) {
    return nullptr;
  }
  Py_INCREF(reinterpret_cast<PyObject*>(self));
  return self;
}

//--------------------------------------------------------------------------------------------------
// finish
//--------------------------------------------------------------------------------------------------
static PyObject* finish(SpanObject* self, PyObject* args,
                        PyObject* keywords) noexcept {
  return self->span_bridge->finish(args, keywords);
}

//--------------------------------------------------------------------------------------------------
// enterContext
//--------------------------------------------------------------------------------------------------
static PyObject* enterContext(PyObject* self, PyObject* /*args*/) noexcept {
  Py_INCREF(self);
  return self;
}

//--------------------------------------------------------------------------------------------------
// exitContext
//--------------------------------------------------------------------------------------------------
static PyObject* exitContext(SpanObject* self, PyObject* args) noexcept {
  return self->span_bridge->exit(args);
}

//--------------------------------------------------------------------------------------------------
// getContext
//--------------------------------------------------------------------------------------------------
static PyObject* getContext(SpanObject* self, PyObject* /*ignored*/) noexcept {
  return makeSpanContext(std::unique_ptr<SpanContextBridge>{
      new SpanContextBridge{self->span_bridge->span()}});
}

//--------------------------------------------------------------------------------------------------
// getTracer
//--------------------------------------------------------------------------------------------------
static PyObject* getTracer(SpanObject* self, PyObject* /*ignored*/) noexcept {
  Py_INCREF(self->tracer);
  return self->tracer;
}

//--------------------------------------------------------------------------------------------------
// SpanMethods
//--------------------------------------------------------------------------------------------------
/* static PyMethodDef SpanMethods[] = { */
/*     {const_cast<char*>("set_operation_name"), reinterpret_cast<PyCFunction>(setOperationName), */
/*      METH_VARARGS | METH_KEYWORDS, const_cast<char*>(PyDoc_STR("set the span's operation name"))}, */
/*     {const_cast<char*>("set_tag"), reinterpret_cast<PyCFunction>(setTag), */
/*      METH_VARARGS | METH_KEYWORDS, const_cast<char*>(PyDoc_STR("set a tag"))}, */
/*     {const_cast<char*>("log_kv"), reinterpret_cast<PyCFunction>(logKeyValues), */
/*      METH_VARARGS | METH_KEYWORDS, const_cast<char*>(PyDoc_STR("log key-values"))}, */
/*     {const_cast<char*>("set_baggage_item"), reinterpret_cast<PyCFunction>(setBaggageItem), */
/*      METH_VARARGS | METH_KEYWORDS, const_cast<char*>(PyDoc_STR("stores a baggage item"))}, */
/*     {const_cast<char*>("get_baggage_item"), reinterpret_cast<PyCFunction>(getBaggageItem), */
/*      METH_VARARGS | METH_KEYWORDS, const_cast<char*>(PyDoc_STR("retrieves a baggage item"))}, */
/*     {const_cast<char*>("log_event"), reinterpret_cast<PyCFunction>(logEvent), */
/*      METH_VARARGS | METH_KEYWORDS, const_cast<char*>(PyDoc_STR("log an event"))}, */
/*     {const_cast<char*>("log"), reinterpret_cast<PyCFunction>(log), METH_VARARGS | METH_KEYWORDS, */
/*      PyDoc_STR("log key-values")}, */
/*     {const_cast<char*>("finish"), reinterpret_cast<PyCFunction>(finish), */
/*      METH_VARARGS | METH_KEYWORDS, const_cast<char*>(PyDoc_STR("finish the span"))}, */
/*     {const_cast<char*>("__enter__"), reinterpret_cast<PyCFunction>(enterContext), METH_NOARGS, */
/*      nullptr}, */
/*     {const_cast<char*>("__exit__"), reinterpret_cast<PyCFunction>(exitContext), METH_VARARGS, */
/*      nullptr}, */
/*     {nullptr, nullptr}}; */
static PyMethodDef SpanMethods[] = {
    {"set_operation_name", reinterpret_cast<PyCFunction>(setOperationName),
     METH_VARARGS | METH_KEYWORDS, PyDoc_STR("set the span's operation name")},
    {"set_tag", reinterpret_cast<PyCFunction>(setTag),
     METH_VARARGS | METH_KEYWORDS, PyDoc_STR("set a tag")},
    {"log_kv", reinterpret_cast<PyCFunction>(logKeyValues),
     METH_VARARGS | METH_KEYWORDS, PyDoc_STR("log key-values")},
    {"set_baggage_item", reinterpret_cast<PyCFunction>(setBaggageItem),
     METH_VARARGS | METH_KEYWORDS, PyDoc_STR("stores a baggage item")},
    {"get_baggage_item", reinterpret_cast<PyCFunction>(getBaggageItem),
     METH_VARARGS | METH_KEYWORDS, PyDoc_STR("retrieves a baggage item")},
    {"log_event", reinterpret_cast<PyCFunction>(logEvent),
     METH_VARARGS | METH_KEYWORDS, PyDoc_STR("log an event")},
    {"log", reinterpret_cast<PyCFunction>(log), METH_VARARGS | METH_KEYWORDS,
     PyDoc_STR("log key-values")},
    {"finish", reinterpret_cast<PyCFunction>(finish),
     METH_VARARGS | METH_KEYWORDS, PyDoc_STR("finish the span")},
    {"__enter__", reinterpret_cast<PyCFunction>(enterContext), METH_NOARGS,
     nullptr},
    {"__exit__", reinterpret_cast<PyCFunction>(exitContext), METH_VARARGS,
     nullptr},
    {nullptr, nullptr}};

//--------------------------------------------------------------------------------------------------
// SpanGetSetList
//--------------------------------------------------------------------------------------------------
static PyGetSetDef SpanGetSetList[] = {
    {const_cast<char*>("context"), reinterpret_cast<getter>(getContext), nullptr,
     const_cast<char*>(PyDoc_STR("Returns the span's context"))},
    {const_cast<char*>("tracer"), reinterpret_cast<getter>(getTracer), nullptr,
     const_cast<char*>(PyDoc_STR("Returns the tracer used to create the span"))},
    {nullptr}};

//--------------------------------------------------------------------------------------------------
// startSpan
//--------------------------------------------------------------------------------------------------
PyObject* makeSpan(std::unique_ptr<SpanBridge>&& span_bridge,
                   PyObject* tracer) noexcept {
  auto result = newPythonObject<SpanObject>(SpanType);
  if (result == nullptr) {
    return nullptr;
  }
  result->span_bridge = span_bridge.release();
  Py_INCREF(tracer);
  result->tracer = tracer;
  return reinterpret_cast<PyObject*>(result);
}

//--------------------------------------------------------------------------------------------------
// isSpan
//--------------------------------------------------------------------------------------------------
bool isSpan(PyObject* object) noexcept {
  return object->ob_type == reinterpret_cast<PyTypeObject*>(SpanType);
}

//--------------------------------------------------------------------------------------------------
// getSpanContext
//--------------------------------------------------------------------------------------------------
SpanContextBridge getSpanContextFromSpan(PyObject* object) noexcept {
  assert(isSpan(object));
  return SpanContextBridge{
      reinterpret_cast<SpanObject*>(object)->span_bridge->span()};
}

//--------------------------------------------------------------------------------------------------
// setupSpanClass
//--------------------------------------------------------------------------------------------------
bool setupSpanClass(PyObject* module) noexcept {
  TypeDescription type_description;
  type_description.name =
    PYTHON_BRIDGE_TRACER_MODULE "._Span";
  type_description.size = sizeof(SpanObject);
  type_description.doc = toVoidPtr("CppBridgeSpan");
  type_description.dealloc = toVoidPtr(deallocSpan);
  type_description.methods = toVoidPtr(SpanMethods);
  type_description.getset = toVoidPtr(SpanGetSetList);
  auto span_type = makeType<SpanObject>(type_description);
  if (span_type == nullptr) {
    return false;
  }
  SpanType = span_type;
  auto rcode = PyModule_AddObject(module, "_Span", span_type);
  return rcode == 0;
}
}  // namespace python_bridge_tracer
