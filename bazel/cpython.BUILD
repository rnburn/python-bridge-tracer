#genrule(
#  name = "headers",
#  srcs = glob(["**"]),
#  cmd = "cp -r $$(dirname $(location configure))/Include $(location cpython_include_dir)",
#  visibility = [
#      "//visibility:public",
#  ],
#  outs = ["cpython_include_dir"],
#)

cc_library(
  name = "cpython_header_lib",
  srcs = glob(["Include/**/*.h"]),
  visibility = [
      "//visibility:public",
  ],
  includes = ["Include"],
  strip_include_prefix = "Include",
  deps = [
    "@vendored_pyconfig//:pyconfig_lib",
  ],
)
