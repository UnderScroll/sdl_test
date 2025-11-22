cmake_minimum_required(VERSION 3.31)

project(
  sdl_test
  VERSION 0.1
  DESCRIPTION "A test project to learn SDL3 and Cmake"
  LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Local dependencies subdirectory
add_subdirectory(dependencies)

# External dependencies
include(FetchContent)

# SDL3
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
if (NOT SDL3_FOUND)
  message(STATUS "External dependency [SDL3] not found. Falling back to Fetching...")

  FetchContent_Declare(
    SDL3
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
    GIT_TAG        "release-3.2.26"
  )

  FetchContent_MakeAvailable(SDL3)
else()
  message(STATUS "External dependency [SDL3] found at ${SDL3_DIR}")
endif()

# magic_enum
find_package(magic_enum CONFIG QUIET)
if (NOT magic_enum_FOUND)
  message(STATUS "External dependency [magic-enum] not found. Falling back to Fetching...")

  FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY "https://github.com/Neargye/magic_enum.git"
    GIT_TAG        "v0.9.7"
  )

  FetchContent_MakeAvailable(magic_enum)
else()
  message(STATUS "External dependency [magic-enum] found at ${magic_enum_DIR}")
endif()

# glm
find_package(glm CONFIG QUIET)
if (NOT glm_FOUND)
  message(STATUS "External dependency [glm] not found. Falling back to Fetching...")

  FetchContent_Declare(
    glm
    GIT_REPOSITORY "https://github.com/g-truc/glm.git"
    GIT_TAG        "1.0.2"
  )

  FetchContent_MakeAvailable(glm)
else()
  message(STATUS "External dependency [glm] found at ${glm_DIR}")
endif()

# Target
file(
  GLOB_RECURSE
  src_files
  CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/*.cpp"
)
add_executable(${PROJECT_NAME} ${src_files})

# Target parameters
target_compile_features(
  ${PROJECT_NAME}
  PRIVATE
  cxx_std_23
)
target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_options(
  ${PROJECT_NAME}
  PRIVATE
    "-fmacro-prefix-map=${CMAKE_CURRENT_SOURCE_DIR}/="
)

# Libraries
# Local
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
    unders_helpers
)
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
    string_extension
)
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
    rerror
)
# External
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
    SDL3::SDL3
)
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
    magic_enum::magic_enum
)
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
    glm::glm
)
