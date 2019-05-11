#pragma once

#include <memory>

#include <Python.h>

#include "opentracing/span.h"

namespace python_bridge_tracer {
/**
 * Translates OpenTracing-Python span methods to their OpenTracing-C++ equivalents.
 */
class SpanBridge {
 public:
   explicit SpanBridge(std::unique_ptr<opentracing::Span>&& span) noexcept;

   explicit SpanBridge(std::shared_ptr<opentracing::Span> span) noexcept;

   /**
    * @return the OpenTracing-C++ span associated with the bridge.
    */
   std::shared_ptr<const opentracing::Span> span() noexcept { return span_; }

   /**
    * Change the operation name of a span.
    * @param args python function arguments
    * @param keywrods python function keywords
    * @return Py_None on success
    */
   PyObject* setOperationName(PyObject* args, PyObject* keywords) noexcept;

   /**
    * Set a tag on the span.
    * @param key the tag's key as a string_view
    * @param value a python object of the tag's value
    * @return true on success
    */
   bool setTagKeyValue(opentracing::string_view key, PyObject* value) noexcept;

   /**
    * Set a tag on the span.
    * @param key the tag's key as a python object
    * @param value a python object of the tag's value
    * @return true on success
    */
   bool setTagKeyValue(PyObject* key, PyObject* value) noexcept;

   /**
    * Set a tag on the span.
    * @param args python function arguments
    * @param keywrods python function keywords
    * @return Py_None on success
    */
   PyObject* setTag(PyObject* args, PyObject* keywords) noexcept;

   /**
    * Set a baggage item on the span.
    * @param args python function arguments
    * @param keywrods python function keywords
    * @return Py_None on success
    */
   PyObject* setBaggageItem(PyObject* args, PyObject* keywords) noexcept;

   /**
    * Get a baggage item on the span.
    * @param args python function arguments
    * @param keywrods python function keywords
    * @return a python string with the value of the baggage item
    */
   PyObject* getBaggageItem(PyObject* args, PyObject* keywords) noexcept;

   /**
    * Finish the span.
    * @param args python function arguments
    * @param keywrods python function keywords
    * @return Py_None on success
    */
   PyObject* finish(PyObject* args, PyObject* keywords) noexcept;

   /**
    * Implement the span's __exit__ method for use with python context management
    * @param args the python function arguments
    * @return Py_None on success
    */
   PyObject* exit(PyObject* args) noexcept;
 private:
  std::shared_ptr<opentracing::Span> span_;
  opentracing::FinishSpanOptions finish_span_options_;
};
} // namespace python_bridge_tracer
