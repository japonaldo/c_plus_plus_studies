# TODO add Copyright

include(JmfMacroExecutable)
include(JmfMacroLibModule)

get_filename_component(JMF_MODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set(JMF_TEST_MODULE_NAME ${JMF_UNIT_TEST_PREFIX}${JMF_MODULE_NAME})

file(GLOB_RECURSE SOURCES *.cpp)

ADD_JMF_EXECUTABLE(
  NAME               ${JMF_TEST_MODULE_NAME}
  SRC                ${SOURCES}
  SYS_INCLUDE_DIRS   SYSTEM
  INCLUDE_DIRS       ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}/src
                     LINKER_FLAGS  
                     DEPS gtest gmock 
)
