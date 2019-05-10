#pragma once

#include <opentracing/tracer.h>

namespace python_bridge_tracer {
/**
 * Create an opentracing::Tracer from a dynamically loaded vendor plugin
 * @param tracer_library a vendor's tracer plugin
 * @param config a json config used to construct the tracer
 * @return the vendor's tracer
 */
std::shared_ptr<opentracing::Tracer> makeDynamicTracer(
    const char* tracer_library, const char* config);
}  // namespace python_bridge_tracer
