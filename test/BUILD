load("@python_pip_deps//:requirements.bzl", "requirement")

load(
    "//bazel:python_bridge_build_system.bzl",
    "python_bridge_test",
    "python_bridge_package",
)

python_bridge_package()

python_bridge_test(
    name = "tracer_test_py3",
    srcs = [
        "tracer_test.py",
    ],
    main = "tracer_test.py",
    data = [
        "//binary/py3:bridge_tracer.so",
        "@io_opentracing_cpp//mocktracer:libmocktracer_plugin.so",
    ],
    deps = [
        requirement("opentracing"),
    ],
)

python_bridge_test(
    name = "tracer_test_py27",
    srcs = [
        "tracer_test.py",
    ],
    main = "tracer_test.py",
    data = [
        "//binary/py27mu:bridge_tracer.so",
        "@io_opentracing_cpp//mocktracer:libmocktracer_plugin.so",
    ],
    python_version = "PY2",
    deps = [
        requirement("opentracing"),
    ],
)
