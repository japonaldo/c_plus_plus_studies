# TODO add Copyright

include(JmfMacroExecutable)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

ADD_JMF_EXECUTABLE(
  NAME               ${JMF_MODULE_NAME}
  SRC                ${SOURCES}
  SYS_INCLUDE_DIRS   SYSTEM
                     ${JMF_3RD_PARTY_ROOT_DIR}/nlohmann_json/single_include/
  INCLUDE_DIRS       ${CMAKE_CURRENT_SOURCE_DIR}
  LINKER_FLAGS  
  DEPS               curl
  )
