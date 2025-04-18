cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(EXISTS "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(
    psb-opentelemetry-logs-exporter-stream
    VERSION 0.0.1
    DESCRIPTION "Stream logs exporter for OpenTelemetry"
    LANGUAGES CXX
)

set(CMAKE_VERBOSE_MAKEFILE ON)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_TESTING "Whether to enable tests" ${PROJECT_IS_TOP_LEVEL})
option(INSTALL_STREAM_EXPORTER "Whether to enable install targets" ${PROJECT_IS_TOP_LEVEL})
option(ENABLE_MAINTAINER_MODE "Enable maintainer mode" OFF)
option(USE_CLANG_TIDY "Use clang-tidy" OFF)

include(build_types)
include(tools)
include(coverage)

find_package(opentelemetry-cpp CONFIG REQUIRED)

add_subdirectory(src)
if(BUILD_TESTING)
    include(FindGTest)
    find_package(GTest CONFIG REQUIRED)

    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif()

message(STATUS "Configuration summary:")
message(STATUS "  - Build shared libraries: ${BUILD_SHARED_LIBS}")
message(STATUS "  - Run tests: ${BUILD_TESTING}")
message(STATUS "  - Enable coverage: ${ENABLE_COVERAGE}")
message(STATUS "  - Generate install target: ${INSTALL_STREAM_EXPORTER}")
message(STATUS "  - Maintainer mode: ${ENABLE_MAINTAINER_MODE}")
message(STATUS "  - Use clang-tidy: ${USE_CLANG_TIDY}")
