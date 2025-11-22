cmake_minimum_required(VERSION 3.31)

project(
  rerror
  DESCRIPTION "Adds a rust like error type to be used in std::unexpected"
  VERSION 0.1
  LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Target
add_library(
  ${PROJECT_NAME}
  INTERFACE
)

# Target parameters
target_compile_features(
  ${PROJECT_NAME}
  INTERFACE
    cxx_std_23
)
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Libraries
target_link_libraries(
  ${PROJECT_NAME}
  INTERFACE
    unders_helpers
)

target_link_libraries(
  ${PROJECT_NAME}
  INTERFACE
    string_extension
)

target_link_libraries(
  ${PROJECT_NAME}
  INTERFACE
    magic_enum::magic_enum
)

# Expose headers
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
