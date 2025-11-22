cmake_minimum_required(VERSION 3.31)

project(
  unders_helpers
  VERSION 0.1
  DESCRIPTION "Miscellanious header only library used in UnderScroll's projects"
  LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Target
add_library(${PROJECT_NAME} INTERFACE)

# Target parameters
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
