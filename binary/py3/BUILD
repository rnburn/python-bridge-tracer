load(
    "//bazel:python_bridge_build_system.bzl",
    "python_bridge_cc_binary",
    "python_bridge_package",
)

python_bridge_package()

cc_binary(
    name = "bridge_tracer.so",
    linkshared = True,
    deps = [
      "//:bridge_tracer_module_lib_py3",
    ],
    visibility = [
        "//visibility:public",
    ],
)
