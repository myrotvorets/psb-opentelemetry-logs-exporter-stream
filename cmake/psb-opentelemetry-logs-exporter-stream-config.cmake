get_filename_component(PSB_OTEL_LOG_EXPORTER_STREAM_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

list(APPEND CMAKE_MODULE_PATH ${PSB_OTEL_LOG_EXPORTER_STREAM_CMAKE_DIR})

include(CMakeFindDependencyMacro)
find_dependency(opentelemetry-cpp)

if(NOT TARGET psb-opentelemetry-logs-exporter-stream)
    include("${PSB_OTEL_LOG_EXPORTER_STREAM_CMAKE_DIR}/psb-opentelemetry-logs-exporter-stream-target.cmake")
    add_library(psb::opentelemetry::logs-exporter-stream ALIAS psb-opentelemetry-logs-exporter-stream)
endif()
