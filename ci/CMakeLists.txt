cmake_minimum_required(VERSION 3.25)

message("${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake")

project(test-logs-exporter-stream VERSION 0.0.1 LANGUAGES CXX)

find_package(psb-opentelemetry-logs-exporter-stream CONFIG REQUIRED)

add_executable("${PROJECT_NAME}" test-logs-exporter-stream.cpp)
target_link_libraries("${PROJECT_NAME}" PRIVATE psb::opentelemetry::logs-exporter-stream)
