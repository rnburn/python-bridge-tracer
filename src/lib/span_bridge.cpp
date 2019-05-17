#include "span_bridge.h"

#include "utility.h"
#include "to_string.h"
#include "python_object_wrapper.h"
#include "python_string_wrapper.h"

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// setStringTag
//--------------------------------------------------------------------------------------------------
static bool setStringTag(opentracing::Span& span, opentracing::string_view key,
    PyObject* value) noexcept {
  PythonObjectWrapper utf8 = PyUnicode_AsUTF8String(value);
  if (utf8.error()) {
    return false;
  }
  char* s;
  auto rcode = PyBytes_AsStringAndSize(utf8, &s, nullptr);
  if (rcode == -1) {
    return false;
  }
  span.SetTag(key, s);
  return true;
}

//--------------------------------------------------------------------------------------------------
// getTimestamp
//--------------------------------------------------------------------------------------------------
static std::chrono::system_clock::time_point getTimestamp(double py_timestamp) {
  if (py_timestamp != 0) {
    return toTimestamp(py_timestamp);
  }
  return std::chrono::system_clock::now();
}

//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
SpanBridge::SpanBridge(std::unique_ptr<opentracing::Span>&& span) noexcept
  : span_{span.release()} 
{}

SpanBridge::SpanBridge(std::shared_ptr<opentracing::Span> span) noexcept
    : span_{std::move(span)} {}

//--------------------------------------------------------------------------------------------------
// setOperationName
//--------------------------------------------------------------------------------------------------
bool SpanBridge::setOperationName(PyObject* args,
                                       PyObject* keywords) noexcept {
  static char* keyword_names[] = {const_cast<char*>("operation_name"), nullptr};
  const char* operation_name = nullptr;
  int operation_name_length = 0;
  if (PyArg_ParseTupleAndKeywords(args, keywords, "s#:set_operation_name",
                                  keyword_names, &operation_name,
                                  &operation_name_length) == 0) {
    return false;
  }
  span_->SetOperationName(opentracing::string_view{
      operation_name, static_cast<size_t>(operation_name_length)});
  return true;
}

//--------------------------------------------------------------------------------------------------
// setTagKeyValue
//--------------------------------------------------------------------------------------------------
bool SpanBridge::setTagKeyValue(opentracing::string_view key, PyObject* value) noexcept {
  opentracing::Value cpp_value;
  if (PyUnicode_Check(value) == 1) {
    return setStringTag(*span_, key, value);
  }
  if (PyBool_Check(value) == 1) {
    cpp_value = static_cast<bool>(PyObject_IsTrue(value));
  } else if (PyLong_Check(value) == 1) {
    auto long_value = PyLong_AsLong(value);
    if (long_value == -1 && PyErr_Occurred() != nullptr) {
      return false;
    }
    cpp_value = long_value;
  } else if (PyFloat_Check(value) == 1) {
    auto double_value = PyFloat_AsDouble(value);
    if (PyErr_Occurred() != nullptr) {
      return false;
    }
    cpp_value = double_value;
  } else {
    PyErr_Format(PyExc_TypeError,
                 "tag value must be a string, bool, or a numeric type");
    return false;
  }
  span_->SetTag(key, cpp_value);
  return true;
}

bool SpanBridge::setTagKeyValue(PyObject* key, PyObject* value) noexcept {
  if (PyUnicode_Check(key) != 1) {
    PyErr_Format(PyExc_TypeError, "tag key must be a string");
    return false;
  }
  PythonStringWrapper key_str{key};
  if (key_str.error()) {
    return false;
  }
  return setTagKeyValue(static_cast<opentracing::string_view>(key_str), value);
}

//--------------------------------------------------------------------------------------------------
// logKeyValues
//--------------------------------------------------------------------------------------------------
bool SpanBridge::logKeyValues(PyObject* args, PyObject* keywords) noexcept {
  static char* keyword_names[] = {const_cast<char*>("key_values"),
                                  const_cast<char*>("timestamp"), nullptr};
  PyObject* key_values = nullptr;
  double timestamp = 0;
  if (PyArg_ParseTupleAndKeywords(args, keywords, "O|d:log_kv",
                                  keyword_names, &key_values, &timestamp) == 0) {
    return false;
  }
  if (PyDict_Check(key_values) == 0) {
    PyErr_Format(PyExc_TypeError, "key_values must be a dict");
    return false;
  }
  opentracing::LogRecord log_record;
  log_record.timestamp = getTimestamp(timestamp);
  log_record.fields.reserve(static_cast<size_t>(PyDict_Size(key_values)));
  PyObject* key;
  PyObject* value;
  Py_ssize_t position = 0;
  while (PyDict_Next(key_values, &position, &key, &value) == 1) {
    PythonStringWrapper key_str{key};
    if (key_str.error()) {
      return false;
    }
    std::string value_str;
    if (!toString(value, value_str)) {
      return false;
    }
    log_record.fields.emplace_back(
        std::string{static_cast<opentracing::string_view>(key_str)},
        opentracing::Value{std::move(value_str)});
  }
  finish_span_options_.log_records.emplace_back(std::move(log_record));
  return true;
}

bool SpanBridge::logKeyValues(
    std::initializer_list<std::pair<const char*, PyObject*>> key_values,
    double py_timestamp) noexcept {
  opentracing::LogRecord log_record;
  log_record.timestamp = getTimestamp(py_timestamp);
  log_record.fields.reserve(static_cast<size_t>(key_values.size()));
  for (auto& key_value : key_values) {
    if (key_value.second == nullptr) {
      continue;
    }
    PythonStringWrapper value_str{key_value.second};
    if (value_str.error()) {
      return false;
    }
    log_record.fields.emplace_back(
        key_value.first,
        opentracing::Value{
            std::string{static_cast<opentracing::string_view>(value_str)}});
  }
  finish_span_options_.log_records.emplace_back(std::move(log_record));
  return true;
}

//--------------------------------------------------------------------------------------------------
// logEvent
//--------------------------------------------------------------------------------------------------
bool SpanBridge::logEvent(PyObject* args, PyObject* keywords) noexcept {
  static char* keyword_names[] = {const_cast<char*>("event"),
                                  const_cast<char*>("payload"), nullptr};
  PyObject* event = nullptr;
  PyObject* payload = nullptr;
  if (PyArg_ParseTupleAndKeywords(args, keywords, "O|O:log_event",
                                  keyword_names, &event, &payload) == 0) {
    return false;
  }
  if (payload == nullptr) {
    return logKeyValues({{"event", event}});
  }
  return logKeyValues({{"event", event}, {"payload", payload}});
}

//--------------------------------------------------------------------------------------------------
// log
//--------------------------------------------------------------------------------------------------
bool SpanBridge::log(PyObject* args, PyObject* keywords) noexcept {
  static char* keyword_names[] = {const_cast<char*>("event"),
                                  const_cast<char*>("payload"),
                                  const_cast<char*>("timestamp"), nullptr};
  PyObject* event = nullptr;
  PyObject* payload = nullptr;
  double timestamp = 0;
  if (PyArg_ParseTupleAndKeywords(args, keywords, "|OOd:log", keyword_names,
                                  &event, &payload, &timestamp) == 0) {
    return false;
  }
  return logKeyValues({{"event", event}, {"payload", payload}}, timestamp);
}

//--------------------------------------------------------------------------------------------------
// setBaggageItem
//--------------------------------------------------------------------------------------------------
bool SpanBridge::setBaggageItem(PyObject* args,
                                     PyObject* keywords) noexcept {
  static char* keyword_names[] = {const_cast<char*>("key"),
                                  const_cast<char*>("value"), nullptr};
  const char* key_data = nullptr;
  int key_length = 0;
  const char* value_data = nullptr;
  int value_length = 0;
  if (PyArg_ParseTupleAndKeywords(args, keywords, "s#s#:set_baggage_item",
                                  keyword_names, &key_data, &key_length,
                                  &value_data, &value_length) == 0) {
    return false;
  }
  span_->SetBaggageItem(
      opentracing::string_view{key_data, static_cast<size_t>(key_length)},
      opentracing::string_view{value_data, static_cast<size_t>(value_length)});
  return true;
}

//--------------------------------------------------------------------------------------------------
// getBaggageItem
//--------------------------------------------------------------------------------------------------
PyObject* SpanBridge::getBaggageItem(PyObject* args, PyObject* keywords) noexcept {
  static char* keyword_names[] = {const_cast<char*>("key"), nullptr};
  const char* key_data = nullptr;
  int key_length = 0;
  if (PyArg_ParseTupleAndKeywords(args, keywords, "s#:get_baggage_item",
                                  keyword_names, &key_data, &key_length) == 0) {
    return nullptr;
  }
  auto value = span_->BaggageItem(
      opentracing::string_view{key_data, static_cast<size_t>(key_length)});
  if (value.empty()) {
    Py_RETURN_NONE;
  }
  return PyUnicode_FromStringAndSize(value.data(), static_cast<Py_ssize_t>(value.size()));
}

//--------------------------------------------------------------------------------------------------
// setTag
//--------------------------------------------------------------------------------------------------
bool SpanBridge::setTag(PyObject* args, PyObject* keywords) noexcept {
  static char* keyword_names[] = {const_cast<char*>("key"),
                                  const_cast<char*>("value"), nullptr};
  const char* key_data;
  int key_length;
  PyObject* value;
  if (PyArg_ParseTupleAndKeywords(args, keywords, "s#O:set_tag", keyword_names,
                                  &key_data, &key_length, &value) == 0) {
    return false;
  }
  return setTagKeyValue(
      opentracing::string_view{key_data, static_cast<size_t>(key_length)},
      value);
}

//--------------------------------------------------------------------------------------------------
// finish
//--------------------------------------------------------------------------------------------------
PyObject* SpanBridge::finish(PyObject* args, PyObject* keywords) noexcept {
  static char* keyword_names[] = {
    const_cast<char*>("finish_time"),
    nullptr
  };
  double finish_time = 0;
  if (PyArg_ParseTupleAndKeywords(
        args, keywords, "|d:finish", keyword_names, &finish_time) == 0) {
    return nullptr;
  }
  if (finish_time != 0) {
    finish_span_options_.finish_steady_timestamp =
        opentracing::convert_time_point<std::chrono::steady_clock>(
            toTimestamp(finish_time));
  }
  span_->FinishWithOptions(finish_span_options_);
  Py_RETURN_NONE;
}

//--------------------------------------------------------------------------------------------------
// exit
//--------------------------------------------------------------------------------------------------
PyObject* SpanBridge::exit(PyObject* args) noexcept {
  PyObject* exc_type;
  PyObject* exc_value;
  PyObject* traceback;
  if (PyArg_ParseTuple(args, "OOO", &exc_type, &exc_value, &traceback) == 0) {
    return nullptr;
  }
  if (exc_type != Py_None) {
    span_->SetTag("error", true);
    std::string exc_value_str;
    if (!toString(exc_value, exc_value_str)) {
      return nullptr;
    }
    std::string exc_type_str;
    if (!toString(exc_type, exc_type_str)) {
      return nullptr;
    }
    std::string traceback_str;
    if (!toString(traceback, traceback_str)) {
      return nullptr;
    }
    span_->Log({{"event", "error"},
                {"message", std::move(exc_value_str)},
                {"error.object", std::move(exc_value_str)},
                {"error.kind", std::move(exc_type_str)},
                {"stack", std::move(traceback_str)}});
  }
  span_->FinishWithOptions(finish_span_options_);
  Py_RETURN_NONE;
}
}  // namespace python_bridge_tracer
