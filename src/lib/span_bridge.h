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
    * @return true on success
    */
   bool setOperationName(PyObject* args, PyObject* keywords) noexcept;

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
    * @return true on success
    */
   bool setTag(PyObject* args, PyObject* keywords) noexcept;

   /**
    * Log key-values.
    * @param args python function arguments
    * @param keywrods python function keywords
    * @return true on success
    */
   bool logKeyValues(PyObject* args, PyObject* keywords) noexcept;

   /**
    * Alternative deprecated version of log.
    * @param args python function arguments
    * @param keywrods python function keywords
    * @return true on success
    */
   bool logEvent(PyObject* args, PyObject* keywords) noexcept;

   /**
    * Alternative deprecated version of log.
    * @param args python function arguments
    * @param keywrods python function keywords
    * @return true on success
    */
   bool log(PyObject* args, PyObject* keywords) noexcept;

   /**
    * Set a baggage item on the span.
    * @param args python function arguments
    * @param keywrods python function keywords
    * @return true on success
    */
   bool setBaggageItem(PyObject* args, PyObject* keywords) noexcept;

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

  bool logKeyValues(
      std::initializer_list<std::pair<const char*, PyObject*>> key_values,
      double py_timestamp = 0) noexcept;
};
} // namespace python_bridge_tracer
