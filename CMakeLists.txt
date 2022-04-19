cmake_minimum_required(VERSION 3.14)

set(LIB_NAME event_batch)
project(${LIB_NAME} LANGUAGES CXX)

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(${LIB_NAME}_DOC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc)
set(${LIB_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(${LIB_NAME}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(${LIB_NAME}_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)

# CXX flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -march=native -mtune=native")

# Default to Release
set(DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Options
option(${LIB_NAME}_BUILD_RUNTIME_BENCHMARK "Build runtime benchmark" OFF)
option(${LIB_NAME}_BUILD_TEST "Build tests" OFF)
option(${LIB_NAME}_BUILD_DOC "Build documentation" OFF)

# Dependencies
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

# pontella
FetchContent_Declare(
  pontella
  GIT_REPOSITORY https://github.com/neuromorphic-paris/pontella
  GIT_TAG        master
)
FetchContent_MakeAvailable(pontella)

# sepia
FetchContent_Declare(
  sepia
  GIT_REPOSITORY https://github.com/neuromorphic-paris/sepia
  GIT_TAG        master
)
FetchContent_MakeAvailable(sepia)

# Source
add_subdirectory(${${LIB_NAME}_SOURCE_DIR})

# Tests
if(${LIB_NAME}_BUILD_TEST)
  enable_testing()
  add_subdirectory(${${LIB_NAME}_TEST_DIR})
endif()

# Documentation
if(${LIB_NAME}_BUILD_DOC)
  add_subdirectory(${${LIB_NAME}_DOC_DIR})
endif()
