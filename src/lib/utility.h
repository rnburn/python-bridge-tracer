#pragma once

#include <Python.h>

#include <chrono>
#include <utility>
#include <type_traits>

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

PyObject* getModuleAttribute(const char* module_name,
                             const char* attribute) noexcept;

std::chrono::system_clock::time_point toTimestamp(double py_timestamp) noexcept;
}  // namespace python_bridge_tracer
