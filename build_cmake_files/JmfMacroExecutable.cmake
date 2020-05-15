cmake_minimum_required(VERSION 3.13)

include(CMakeParseArguments)

macro(ADD_JMF_EXECUTABLE)
  set(options)
  set(single_args NAME)
  set(multi_args SYS_INCLUDE_DIRS INCLUDE_DIRS SRC DEPS LD_SCRIPT LINKER_FLAGS TARGET_DEPS)
  CMAKE_PARSE_ARGUMENTS(JMF_EXECUTABLE "${options}" "${single_args}" "${multi_args}" ${ARGN})

  message(STATUS "####################################################")
  message(STATUS "# Adding for compiling : ${JMF_EXECUTABLE_NAME}")

  if(CMAKE_MESSAGE_VERBOSE)
    message(STATUS "# SRC: ${CSRC}")
  endif()

  message(STATUS "####################################################")

  include_directories(${JMF_EXECUTABLE_INCLUDE_DIRS})
  include_directories(SYSTEM ${JMF_EXECUTABLE_SYS_INCLUDE_DIRS})
  add_executable(${JMF_EXECUTABLE_NAME} ${JMF_EXECUTABLE_SRC})

  if(DEFINED JMF_EXECUTABLE_TARGET_DEPS)
    add_dependencies(${JMF_EXECUTABLE_NAME} ${JMF_EXECUTABLE_TARGET_DEPS})
  endif()

  target_link_libraries(${JMF_EXECUTABLE_NAME} PUBLIC ${JMF_EXECUTABLE_DEPS})
  
  if (DEFINED JMF_EXECUTABLE_LINKER_FLAGS)
    set_property(TARGET ${JMF_EXECUTABLE_NAME} APPEND_STRING PROPERTY LINK_FLAGS " ${JMF_EXECUTABLE_LINKER_FLAGS}")
  endif()

  install(TARGETS ${JMF_EXECUTABLE_NAME} RUNTIME
          CONFIGURATIONS Debug DebugFull Release
          DESTINATION bin)
  
endmacro()
