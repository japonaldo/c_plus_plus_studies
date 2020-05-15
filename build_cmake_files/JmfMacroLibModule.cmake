cmake_minimum_required(VERSION 3.13)

include(CMakeParseArguments)

macro(ADD_JMF_LIB_MODULE)
  set(options FORCE_BUILD_SHARED IS_PUBLIC)
  set(single_args NAME)
  set(multi_args SYS_INCLUDE_DIRS INCLUDE_DIRS SRC DEPS FLAGS TARGET_DEPS)
  CMAKE_PARSE_ARGUMENTS(JMF_LIB_MODULE "${options}" "${single_args}" "${multi_args}" ${ARGN})

  include_directories(${JMF_LIB_MODULE_INCLUDE_DIRS})
  include_directories(SYSTEM ${JMF_LIB_MODULE_SYS_INCLUDE_DIRS})

  string(TOLOWER "${JMF_SYSTEM_NAME}" system)

  set(JMF_LIB_MODULE_NAME_STATIC ${JMF_LIB_MODULE_NAME}-static)
  add_library(${JMF_LIB_MODULE_NAME_STATIC} STATIC ${JMF_LIB_MODULE_SRC})
  target_link_libraries(${JMF_LIB_MODULE_NAME_STATIC} PUBLIC ${JMF_LIB_MODULE_DEPS})
  set_target_properties(${JMF_LIB_MODULE_NAME_STATIC}
                        PROPERTIES OUTPUT_NAME ${JMF_LIB_MODULE_NAME}-static)

  install(TARGETS ${JMF_LIB_MODULE_NAME_STATIC}
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib
          RUNTIME DESTINATION bin
  )

  if(DEFINED JMF_LIB_MODULE_TARGET_DEPS)
    add_dependencies(${JMF_LIB_MODULE_NAME_STATIC} ${JMF_LIB_MODULE_TARGET_DEPS})
  endif()

  add_definitions(${JMF_LIB_MODULE_FLAGS})
endmacro()
