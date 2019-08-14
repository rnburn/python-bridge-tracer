#pragma once

#include <Python.h>

#include <chrono>
#include <utility>
#include <type_traits>

#include <opentracing/string_view.h>

namespace python_bridge_tracer {
/**
 * Helper function to cast pointers to void*
 *
 * @param ptr the pointer to convert
 * @return a void* from ptr
 */
template <class T, typename std::enable_if<!std::is_function<T>::value>::type* = nullptr>
void* toVoidPtr(T* ptr) noexcept {
  return reinterpret_cast<void*>(ptr);
}

template <class T, typename std::enable_if<!std::is_function<T>::value>::type* = nullptr>
void* toVoidPtr(const T* ptr) noexcept {
  return toVoidPtr(const_cast<T*>(ptr));
}

template <class T, typename std::enable_if<std::is_function<T>::value>::type* = nullptr>
void* toVoidPtr(T* ptr) noexcept {
  return reinterpret_cast<void*>(ptr);
}

/** 
 * Contruct a new python class.
 *
 * Like PyObject_New except that it doesn't use c-casts so that clang-tidy won't complain.
 *
 * @param type the python type object for the class
 * @return the newly consturcted class.
 */
template <class T>
T* newPythonObject(PyObject* type) {
  auto result = _PyObject_New(reinterpret_cast<PyTypeObject*>(type));
  return reinterpret_cast<T*>(result);
}

/**
 * Import and lookup an attriburte from a module.
 * @param module_name the module to import
 * @param attribute the attribute to lookup
 * @return the object from the attribute lookup
 */
PyObject* getModuleAttribute(const char* module_name,
                             const char* attribute) noexcept;

/**
 * Convert a python time value to a C++ timestamp.
 * @param py_timestamp the python timestamp (seconds since epoch)
 * @return a C++ timepoint
 */
std::chrono::system_clock::time_point toTimestamp(double py_timestamp) noexcept;

/**
 * Check if a given object is a string
 * @param obj the object to check
 * @return true if obj is a string
 */
bool isString(PyObject* obj) noexcept;

/**
 * Check if an object is an integer
 * @param obj the object to check
 * @return true if obj is an integer
 */
bool isInt(PyObject* obj) noexcept;

/**
 * Convert an object to an integer
 * @param obj the object to convert
 * @param value the converted integer
 * @return true if successful
 */
bool toLong(PyObject* obj, long& value) noexcept;

/**
 * Converts a C++ string to a python string
 * @param data the string's data
 * @param size the size of the string
 * @return a Python string object
 */
PyObject* toPyString(opentracing::string_view s) noexcept;
}  // namespace python_bridge_tracer
