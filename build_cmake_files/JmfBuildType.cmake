cmake_minimum_required(VERSION 3.13)

if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

STRING(TOLOWER ${CMAKE_BUILD_TYPE} JMF_BUILD_TYPE_DIR)

# -Og should be the optimization level of choice for the standard edit-compile-debug cycle, 
# offering a reasonable level of optimization while maintaining fast compilation 
# and a good debugging experience. It is a better choice than -O0 for producing debuggable 
# code because some compiler passes that collect debug information are disabled at -O0.
# from: https://interrupt.memfault.com/blog/gdb-for-firmware-1
# Debug flags
set(CMAKE_C_FLAGS_DEBUG   "-Og -g3")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g3")

# DebugFull flags
set(CMAKE_C_FLAGS_DEBUGFULL   "-Og -g3 -ggdb3")
set(CMAKE_CXX_FLAGS_DEBUGFULL "-Og -g3 -ggdb3")

# Release flags (with Debug info)
set(CMAKE_C_FLAGS_RELEASE   "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

enable_language(C CXX ASM)

if(DEFINED JMF_SYSTEM_NAME)
  include(${JMF_SYSTEM_NAME})
else()
  include(${CMAKE_SYSTEM_NAME})
  set(JMF_SYSTEM_NAME ${CMAKE_SYSTEM_NAME}) 
endif()
message(STATUS "Target system type:${JMF_SYSTEM_NAME}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Building with DEBUG information")
  add_definitions(-DDEBUG)

elseif(CMAKE_BUILD_TYPE STREQUAL "DebugFull")
  message(STATUS "Building with DEBUGFULL information")
  add_definitions(-DDEBUG)
  add_definitions(-DDEBUG_FULL)

elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  message(STATUS "Building with RELEASE information")
else()
  message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}. Use:\n Release | Debug | DebugFull")
endif()


