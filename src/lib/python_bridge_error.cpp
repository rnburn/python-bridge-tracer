#include "python_bridge_error.h"

namespace python_bridge_tracer {
//--------------------------------------------------------------------------------------------------
// PythonBridgeErrorCategory
//--------------------------------------------------------------------------------------------------
namespace {
class PythonBridgeErrorCategory final : public std::error_category {
 public:
   const char* name() const noexcept override {
     return "PythonBridgeErrorCategory";
   }

   std::error_condition default_error_condition(int code) const noexcept override {
     return std::error_condition(code, *this);
   }

   std::string message(int code) const override {
     if (code == python_error.value()) {
       return "python error";
     }
     return "python_bridge_tracer: unknown error";
   }
 private:
};
} // namespace

//--------------------------------------------------------------------------------------------------
// python_bridge_error_category
//--------------------------------------------------------------------------------------------------
const std::error_category& python_bridge_error_category() noexcept {
  static PythonBridgeErrorCategory error_category;
  return error_category;
}
} // namespace python_bridge_tracer
