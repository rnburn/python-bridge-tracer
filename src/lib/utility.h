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

// FinalAction and finally
//
// Taken from
// https://github.com/Microsoft/GSL/blob/cea0d0ac2bd775f0fb4c7e357a089979370ae3cd/include/gsl/gsl_util
//
// FinalAction allows you to ensure something gets run at the end of a scope
template <class F>
class FinalAction {
 public:
  explicit FinalAction(F f) noexcept : f_(std::move(f)) {}

  FinalAction(FinalAction&& other) noexcept
      : f_(std::move(other.f_)), invoke_(other.invoke_) {
    other.invoke_ = false;
  }

  FinalAction(const FinalAction&) = delete;
  FinalAction& operator=(const FinalAction&) = delete;
  FinalAction& operator=(FinalAction&&) = delete;

  ~FinalAction() noexcept {
    if (invoke_) {
      f_();
    }
  }

 private:
  F f_;
  bool invoke_{true};
};

// finally() - convenience function to generate a FinalAction
template <class F>
FinalAction<F> finally(F&& f) noexcept {
  return FinalAction<F>(std::forward<F>(f));
}

PyObject* getModuleAttribute(const char* module_name,
                             const char* attribute) noexcept;

std::chrono::system_clock::time_point toTimestamp(double py_timestamp) noexcept;
}  // namespace python_bridge_tracer
