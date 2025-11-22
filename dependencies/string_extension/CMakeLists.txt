cmake_minimum_required(VERSION 3.31)

project(
  string_extension
  DESCRIPTION "Adds simple helper functions to be used with std::string"
  VERSION 0.1
  LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Target
file(GLOB src_files CONFIGURE_DEPENDS "src/*.cpp")

add_library(
  ${PROJECT_NAME}
  STATIC
  ${src_files}
)

# Target parameters
target_compile_features(
  ${PROJECT_NAME}
  PUBLIC
    cxx_std_23
)
target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Libraries
target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
    unders_helpers
)

# Expose headers
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
